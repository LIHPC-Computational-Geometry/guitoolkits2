//
// CP : fichier issu de convertior.cc de omniORB 4.0.7
//
// Objectif : migrer une IOR de profile de type socket UNIX sur la machine
// cliente pour supporter le tunneling dans de telles circonstances.
//

// -*- Mode: C++; -*-
//                              Package   : convertior
// convertior.cc                Author    : Eoin Carroll (ewc)
//
//    Copyright (C) 1997-1999 AT&T Laboratories Cambridge
//
//  This file is part of convertior.
//
//  Convertior is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
//  USA.
//
//
// Takes an IOR, hostname as arguments. Outputs IOR with new hostname.

#ifdef OMNIORB	// CP

#include <string.h>
#include <stdlib.h>
#include <stdio.h>	// CP
#include <sstream>	// CP
#include <TkUtil/UTF8String.h>		// CP
#include <TkUtil/Exception.h>		// CP
using namespace TkUtil;		// CP

static const Charset	charset ("àéèùô");


#include <omniORB4/CORBA.h>
#include <omniORB4/omniIOR.h>		// CP v 5.0.0

#  include <iostream>
using namespace std;

// unistd.h on some platforms, Mac OS X 10.4 for example, defines a macro called minor.
#undef minor

#ifndef Swap16
#define Swap16(s) ((((s) & 0xff) << 8) | (((s) >> 8) & 0xff))
#else
#error "Swap16 has already been defined"
#endif

#ifndef Swap32
#define Swap32(l) ((((l) & 0xff000000) >> 24) | \
		   (((l) & 0x00ff0000) >> 8)  | \
		   (((l) & 0x0000ff00) << 8)  | \
		   (((l) & 0x000000ff) << 24))
#else
#error "Swap32 has already been defined"
#endif


//
// CP v 5.0.0 (28/10/19)
//
// For the TAGs that the ORB will look at, add a handler to the following table.
//
/*
static struct {
  IOP::ComponentId id;
  void (*fn)(const IOP::TaggedComponent&,omniIOR&);
  char* (*dump)(const IOP::TaggedComponent&);
} componentUnmarshalHandlers[] = {
  // This table must be arranged in ascending order of IOP::ComponentId

  { IOP::TAG_ORB_TYPE,
    omniIOR::unmarshal_TAG_ORB_TYPE,
    omniIOR::dump_TAG_ORB_TYPE },

  { IOP::TAG_CODE_SETS,
    omniIOR::unmarshal_TAG_CODE_SETS,
    omniIOR::dump_TAG_CODE_SETS },

  { IOP::TAG_POLICIES, 0, 0 },

  { IOP::TAG_ALTERNATE_IIOP_ADDRESS,
    omniIOR::unmarshal_TAG_ALTERNATE_IIOP_ADDRESS,
    omniIOR::dump_TAG_ALTERNATE_IIOP_ADDRESS },

  { IOP::TAG_COMPLETE_OBJECT_KEY, 0, 0 },
  { IOP::TAG_ENDPOINT_ID_POSITION, 0, 0 },
  { IOP::TAG_LOCATION_POLICY, 0, 0 },
  { IOP::TAG_ASSOCIATION_OPTIONS, 0, 0 },
  { IOP::TAG_SEC_NAME, 0, 0 },
  { IOP::TAG_SPKM_1_SEC_MECH, 0, 0 },
  { IOP::TAG_SPKM_2_SEC_MECH, 0, 0 },
  { IOP::TAG_KERBEROSV5_SEC_MECH, 0, 0 },
  { IOP::TAG_CSI_ECMA_SECRET_SEC_MECH, 0, 0 },
  { IOP::TAG_CSI_ECMA_HYBRID_SEC_MECH, 0, 0 },

  { IOP::TAG_SSL_SEC_TRANS,
    omniIOR::unmarshal_TAG_SSL_SEC_TRANS,
    omniIOR::dump_TAG_SSL_SEC_TRANS },

  { IOP::TAG_CSI_ECMA_PUBLIC_SEC_MECH, 0, 0 },
  { IOP::TAG_GENERIC_SEC_MECH, 0, 0 },
  { IOP::TAG_FIREWALL_TRANS, 0, 0 },
  { IOP::TAG_SCCP_CONTACT_INFO, 0, 0 },
  { IOP::TAG_JAVA_CODEBASE, 0, 0 },

  { IOP::TAG_CSI_SEC_MECH_LIST,
    omniIOR::unmarshal_TAG_CSI_SEC_MECH_LIST,
    omniIOR::dump_TAG_CSI_SEC_MECH_LIST },

  { IOP::TAG_DCE_STRING_BINDING, 0, 0 },
  { IOP::TAG_DCE_BINDING_NAME, 0, 0 },
  { IOP::TAG_DCE_NO_PIPES, 0, 0 },
  { IOP::TAG_DCE_SEC_MECH, 0, 0 },
  { IOP::TAG_INET_SEC_TRANS, 0, 0 },

  { IOP::TAG_GROUP,
    omniIOR::unmarshal_TAG_GROUP,
    omniIOR::dump_TAG_GROUP  },

  { IOP::TAG_PRIMARY, 0, 0 },
  { IOP::TAG_HEARTBEAT_ENABLED, 0, 0 },

  { IOP::TAG_OMNIORB_BIDIR,
    omniIOR::unmarshal_TAG_OMNIORB_BIDIR,
    omniIOR::dump_TAG_OMNIORB_BIDIR },

  { IOP::TAG_OMNIORB_UNIX_TRANS,
    omniIOR::unmarshal_TAG_OMNIORB_UNIX_TRANS,
    omniIOR::dump_TAG_OMNIORB_UNIX_TRANS },

  { IOP::TAG_OMNIORB_PERSISTENT_ID,
    omniIOR::unmarshal_TAG_OMNIORB_PERSISTENT_ID,
    omniIOR::dump_TAG_OMNIORB_PERSISTENT_ID },

  { 0xffffffff, 0, 0 }
};
*/

// CP v 6.1.0 : on ne conserve que le nécessaire : IOP::TAG_OMNIORB_UNIX_TRANS avec 0 en 3ème champ
static struct {
  IOP::ComponentId id;
  void (*fn)(const IOP::TaggedComponent&,omniIOR&);
  char* (*dump)(const IOP::TaggedComponent&);
} componentUnmarshalHandlers[] = {
  // This table must be arranged in ascending order of IOP::ComponentId

  { IOP::TAG_OMNIORB_UNIX_TRANS,
    omniIOR::unmarshal_TAG_OMNIORB_UNIX_TRANS,
    0/*omniIOR::dump_TAG_OMNIORB_UNIX_TRANS*/ },

  { 0xffffffff, 0, 0 }
};

// Fonction IOP::dump de omniORB 4.1.7. Cette fonction est declaree dans omniORB 4.2.* mais non implementée ...
static int tablesize = 0;
char* IOP_dumpComponent(const IOP::TaggedComponent& c) {

  if (!tablesize) {
    while (componentUnmarshalHandlers[tablesize].id != 0xffffffff) tablesize++;
  }

  int top = tablesize;
  int bottom = 0;

  do {
    int i = (top + bottom) >> 1;
    IOP::ComponentId id = componentUnmarshalHandlers[i].id;
    if (id == c.tag) {
      if (componentUnmarshalHandlers[i].dump) {
	return componentUnmarshalHandlers[i].dump(c);
      }
      break;
    }
    else if (id > c.tag) {
      top = i;
    }
    else {
      bottom = i + 1;
    }
  } while (top != bottom);

  // Reach here if we don't know how to dump the content.
  CORBA::ULong len = c.component_data.length() * 2 + 4;
  const char* tagname = IOP::ComponentIDtoName(c.tag);
  if (!tagname) {
    len += sizeof("unknown tag()") + 10;
  }
  else {
    len += strlen(tagname);
  }
  CORBA::String_var outstr;
  char* p;
  outstr = p = CORBA::string_alloc(len);
  memset(p,0,len+1);

  if (tagname) {
    strcpy(p,tagname);
  }
  else {
    sprintf(p,"unknown tag(0x%08lx)",(unsigned long)c.tag);
  }
  p += strlen(p);
  *p++ = ' ';
  *p++ = '0';
  *p++ = 'x';

  CORBA::Char* data = (CORBA::Char *) c.component_data.get_buffer();

  for (CORBA::ULong i=0; i < c.component_data.length(); i++) {
    int v = (data[i] & 0xf0);
    v = v >> 4;
    if (v < 10)
      *p++ = '0' + v;
    else
      *p++ = 'a' + (v - 10);
    v = ((data[i] & 0xf));
    if (v < 10)
      *p++ = '0' + v;
    else
      *p++ = 'a' + (v - 10);
  }

  return outstr._retn();
}	// IOP_dumpComponent

static char* convertRef(const char* old_ior, const char* hostname);

string convertior (const string& ior, const string& newHost)
{
	const string	newior (convertRef (ior.c_str ( ), newHost.c_str ( )));

	if (false == newior.empty ( ))
		return string (newior);

	UTF8String	error (charset);
	error << "Impossibilité de migrer l'IOR\n" << ior << "\n" << "sur la machine " << newHost << ".";
    throw Exception (error);
}	// convertior


void converMultipleComponentProfile (IOP::MultipleComponentProfile& components, const string& newHost, const string& unixSocket)
{	// Quand on arrive ici le nombre de components est nul
	// components.length ( );
	if (0 != components.length ( ))
	{
		CORBA::ULong	total	= components.length ( );
		for (CORBA::ULong index = 0; index < total; index++)
		{
			if (IOP::TAG_OMNIORB_UNIX_TRANS == components [index].tag)
			{
//				char*	content	= IOP::dumpComponent (components [index]);
				char*	content	= IOP_dumpComponent (components [index]);	// CP v 5.0.0
				istringstream	stream (content);
				// content est du type :
				// 				TAG_OMNIORB_UNIX_TRANS machine path
				string	tag, host, filename;
				stream >> tag >> host >> filename;
				// Code issu de ior.cc de omniORB 4.0.7 :
				cdrEncapsulationStream s (CORBA::ULong(0),CORBA::Boolean(1));
				s .marshalRawString (newHost.c_str ( ));
				s .marshalRawString (unixSocket.c_str ( ));
				CORBA::Octet* ptr	= 0;
				CORBA::ULong max	= 0, len	= 0;
				s.getOctetStream (ptr, max, len);
				components [index].component_data.replace (max, len, ptr, 1);
			}	// if (IOP::TAG_OMNIORB_UNIX_TRANS == components [index].tag)
		}	// for (CORBA::ULong index = 0; index < total; index++)
	}	// if (0 != components.length ( ))
	else
	{
		// => rien à modifier, tout à créer.
		// cf. code de la fonction insertSupportedComponents de ior.cc en ligne 1236.
		// Dans ior.cc on stocke toutes les interfaces (TCP/IP, SSL, socket  domaine Unix, ...).
		// Ici on ne supporte pour le moment qu'un socket de domaine Unix.
		IOP::TaggedComponent&	component	= omniIOR::newIIOPtaggedComponent (components);
		component.tag	= IOP::TAG_OMNIORB_UNIX_TRANS;
		// Code issu de ior.cc de omniORB 4.0.7 :
		cdrEncapsulationStream stream (CORBA::ULong(0),CORBA::Boolean(1));
		stream .marshalRawString (newHost.c_str ( ));
		stream .marshalRawString (unixSocket.c_str ( ));
		CORBA::Octet* ptr	= 0;
		CORBA::ULong max,len;
		stream.getOctetStream (ptr,max,len);
		component.component_data.replace (max,len,ptr,1);
	}	// else if (0 != components.length ( ))
}	// converMultipleComponentProfile


static void toIOR(const char* iorstr,IOP::IOR& ior)
{
  size_t s = (iorstr ? strlen(iorstr) : 0);
  if (s<4)
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);
  const char *p = iorstr;
  if (p[0] != 'I' ||
      p[1] != 'O' ||
      p[2] != 'R' ||
      p[3] != ':')
    throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);

  s = (s-4)/2;  // how many octets are there in the string
  p += 4;

  cdrMemoryStream buf((CORBA::ULong)s,0);

  for (int i=0; i<(int)s; i++) {
    int j = i*2;
    CORBA::Octet v;
    
    if (p[j] >= '0' && p[j] <= '9') {
      v = ((p[j] - '0') << 4);
    }
    else if (p[j] >= 'a' && p[j] <= 'f') {
      v = ((p[j] - 'a' + 10) << 4);
    }
    else if (p[j] >= 'A' && p[j] <= 'F') {
      v = ((p[j] - 'A' + 10) << 4);
    }
    else
      throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);

    if (p[j+1] >= '0' && p[j+1] <= '9') {
      v += (p[j+1] - '0');
    }
    else if (p[j+1] >= 'a' && p[j+1] <= 'f') {
      v += (p[j+1] - 'a' + 10);
    }
    else if (p[j+1] >= 'A' && p[j+1] <= 'F') {
      v += (p[j+1] - 'A' + 10);
    }
    else
      throw CORBA::MARSHAL(0,CORBA::COMPLETED_NO);
    buf.marshalOctet(v);
  }

  buf.rewindInputPtr();
  CORBA::Boolean b = buf.unmarshalBoolean();
  buf.setByteSwapFlag(b);

  ior.type_id = IOP::IOR::unmarshaltype_id(buf);
  ior.profiles <<= buf;
}	// toIOR


static char* toString(IOP::IOR& ior)
{
  cdrMemoryStream buf(CORBA::ULong(0),CORBA::Boolean(1));
  buf.marshalBoolean(omni::myByteOrder);
  buf.marshalRawString(ior.type_id);
  ior.profiles >>= buf;

  // turn the encapsulation into a hex string with "IOR:" prepended
  buf.rewindInputPtr();
  size_t s = buf.bufSize();
  CORBA::Char * data = (CORBA::Char *)buf.bufPtr();

  char *result = new char[4+s*2+1];
  result[4+s*2] = '\0';
  result[0] = 'I';
  result[1] = 'O';
  result[2] = 'R';
  result[3] = ':';
  for (int i=0; i < (int)s; i++) {
    int j = 4 + i*2;
    int v = (data[i] & 0xf0);
    v = v >> 4;
    if (v < 10)
      result[j] = '0' + v;
    else
      result[j] = 'a' + (v - 10);
    v = ((data[i] & 0xf));
    if (v < 10)
      result[j+1] = '0' + v;
    else
      result[j+1] = 'a' + (v - 10);
  }
  return result;
}	// toString


#	if OMNIORB_HEXADECIMAL_VERSION >= 0x04030000
// ============================================== CODE omniORB >= 4.3.0 (CorbaUtil >= 6.1.0) ============================================== 

//
// CP v 6.1.0 (31/03/23). Code issu de omniORB 4.3.0.
//

static char* convertRef (const char* old_ior, const char* hostname)	// Issu de src/appl/utils/convertior/convertior.cc
{
  try
  {
    IOP::IOR ior;

    toIOR(old_ior,ior);

    for (unsigned long count=0; count < ior.profiles.length(); count++) {

      if (ior.profiles[count].tag == IOP::TAG_INTERNET_IOP) {
	IIOP::ProfileBody pBody;
	IIOP::unmarshalProfile(ior.profiles[count],pBody);
	pBody.address.host = hostname;
	IOP::TaggedProfile profile;
	IIOP::encodeProfile(pBody,profile);
	CORBA::ULong max = profile.profile_data.maximum();
	CORBA::ULong len = profile.profile_data.length();
	CORBA::Octet* buf = profile.profile_data.get_buffer(1);
	ior.profiles[count].profile_data.replace(max,len,buf,1);
      }
    }
    CORBA::String_var result = toString(ior);
    return result._retn();

  }
  catch(CORBA::MARSHAL& ex) {
    ConsoleOutput::cerr ( ) << "Invalid stringified IOR supplied." << co_endl;
    ConsoleOutput::cerr ( ) << "(Minor = " << (unsigned long)ex.minor() << ")" << co_endl;
    return 0;
  }
  catch(...) {
    ConsoleOutput::cerr ( ) << "Exception while processing stringified IOR." << co_endl;
    return 0;
  }
}	// convertRef


// ============================================== FIN DE CODE omniORB >= 4.3.0 ============================================== 
#	else	// OMNIORB_HEXADECIMAL_VERSION >= 0x04030000
// ============================================== CODE omniORB < 4.3.0 (CorbaUtil <= 6.0.0) ============================================== 

static char* convertRef(const char* old_ior, const char* hostname)
{
	try
	{
		IOP::IOR ior;

		toIOR(old_ior,ior);

		for (unsigned long count=0; count < ior.profiles.length(); count++)
		{

			if (ior.profiles[count].tag == IOP::TAG_INTERNET_IOP)
			{
				IIOP::ProfileBody pBody;
				IIOP::unmarshalProfile(ior.profiles[count],pBody);
				pBody.address.host = hostname;
				const CORBA::ULong	total	= pBody.components.length ( );
				for (CORBA::ULong index=0; index < total; index++)
				{
					if (IOP::TAG_OMNIORB_UNIX_TRANS == pBody.components [index].tag)
					{
//						char*	content	= IOP::dumpComponent (pBody.components [index]);
						char*	content	= IOP_dumpComponent (pBody.components [index]);	// CP v 5.0.0
						istringstream	stream (content);
						// content est du type :
						// 				TAG_OMNIORB_UNIX_TRANS machine path
						string	tag, host, filename;
						stream >> tag >> host >> filename;
						// Code issu de ior.cc de omniORB 4.0.7 :
						cdrEncapsulationStream s(
											CORBA::ULong(0),CORBA::Boolean(1));
						s.marshalRawString (hostname);	// => new host
						s.marshalRawString (filename.c_str ( ));
//						s.marshalRawString ("/tmp/cu.socket");
						CORBA::Octet* ptr	= 0;
						CORBA::ULong max,len;
						s.getOctetStream (ptr,max,len);

						pBody.components [index].component_data.replace (
																max,len,ptr,1);
//char*	newcontent	= IOP::dumpComponent (pBody.components [index]);
char*	newcontent	= IOP_dumpComponent (pBody.components [index]);	// CP v 5.0.0
cout << "\t\t" << content << " REPLACED WITH " << newcontent << endl;
					}	// if (IOP::TAG_OMNIORB_UNIX_TRANS == ...)
				}	// for (CORBA::ULong index=0; index < total; index++)
				IOP::TaggedProfile profile;
				IIOP::encodeProfile(pBody,profile);
				CORBA::ULong max = profile.profile_data.maximum();
				CORBA::ULong len = profile.profile_data.length();
				CORBA::Octet* buf = profile.profile_data.get_buffer(1);
				ior.profiles[count].profile_data.replace(max,len,buf,1);
			}	// if (ior.profiles[count].tag == IOP::TAG_INTERNET_IOP)
		}	// for (unsigned long count=0; count < ...
		CORBA::String_var result = toString(ior);
		return result._retn();
	}	// try
	catch(CORBA::MARSHAL& ex)
	{
		ConsoleOutput::cerr( ) << "Invalid stringified IOR supplied." <<co_endl;
		ConsoleOutput::cerr( ) << "(Minor = " << (unsigned long)ex.minor() << ")" << co_endl;
		return 0;
	}
	catch (...)
	{
		ConsoleOutput::cerr ( ) << "Exception while processing stringified IOR." << co_endl;
		return 0;
	}

}	// convertRef

// ============================================== FIN DE CODE omniORB < 4.3.0 ============================================== 

#	endif	// OMNIORB_HEXADECIMAL_VERSION >= 0x04030000

#endif	// OMNIORB, CP

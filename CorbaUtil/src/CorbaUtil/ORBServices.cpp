#include "CorbaUtil/ORBServices.h"
#include <TkUtil/NetworkData.h>
#include <TkUtil/UTF8String.h>

#include <sstream>
#include <strings.h>
#include <unistd.h>
#ifdef OMNIORB
#include "convertior.h"
#endif


USING_UTIL
USING_STD
using namespace omni;

static const Charset	charset ("àéèùô");

namespace CorbaFacilities
{

static string _iorHostname ("");
static string _socketPath ("");


string getGivenHostName (const IN_STD string& name)
{
	static const char*	localHost	= "localhost";

	if ((0 != name.length ( )) && (0 == strcasecmp (localHost, name.c_str ( ))))
		return NetworkData::getCurrentHostName ( );

	return name;
}	// getGivenHostName


string getORBName ( )
{
#ifdef OMNIORB
#	if OMNIORB_HEXADECIMAL_VERSION >= 0x04030000
	return OMNI_PACKAGE_NAME;
#	else	// OMNIORB_HEXADECIMAL_VERSION >= 0x04030000
	return PACKAGE_NAME;
#	endif	// OMNIORB_HEXADECIMAL_VERSION >= 0x04030000
#else
#	error "Undefined ORB"
#endif
}	// getORBName


string getORBVersion ( )
{
#ifdef OMNIORB
#	if OMNIORB_HEXADECIMAL_VERSION >= 0x04030000
	return OMNI_PACKAGE_VERSION;
#	else	// OMNIORB_HEXADECIMAL_VERSION >= 0x04030000
	return PACKAGE_VERSION;
#	endif	// OMNIORB_HEXADECIMAL_VERSION >= 0x04030000
#else
#	error "Undefined ORB"
#endif
}	// getORBVersion


static const Version	corbaVersion (CORBA_UTIL_VERSION);
const Version& getVersion ( )
{
	return	corbaVersion;
}	// getVersion


string getNameServiceArgs (const string& host, size_t port)
{
	ostringstream	stream;
	stream << "-ORBInitRef NameService=corbaname::" << host << ":" << port;
	return stream.str ( );
}	// getNameServiceArgs


string corbaExceptionToString (const CORBA::Exception& exc)
{
	ostringstream	stream;
#ifdef OMNIORB
	const CORBA::SystemException*	se	=  dynamic_cast<const CORBA::SystemException*>(&exc);
	if (0 == se)
	{
		stream << "CORBA::Exception (" << exc._name ( ) << ")";
	}
	else	// else if (0 == se)
	{
		stream << "CORBA::SystemException (" << exc._name ( ) << ", " << se->NP_minorString ( ) << ")";
	}	// else if (0 == se)
#else
#	error "Undefined ORB"
#endif

	return stream.str ( );
}	// corbaExceptionToString


void migrateIOR (string& ior, const string& newhost)
{
#ifdef OMNIORB
	string	oldior	= ior;
	ior	= convertior (oldior, newhost);
#else
#	error "Undefined ORB"
#endif
}	// migrateIOR


static CORBA::Boolean encodeIORWithHostName (omniInterceptors::encodeIOR_T::info_T& infos)
{
	infos.iiop.address.host	= _iorHostname.c_str ( );
	converMultipleComponentProfile (infos.iiop.components, _iorHostname, _socketPath);

	return 1;
}	// encodeIORWithHostName


static CORBA::Boolean decodeIORWithRealHostName (omniInterceptors::decodeIOR_T::info_T& infos)
{
	static char		hostname [1025]	= {""};
	static	bool	inited	= false;
	if (false == inited)
	{
		gethostname (hostname, 1024);
		inited	= true;
	}	// if (false == inited)
	omniIOR::IORInfo&	iorInfo	= *(infos.ior.getIORInfo ( ));
	UTF8String	address (charset);
	address << "giop:unix:" << _socketPath;
	iorInfo.addresses ( ).push_back (giopAddress::str2Address (address.ascii ( ).c_str ( )));

	return 1;
}	// decodeIORWithRealHostName


void useUnixDomainProfile (const string& hostname, const string& socketPath)
{
//cout << __FILE__ << ' ' << __LINE__ << "(((((((((((((((((((((((((((((((((((((((((((((" << hostname << ", " << socketPath << "))))))))))))))))))))))))))))" << endl;
#ifdef OMNIORB
	// Source : omniORB4.1.pdf page 92, ior.cc pages 1236 et 1384.
	_iorHostname						= hostname;
	_socketPath							= socketPath;
	omniInterceptors*	interceptors	= omniORB::getInterceptors ( );
//cout << __FILE__ << ' ' << __LINE__ << "(((((((((((((((((((((((((((((((((((((((((((((" << hostname << ", " << socketPath << "))))))))))))))))))))))))))))" << endl;
	// En sortie les IORs doivent contenir le nom de machine du client
	// => celui-ci adressera des références sur la machine qui l'héberge.
	interceptors->encodeIOR.add (encodeIORWithHostName);
	// Pour les références obtenues via des Class_Ptr (et non des IORs) côté
	// client omniORB génère lui-même des IORs sur le client transmises au
	// serveur. Ces IORs contiennent le nom de machine du client, or ici on
	// veut des références locales donc sur la machine serveur  ... :
	interceptors->decodeIOR.add (decodeIORWithRealHostName);
#else
#	error "Undefined ORB"
#endif
}	// useUnixDomainProfile

}	// namespace CorbaFacilities

#include "CorbaUtil/NamingFacilities.h"
#include "CorbaUtil/ORBServices.h"
#include "CorbaUtil/ORBFacilities.h"
#include <TkUtil/UTF8String.h>
#include <TkUtil/InformationLog.h>
#include <TkUtil/ErrorLog.h>
#include <TkUtil/WarningLog.h>
#include <TkUtil/TraceLog.h>

#include <sstream>
#include <iostream>


using namespace std;
using namespace CORBA;
using namespace CosNaming;

USING_UTIL

static const Charset	charset ("àéèùô");


namespace CorbaFacilities
{

class NamingException : public IN_UTIL Exception
{
	public :

	NamingException (const IN_UTIL UTF8String& what,
				const string& objId, const string& objKind
				);

	NamingException (const NamingException&);
	NamingException& operator = (const NamingException&);
	virtual ~NamingException ( )
	{ }
};	// class NamingException


NamingException::NamingException (
		const IN_UTIL UTF8String& what, const string& objId,
		const string& objKind)
	: IN_UTIL Exception ("")
{
	IN_UTIL UTF8String	message (charset);
	message << what << " Object.kind : " << objKind << " Object.id : "
	        << objId << ").";
	setMessage (message);
}	// NamingException::NamingException


NamingException::NamingException (const NamingException& ne)
	: IN_UTIL Exception (ne)
{
}	// NamingException::NamingException (const NamingException&)


NamingException& NamingException::operator = (const NamingException& ne)
{
	IN_UTIL Exception::operator = (ne);
	return *this;
}	// NamingException::operator =


ORB_ptr				NamingFacilities::_orb	= 0;
NamingContext_var	NamingFacilities::_rootContext	= NamingContext::_nil ( );


void NamingFacilities::initialize (ORB_ptr orb, string host, size_t port)
{
	string	hostName	= getGivenHostName (host);
	if (true == is_nil (orb))
		throw IN_UTIL Exception ("NamingFacilities::initialize : ORB nul.");

	_orb	= orb;
	BEGIN_CORBA_TRY_CATCH_BLOCK

	UTF8String	info1Msg (charset), succesMsg (charset);
	info1Msg << "NamingFacilities::initialize. Résolution du service de "
	         << "nommage en cours ...";
	succesMsg << "NamingFacilities::initialize. Résolution du service de "
	          << "nommage effectuée avec succès ";
	ORBFacilities::log (InformationLog (info1Msg));
	Object_var	obj;
	if (0 == hostName.length ( ))
	{
		obj	= _orb->resolve_initial_references ("NameService");
		succesMsg << " au service de nommage référencé à l'initialisation de "
		          << " l'ORB.";
		ORBFacilities::log (InformationLog (succesMsg));
	}
	else
	{
		ostringstream	loc;
		loc << "corbaloc::" << hostName << ":" << port << "/NameService";
		UTF8String	traceMsg (charset);
		traceMsg << "NamingFacilities::initialize. Résolution au service de "
		         << "nommage " << loc.str ( ) << " ...";
		TRACE_LOG_1 (trace, traceMsg)
		ORBFacilities::log (trace);
		obj	= _orb->string_to_object (loc.str ( ).c_str ( ));
		succesMsg << " au service de nommage " << loc.str ( ) << ".";
		ORBFacilities::log (InformationLog (succesMsg));
	}	// else if (0 == host.length ( ))

	_rootContext			= CosNaming::NamingContext::_narrow (obj);
	if (true == is_nil (_rootContext))
		throw IN_UTIL Exception (UTF8String ("NamingFacilities::initialize : impossible d'avoir une référence sur le service de nommage.", charset));

	ORB_CATCH_BLOCK
	COMPLETE_CORBA_TRY_CATCH_BLOCK

	if (true == hasError)
	{
		IN_UTIL UTF8String	error (charset);
		error << "Impossibilité d'avoir une référence sur le service de "
		      << " nommage : " << errorMsg << ".";
		UTF8String	msg	("NamingFacilities::initialize : ", charset);
		msg << error;
		ORBFacilities::log (ErrorLog (msg));
		throw IN_UTIL Exception (error);
	}
}	// NamingFacilities::initialize


void NamingFacilities::finalize ( )
{
	if (false == is_nil (_rootContext))
		_rootContext	= 0;

	_orb	= 0;
	if (0 != ORBFacilities::getLogDispatcher ( ))
		ORBFacilities::log (InformationLog (UTF8String (
			"NamingFacilities::finalize : service de nommage arrêté.",charset)));
}	// NamingFacilities::finalize


bool NamingFacilities::isInited ( )
{
	return true == is_nil (_rootContext) ? false : true;
//	return 0 == _rootContext ? false : true;
}	// NamingFacilities::isInited


void NamingFacilities::bindObjectToName (
						const string& objId, const string& objKind,
						CORBA::Object_ptr objref, bool rebind)
{
	UTF8String	msg1 (charset), msg2 (charset), succes (charset), failure (charset);
	msg1 << "NamingFacilities::bindObjectToName. Tentative de référencement "
	     << "de l'objet kind = " << objKind << " ID = " << objId
	     << " Option rebind = " << (short)rebind
	     << " auprès du service de nommage ...";
	failure << "NamingFacilities::bindObjectToName. Echec du référencement "
	        << "de l'objet kind = " << objKind << " ID = " << objId
	        << " Option rebind = " << (short)rebind
	        << " auprès du service de nommage :";
	succes << "NamingFacilities::bindObjectToName. Référencement "
	       << "de l'objet kind = " << objKind << " ID = " << objId
	       << " Option rebind = " << (short)rebind
	       << " auprès du service de nommage effectué avec succès.";
	TRACE_LOG_1 (trace1, msg1)
	ORBFacilities::log (trace1);
	if (true == is_nil (objref))
	{
		failure << " instance nulle.";
		ORBFacilities::log (ErrorLog (failure));
		throw NamingException (UTF8String ("NamingFacilities::bindObjectToName : instance nulle", charset), objId, objKind);
	}
	if (true == is_nil (_rootContext))
	{
		failure << " service de nommage non initialisé.";
		ORBFacilities::log (ErrorLog (failure));
		throw NamingException (UTF8String ("NamingFacilities::bindObjectToName : service de nommage non initialisé", charset), objId, objKind);
	}

	// Enregistrement du contexte de l'application :
	NamingContext_var	appContext;
	Name				name;
	name.length (1);
	name [0].id		= objId.c_str ( );
	name [0].kind	= objKind.c_str ( );
	try
	{
		_rootContext->bind (name, objref);
		TRACE_LOG_1 (trace2, succes)
		ORBFacilities::log (trace2);
	}
	catch (const NamingContext::AlreadyBound& ex1)
	{
		msg2	= failure;
		msg2 << "Objet déjà référencé.";
		if (true == rebind)
		{
			UTF8String	rbTraceMsg (msg2);
			rbTraceMsg << " Tentative de reconnection en cours ...";
			ORBFacilities::log (WarningLog (rbTraceMsg));
			_rootContext->rebind (name, objref);
			TRACE_LOG_1 (trace2, succes)
			ORBFacilities::log (trace2);
		}
		else
		{
			ORBFacilities::log (ErrorLog (msg2));
			throw NamingException (UTF8String ("NamingFacilities::bindObjectToName : Impossibilité d'enregistrer l'instance auprès du service de nommage : nom déjà pris", charset), objId, objKind);
		}
	}
}	// NamingFacilities::bindObjectToName


Object_ptr NamingFacilities::getObjectReference (
						const string& objId, const string& objKind)
{
	UTF8String	msg1 (charset), failure (charset);
	msg1 << "NamingFacilities::getObjectReference. Tentative d'obtenir une "
	     << "référence sur l'objet kind = " << objKind << " ID = " << objId
	     << " en cours ...";
	failure << "NamingFacilities::getObjectReference. Echec de la tentative "
	        << " d'obtenir une référence sur l'objet kind = " << objKind
	        << " ID = " << objId << " : ";
	TRACE_LOG_1 (trace1, msg1)
	ORBFacilities::log (trace1);
	if (true == is_nil (_rootContext))
	{
		failure << "Service de nommage non initialisé.";
		ORBFacilities::log (ErrorLog (failure));
		throw NamingException (UTF8String ("NamingFacilities::getObjectReference : service de nommage non initialisé", charset), objId, objKind);
	}

	// Obtenir une reference sur l'objet :
	CosNaming::Name	name;
	name.length (1);
	name[0].id		= objId.c_str ( );
	name[0].kind	= objKind.c_str ( );
	
	try
	{
		Object_ptr	obj	= _rootContext->resolve (name);

		UTF8String	succes (charset);
		succes << "NamingFacilities::getObjectReference. Référence "
	           << "sur l'objet kind = " << objKind << " ID = " << objId
		       << " obtenue.";
		TRACE_LOG_1 (trace2, succes)
		ORBFacilities::log (trace2);

		return obj;
	}
	catch (...)
	{
		failure << "Erreur non renseignée.";
		ORBFacilities::log (ErrorLog (failure));
		throw NamingException (UTF8String ("NamingFacilities::getObjectReference : impossible d'obtenir une référence sur l'instance demandée", charset), objId, objKind);
	}
}	// NamingFacilities::getObjectReference

}	// namespace CorbaFacilities


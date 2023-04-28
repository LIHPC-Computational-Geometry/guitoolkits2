#include "OceaneServer_impl.h"
#include "CorbaUtil/ORBServices.h"
#include "CorbaUtil/ORBFacilities.h"
#include <TkUtil/File.h>
#include <TkUtil/InformationLog.h>
#include <TkUtil/WarningLog.h>
#include <TkUtil/ErrorLog.h>
#include <TkUtil/NetworkData.h>
#include <TkUtil/TraceLog.h>
#include <TkUtil/UTF8String.h>

#include <iostream>

using namespace std;
using namespace TkUtil;
using namespace CorbaFacilities;

static const Charset	charset ("àéèùô");


namespace Oceane
{

OceaneServer_impl::OceaneServer_impl ( )
	: POA_Oceane::OceaneServerICorba ( ), PortableServer::RefCountServantBase ( ),
	  _models ( )
{
	log (InformationLog (UTF8String ("OceaneServer_impl::OceaneServer_impl : serveur instancié.", charset)));
}	// OceaneServer_impl::OceaneServer_impl


OceaneServer_impl::OceaneServer_impl (const OceaneServer_impl&)
	: POA_Oceane::OceaneServerICorba ( ), PortableServer::RefCountServantBase ( ),
	  _models ( )
{
}	// OceaneServer_impl::OceaneServer_impl


OceaneServer_impl& OceaneServer_impl::operator = (const OceaneServer_impl&)
{
	return *this;
}	// OceaneServer_impl::operator =


OceaneServer_impl::~OceaneServer_impl ( )
{
	log (InformationLog (UTF8String (
			"OceaneServer_impl::~OceaneServer_impl : Arrêt du serveur Océane en cours ...", charset)));
	UTF8String	closureMsg ("Fermeture des modèles ...", charset);
	TRACE_LOG_1 (closureLog, closureMsg)
	log (closureLog);
	for (vector<Model_impl*>::iterator it = _models.begin ( );
	     _models.end ( ) != it; it++)
		delete *it;
	_models.clear ( );
	log (InformationLog (UTF8String (
		"OceaneServer_impl::~OceaneServer_impl : serveur arrêté.", charset)));
}	// OceaneServer_impl::~OceaneServer_impl


char* OceaneServer_impl::getOceaneServerName ( )
{
	UTF8String name (charset);
	name << "OceaneServer_impl@" << NetworkData::getCurrentHostName ( );

	return CORBA::string_dup (name.iso ( ).c_str ( ));
}	// OceaneServer_impl::getOceaneServerName


CORBA::ULong OceaneServer_impl::getId ( )
{
	return 123;
}	// OceaneServer_impl::getId


ModelICorba_ptr OceaneServer_impl::loadModel (const char* fileName)
{
	UTF8String	msg1 (charset);
	msg1 << "OceaneServer_impl::loadModel. Fichier : " << fileName;
	log (InformationLog (msg1));
	File		file (fileName);
	UTF8String	errorMsg (charset);
	errorMsg << "OceaneServer_impl::loadModel : unable to load " << fileName;
	if (false == file.exists ( ))
	{
		OceaneExceptionICorba	exc;
		errorMsg << " : file does not exists.";
		log (ErrorLog (errorMsg));
		exc.what	= errorMsg.iso ( ).c_str ( );
		throw exc;
	}	// if (false == file.exists ( ))
	if (false == file.isReadable ( ))
	{
		OceaneExceptionICorba	exc;
		errorMsg << " : file is not readable.";
		log (ErrorLog (errorMsg));
		exc.what	= errorMsg.iso ( ).c_str ( );
		throw exc;
	}	// if (false == file.isReadable ( ))
	Model_impl*	model	= getModel (fileName);

	log (InformationLog (UTF8String (
				"OceaneServer_impl::loadModel : modèle chargé.", charset)));
	return ModelICorba::_duplicate (((POA_Oceane::ModelICorba*)model)->_this ( ));
}	// OceaneServer_impl::loadModel


void OceaneServer_impl::closeModel (ModelICorba_ptr m)
{
	log (InformationLog (UTF8String ("OceaneServer_impl::closeModel.", charset)));
	PortableServer::POA_var	poa	= _default_POA ( );
	Model_impl*	model	= 0;
	referenceToImpl (poa, m, model);
	if (0 != model)
	{
		for (vector<Model_impl*>::iterator it = _models.begin ( );
		     _models.end ( ) != it; it++)
		{
			if (*it == model)
			{
				_models.erase (it);
				PortableServer::ObjectId_var	id	= poa->servant_to_id(model);
				poa->deactivate_object (id);
				delete model;	model	= 0;
				break;
			}
		}	// for (vector<Model_impl*>::iterator it = _models.begin ( ); ...
	}	// if (0 != model)
	else
	{
		OceaneExceptionICorba	exc;
		log (WarningLog (UTF8String (
			"OceaneServer_impl::closeModel : modèle non trouvé.", charset)));
		exc.what	= "OceaneServer_impl::closeModel. Modèle non trouvé.";
		throw exc;
	}
	log (InformationLog (UTF8String (
			"OceaneServer_impl::closeModel : modèle fermé.", charset)));
}	// OceaneServer_impl::closeModel


Model_impl* OceaneServer_impl::getModel (const string& fileName)
{
	UTF8String	msg1 (charset), msg2 (charset);
	msg1 << "OceaneServer_impl::getModel : Modèle : " << fileName << " ...";
	log (InformationLog (msg1));
	for (vector<Model_impl*>::iterator it = _models.begin ( );
	     _models.end ( ) != it; it++)
		if (fileName == (*it)->getFileName ( ))
			return *it;

	Model_impl*	model	= new Model_impl (fileName);
	_models.push_back (model);

	msg2 << "OceaneServer_impl::getModel : Modèle : " << fileName <<" trouvé.";
	log (InformationLog (msg2));
	return model;
}	// OceaneServer_impl::getModel


void OceaneServer_impl::shutdown ( )
{
	log (InformationLog (UTF8String (
		"OceaneServer_impl::shutdown : Fermeture des modèles ...", charset)));
	for (vector<Model_impl*>::iterator it = _models.begin ( );
	     _models.end ( ) != it; it++)
		delete *it;
	_models.clear ( );
	log (InformationLog (UTF8String (
					"OceaneServer_impl::shutdown : modèles fermés.", charset)));

	PortableServer::POA_var	poa	= _default_POA ( );
	PortableServer::ObjectId_var	id	= poa->servant_to_id (this);
	poa->deactivate_object (id);

	log (InformationLog (UTF8String (
					"OceaneServer_impl::shutdown : Arrêt de l'ORB ...", charset)));
	ORBFacilities::getORB ( )->shutdown (false);
	log (InformationLog (UTF8String (
					"OceaneServer_impl::shutdown : ORB arrêté.", charset)));
	ORBFacilities::getORB ( )->destroy ( );	// => delete this
	log (InformationLog (UTF8String (
					"OceaneServer_impl::shutdown : ORB détruit.", charset)));
}	// OceaneServer_impl::shutdown


void OceaneServer_impl::log (const IN_UTIL Log& log)
{
	if (0 != ORBFacilities::getLogDispatcher ( ))
		(*ORBFacilities::getLogDispatcher ( )) << log;
}	// OceaneServer_impl::log


}	// namespace Oceane


#include "PrefsXerces/XercesUtils.h"
#include "PrefsXerces/XMLLoader.h"
#include "PrefsXerces/XMLPrefsSAXErrorHandler.h"
#include "PrefsXerces/XMLPrefsDOMErrorHandler.h"
#include "PrefsXerces/XMLSchemaPrefsHelper.h"
#include "PrefsCore/BoolNamedValue.h"
#include "PrefsCore/ColorNamedValue.h"
#include "PrefsCore/LongNamedValue.h"
#include "PrefsCore/UnsignedLongNamedValue.h"
#include "PrefsCore/DoubleNamedValue.h"
#include "PrefsCore/DoubleTripletNamedValue.h"
#include "PrefsCore/StringNamedValue.h"
#include "PrefsXerces/XercesSessionManager.h"

#include <TkUtil/File.h>
#include <TkUtil/Exception.h>
#include <TkUtil/InternalError.h>
#include <TkUtil/Version.h>
#include <TkUtil/Locale.h>
#include <TkUtil/UTF8String.h>

#include <assert.h>
#include <strings.h>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <memory>

#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/impl/DOMDocumentImpl.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/XMLValidator.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>


USING_STD
USING_UTIL
XERCES_CPP_NAMESPACE_USE


BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");


#define CHECK_PTR(ptr)                                                       \
	if (0 == ptr)                                                            \
	{                                                                        \
		UTF8String	msg (charset);                                           \
		msg << "Erreur, pointeur nul : " << #ptr << ".";                     \
		throw Exception (msg);                                               \
	}

#define CHECK_unique_ptr(autoptr)                                              \
	if (0 == autoptr.get ( ))                                                \
	{                                                                        \
		UTF8String	msg (charset);                                           \
		msg << "Erreur, pointeur nul : " << #autoptr << ".";                 \
		throw Exception (msg);                                               \
	}

#define CHECK_ISTREAM(stream, value, name, type)                             \
	if ((true == stream.fail ( )) || (true == stream.bad ( )) ||             \
	    (false == stream.eof ( )))                                           \
	{                                                                        \
		UTF8String	msg (charset);                                           \
		msg << "Erreur, impossible de convertir la chaine de caractères "    \
		    << value << " en " << type                                       \
		    << ". Cela concerne la valeur nommée " << name;                  \
		throw Exception (msg);                                               \
	}




// ===========================================================================
//                   LA CLASSE XMLLoader::XMLDocInformations
// ===========================================================================


const string XMLLoader::XMLDocInformations::noNamespaceSchemaLocationTag ("xsi:noNamespaceSchemaLocation");
const string XMLLoader::XMLDocInformations::xmlnsXsiTag ("xmlns:xsi");


XMLLoader::XMLDocInformations::XMLDocInformations ( )
	: Element::OriginInfos ("XML Document"), _schemaURL ( ), _namespace ( )
{
}	// XMLDocInformations::XMLDocInformations


XMLLoader::XMLDocInformations::XMLDocInformations (const XMLLoader::XMLDocInformations& toCopy)
	: Element::OriginInfos ("XML Document"), _schemaURL ( ), _namespace ( )
{
	copy (toCopy);
}	// XMLDocInformations::XMLDocInformations


XMLLoader::XMLDocInformations& XMLLoader::XMLDocInformations::operator = (const XMLLoader::XMLDocInformations& toCopy)
{
	if (&toCopy != this)
		copy (toCopy);

	return *this;
}	// XMLDocInformations::operator =


XMLLoader::XMLDocInformations::~XMLDocInformations ( )
{
}	// XMLDocInformations::~XMLDocInformations


bool XMLLoader::XMLDocInformations::hasSchema ( ) const
{
	return 0 == _schemaURL.length ( ) ? false : true;
}	// XMLDocInformations::hasSchema


string XMLLoader::XMLDocInformations::getSchemaURL ( ) const
{
	return _schemaURL;
}	// XMLDocInformations::getSchemaURL


void XMLLoader::XMLDocInformations::setSchemaURL (const string& url)
{
	_schemaURL	= url;
}	// XMLDocInformations::setSchemaURL


bool XMLLoader::XMLDocInformations::hasNamespace ( ) const
{
	return 0 == _namespace.get ( ) ? false : true;
}	// XMLDocInformations::hasNamespace


const XMLLoader::XMLDocInformations::XMLNamespace& XMLLoader::XMLDocInformations::getNamespace ( ) const
{
	if (0 == _namespace.get ( ))
		throw Exception ("XMLDocInformations::getNamespace : absence d'espace de nommage.");
	return *(_namespace.get ( ));
}	// XMLDocInformations::getNamespace


void XMLLoader::XMLDocInformations::setNamespace (XMLLoader::XMLDocInformations::XMLNamespace* ns)
{
	_namespace.reset (ns);
}	// XMLDocInformations::setNamespace


Element::OriginInfos* XMLLoader::XMLDocInformations::clone ( ) const
{
	return new XMLLoader::XMLDocInformations (*this);
}	// XMLDocInformations::clone


void XMLLoader::XMLDocInformations::copy (const XMLLoader::XMLDocInformations& toCopy)
{
	if (&toCopy != this)
	{
		Element::OriginInfos::copy (toCopy);
		setSchemaURL (toCopy.getSchemaURL ( ));
		XMLLoader::XMLDocInformations::XMLNamespace*	ns	= false == toCopy.hasNamespace ( ) ? 0 : new XMLLoader::XMLDocInformations::XMLNamespace (toCopy.getNamespace ( ));
		setNamespace (ns);
	}	// XMLDocInformations::copy
}	// XMLDocInformations::copy


// ===========================================================================
//                            LA CLASSE XMLLoader
// ===========================================================================


const IN_STD	string XMLLoader::typeAttrName ("type");
const IN_STD	string XMLLoader::nameAttrName ("name");
const IN_STD	string XMLLoader::overloadableAttrName ("overloadable");
const IN_STD	string XMLLoader::safeguardableAttrName ("safeguardable");
const IN_STD	string XMLLoader::valueElementName ("value");
const IN_UTIL	Version XMLLoader::version ("0.3.0");
const IN_STD	string XMLLoader::versionTag ("XMLLoader_version");


Section* XMLLoader::load (const string& fileName, Section* initialSection)
{
	UTF8String		errorMessage (charset);
	errorMessage << "Lecture de la configuration depuis le fichier "  << fileName << " impossible :\n";
	Section*		mainSection = initialSection;

	// Xerces n'est pas tres robuste lorsque les droits sur les fichiers ne sont pas bons :
	File	file (fileName);
	if ((false == file.exists ( )) || (false == file.isReadable ( )))
	{
		errorMessage << "Fichier inexistant ou absence de droits en lecture sur ce fichier.";
		throw Exception (errorMessage);
	}	// if ((false == file.exists ( )) || (false == file.isReadable ( )))

	XercesSessionManager	sessionManager;
	try
	{
		unique_ptr<XercesDOMParser>		parser (new XercesDOMParser ( ));
		CHECK_unique_ptr (parser)
		unique_ptr<XMLPrefsSAXErrorHandler>	errorHandler (new XMLPrefsSAXErrorHandler ( ));
		CHECK_unique_ptr (errorHandler)
		parser->setErrorHandler(errorHandler.get ( ));
		// Parsing en 2 temps : premiere passe pour verifier le document, sans schéma.
		parser->setDoNamespaces (true);
		parser->setDoSchema (true);
		parser->parse (fileName.c_str ( ));
		if ((true == errorHandler->hasErrors ( )) || (true == errorHandler->hasFatalErrors ( )))
		{
			UTF8String	message ("Erreurs XML dans le fichier ", charset);
			message	+= fileName + " :\n";
			message	+= errorHandler->wholeMessages ( );
			throw Exception (message);
		}	// if ((true == errorHandler->hasError ( )) || ...

		const bool	useSchema	= parser->getValidator ( ).handlesSchema ( );
		if (true == useSchema)
		{
			parser->reset ( );
			errorHandler->resetErrors ( );
			parser->setValidationSchemaFullChecking (true);
			parser->setValidationScheme (AbstractDOMParser::Val_Always);
//			parser->setValidationConstraintFatal (true);
			parser->parse (fileName.c_str ( ));
			if ((true == errorHandler->hasErrors ( )) || (true == errorHandler->hasFatalErrors ( )))
			{
				UTF8String	message ("Erreurs XML Schema dans le fichier ", charset);
				message	+= fileName + " :\n";
				message	+= errorHandler->wholeMessages ( );
				throw Exception (message);
			}	// if ((true == errorHandler->hasError ( )) || ...
		}	// if (true == useSchema)

		unique_ptr<DOMDocument>	doc	(parser->adoptDocument ( ));
		// Memoriser les infos liees a l'eventuelle utilisation d'un schema.
		unique_ptr<XMLLoader::XMLDocInformations>		docInfos;
		DOMAttr*	schemaAttr	= doc->getDocumentElement ( )->getAttributeNode (X (XMLDocInformations::noNamespaceSchemaLocationTag.c_str( )));
		DOMAttr*	nsAttr		= doc->getDocumentElement ( )->getAttributeNode (X (XMLDocInformations::xmlnsXsiTag.c_str ( )));
		const string	schemaURL	= 0 == schemaAttr ? "" : StrX (schemaAttr->getValue ( )).localForm( );
		const string	ns			= 0 == nsAttr ? "" : StrX (nsAttr->getValue ( )).localForm( );

		if ((false == useSchema) && (0 != schemaURL.length ( )))
		{	// Xerces dit qu'il n'y a pas de schéma associé et pourtant les attributs associés au schéma sont là => le document "schéma" n'a pas été trouvé.
			UTF8String	message (charset);
			message << "Schéma " << schemaURL  << " introuvable pour le fichier XML " << fileName << ".";
			throw Exception (message);
		}	// if ((false == useSchema) && (0 != schemaURL.length ( )))

		docInfos.reset (new XMLLoader::XMLDocInformations ( ));
		CHECK_PTR (docInfos.get ( ))
		docInfos->setSchemaURL (schemaURL);
		if ((0 != nsAttr) && (0 != ns.length ( )))
		{
			string	prefix;
			if (0 == XMLDocInformations::xmlnsXsiTag.find ("xmlns:"))
				prefix	= XMLDocInformations::xmlnsXsiTag.substr (6);
			else
				throw Exception ("XMLLoader::load. Balise de description du schéma invalide.");
			XMLDocInformations::XMLNamespace*	nsDef	= new XMLDocInformations::XMLNamespace (prefix, ns);
			CHECK_PTR (nsDef)
			docInfos->setNamespace (nsDef);
		}	// if ((0 != nsAttr) && (0 != ns.length ( ))

		delete parser.release ( );
		CHECK_unique_ptr (doc)
		CHECK_PTR (doc->getDocumentElement ( ))
 		UTF8String	mainSectionName (StrX (doc->getDocumentElement ( )->getNodeName ( )).localForm ( ));	// CP v 5.5.4
//			(const char*)doc->getDocumentElement ( )->getNodeName ( ), Charset::UTF_16);

		// On teste la version afin d'utiliser éventuellement un lecteur antérieur :
		DOMAttr*	versionAttr	=  doc->getDocumentElement ( )->getAttributeNode (X (versionTag.c_str ( )));
		CHECK_PTR (versionAttr)
		const Version	docVersion (StrX (versionAttr->getValue ( )).localForm( ));
		if ((0 != mainSection) && (mainSection->getName ( ) != mainSectionName))
		{
			UTF8String	msg (charset);
			msg << " la configuration à modifier ("  << initialSection->getName ( )  << ") et celle lue dans le fichier " << fileName 
			    << "(" << mainSectionName << ") n'ont pas le même nom.";
			throw Exception (msg);
		}	// if ((0 != mainSection) && ...
		if (0 == mainSection)
			mainSection	= new Section (mainSectionName);
		CHECK_PTR (mainSection)
		mainSection->setOriginInfos (docInfos.release ( ));

		// On charge le document :
		loadSection (*mainSection, *(doc->getDocumentElement ( )));
	}
	catch (const XMLException& xmlExc)
	{
		if (mainSection != initialSection) delete mainSection;
		errorMessage << StrX (xmlExc.getMessage ( )).localForm ( );
		throw Exception	(errorMessage);
	}	// catch (const XMLException& xmlExc)
	catch (const Exception exc)
	{
		if (mainSection != initialSection) delete mainSection;
		errorMessage << exc.getFullMessage ( );
		throw Exception (errorMessage);
	}
	catch (const exception& stdExc)
	{
		if (mainSection != initialSection) delete mainSection;
		errorMessage << stdExc.what ( );
		throw Exception (errorMessage);
	}
	catch (...)
	{
		if (mainSection != initialSection) delete mainSection;
		errorMessage << "Erreur non documentée.";
		throw Exception (errorMessage);
	}

	return mainSection;
}	// XMLLoader::load


void XMLLoader::save (const Section& section, const string& fileName, ENCODING encoding)
{
	UTF8String	errorMessage (charset);
	errorMessage << "Enregistrement de la configuration " << section.getName ( ) << " dans le fichier " << fileName << " impossible :\n";

	// Xerces n'est pas tres robuste lorsque les droits sur les fichiers ne sont pas bons :
	File	file (fileName);
	if (false == file.isWritable ( ))
	{
		errorMessage << "Absence de droits en écriture sur ce fichier.";
		throw Exception (errorMessage);
	}	// if (false == file.isWritable ( ))

	XercesSessionManager	sessionManager;
	try
	{
		if (false == section.isSafeguardable ( ))
			throw Exception (UTF8String ("Section non enregistrable", charset));	// v 5.7.0
			
		DOMImplementation*	domImpl	= DOMImplementationRegistry::getDOMImplementation (X ("Core"));
		CHECK_PTR (domImpl)
		unique_ptr<DOMDocument>	doc	(domImpl->createDocument (
						0,					// root element namespace
						unicodeToXMLString (section.getName ( ), encoding),
						0));				// DTD
		CHECK_unique_ptr (doc)
		DOMElement*	rootElem	= doc->getDocumentElement ( );
		CHECK_PTR (rootElem)
		DOMAttr*	attr		= 0;
		doc->setXmlStandalone (true);

		// Le document fait il reference a un schema ?
		const XMLLoader::XMLDocInformations*	docInfos	= dynamic_cast<const XMLLoader::XMLDocInformations*>(section.getOriginInfos ( ));
		if (0 != docInfos)
		{
			if (true == docInfos->hasSchema ( ))
			{
				// Le schéma utilisé :
				doc->setXmlStandalone (false);
				attr	= doc->createAttribute (X (XMLDocInformations::noNamespaceSchemaLocationTag.c_str( )));
				CHECK_PTR (attr)
				attr->setValue (X (docInfos->getSchemaURL ( ).c_str ( )));
				rootElem->setAttributeNode (attr);

				// La description du schéma :
				if (true == docInfos->hasNamespace ( ))
				{
					const XMLDocInformations::XMLNamespace&	ns	= docInfos->getNamespace ( );
					attr	= doc->createAttribute (X (XMLDocInformations::xmlnsXsiTag.c_str ( )));
					CHECK_PTR (attr)
					attr->setValue (X (ns.getURL ( ).c_str ( )));
					rootElem->setAttributeNode (attr);
				}	// if (true == docInfos->hasNamespace ( ))
			}	// if (true == docInfos->hasSchema ( ))
		}	// if (0 != docInfos)
		addComments (section, *rootElem, *doc, encoding);
		addChildren (section, *rootElem, *doc, encoding);

		attr	= doc->createAttribute (X (XMLLoader::versionTag.c_str ( )));
		CHECK_PTR (attr)
		attr->setValue (X (version.getVersion ( ).c_str ( )));
		rootElem->setAttributeNode (attr);

		unique_ptr<XMLPrefsDOMErrorHandler>	errorHandler(new XMLPrefsDOMErrorHandler( ));

		DOMImplementation*	impl	= DOMImplementationRegistry::getDOMImplementation (0);
		unique_ptr<DOMLSSerializer>	serializer (((DOMImplementationLS*)impl)->createLSSerializer ( ));
		unique_ptr<DOMLSOutput>		outputDesc (((DOMImplementationLS*)impl)->createLSOutput ( ));
		unique_ptr<XMLFormatTarget>	formatTarget (new LocalFileFormatTarget (X (fileName.c_str ( ))));
		CHECK_unique_ptr (formatTarget)
		outputDesc->setByteStream (formatTarget.get ( ));

		serializer->getDomConfig ( )->setParameter (XMLUni::fgDOMWRTFormatPrettyPrint, true);
		serializer->getDomConfig ( )->setParameter (X ("error-handler"), errorHandler.get ( ));
		CHECK_PTR ((DOMDocumentImpl*)doc.get ( ))
		switch (encoding)
		{
			case UTF_8	: ((DOMDocumentImpl*)doc.get ( ))->setXmlEncoding (X ("UTF-8"));
				break;
			case ISO_8859	:
				try
				{
					string	charset	= Locale::getNormalizedIsoString (Locale::EXE);
					((DOMDocumentImpl*)doc.get ( ))->setXmlEncoding (X (charset.c_str ( )));
				}
				catch (...)
				{
					((DOMDocumentImpl*)doc.get ( ))->setXmlEncoding (X ("ISO-8859-1"));
				}
				break;
			case UTF_16	: ((DOMDocumentImpl*)doc.get ( ))->setXmlEncoding (X ("UTF-16"));
				break;
			default		:
				errorMessage << "Type d'encodage (" << (unsigned long)encoding << ") inconnu.";
				throw Exception (errorMessage);
		}	// switch (encoding)
 
		if (false == serializer->write (doc.get ( ), outputDesc.get ( )))
		{
			UTF8String	msg (charset);
			msg  << "ATTENTION : des erreurs se sont produites lors de la sauvegarde de la configuration "
			     << section.getName ( ) << " dans " << fileName << " :\n" << errorHandler->wholeMessages ( );
			throw Exception (msg);
		}	// if (false == serializer->write (doc.get ( ), outputDesc.get ( )))
	}
	catch (const DOMException& domExc)
	{
		errorMessage << StrX (domExc.msg).localForm ( );
		throw Exception	(errorMessage);
	}	// catch (const XMLException& xmlExc)
	catch (const XMLException& xmlExc)
	{
		errorMessage << StrX (xmlExc.getMessage ( )).localForm ( );
		throw Exception	(errorMessage);
	}	// catch (const XMLException& xmlExc)
	catch (const Exception exc)
	{
		errorMessage << exc.getFullMessage ( );
		throw Exception (errorMessage);
	}
	catch (const exception& stdExc)
	{
		errorMessage << stdExc.what ( );
		throw Exception (errorMessage);
	}
	catch (...)
	{
		errorMessage << "Erreur non documentée.";
		throw Exception (errorMessage);
	}
}	// XMLLoader::save


const XMLCh* XMLLoader::unicodeToXMLString (const UTF8String& str, ENCODING encoding)
{
	static XMLCh*	retVal	= 0;
	if (0 != retVal)
		XMLString::release (&retVal);

	switch (encoding)
	{
		case UTF_8		: 
			retVal	= XMLString::replicate (X (str.utf8 ( ).c_str ( )));	// v 5.6.0
			break;
		case ISO_8859	: 
		case UTF_16		: 
		{
			static bool	firstCall	= true;
//			retVal	= XMLString::replicate (str.unicode ( ));
			const UTF16String	utf16 (str.utf16 ( ));
			// ATTENTION : au premier appel UTF-8 -> UTF-16 la classe UTF8String semble mettre en 1er octet l'endian (BE/LE) utilisé (en fait
			// c'est iconv qui ferait ça).
			if (true == firstCall)
			{
				firstCall	= false;
				retVal		= XMLString::replicate ((const XMLCh*)utf16.utf16 ( ) + 1);
			}
			else
				retVal	= XMLString::replicate ((const XMLCh*)utf16.utf16 ( ));
			break;
		}
		default			: 
		throw Exception ("XMLLoader::unicodeToXMLString : encodage inconnu.");
	}	// switch (encoding)

	return retVal;
}	// XMLLoader::unicodeToXMLString


bool XMLLoader::isSimpleType (const Element& element)
{
	const DoubleTripletNamedValue*	dtValue		= dynamic_cast<const DoubleTripletNamedValue*>(&element);
	const ColorNamedValue*			colorValue	= dynamic_cast<const ColorNamedValue*>(&element);

	if ((0 == dtValue) && (0 == colorValue))
		return true;

	return false;
}	// XMLLoader::isSimpleType


bool XMLLoader::typesAreCompatible (const string& type1, const string& type2)
{
	if (type1 == type2)
		return true;

	string	t1	= type1, t2	= type2;
	if (t1 == "boolean")
		t1	= BoolNamedValue::typeName;
	if (t2 == "boolean")
		t2	= BoolNamedValue::typeName;
	if (t1 == "unsignedLong")
		t1	= UnsignedLongNamedValue::typeName;
	if (t2 == "unsignedLong")
		t2	= UnsignedLongNamedValue::typeName;

	return t1 == t2;
}	// XMLLoader::typesAreCompatible


UTF8String XMLLoader::getElementName (const DOMElement& element)
{
	DOMAttr*	nameAttr	= element.getAttributeNode (X (XMLLoader::nameAttrName.c_str ( )));
	if (0 == nameAttr)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "L'élément n'a pas d'attribut définissant son nom.";
		throw Exception (errorMsg);
	}	// if (0 == nameAttr)

//	return UTF8String ((const char*)nameAttr->getValue ( ), Charset::UTF_16);
	return UTF8String (StrX (nameAttr->getValue ( )).localForm ( ));	// CP v 5.5.4
}	// XMLLoader::getElementName


string XMLLoader::getElementType (const DOMElement& element)
{
	DOMAttr*	typeAttr	= element.getAttributeNode (X (XMLLoader::typeAttrName.c_str ( )));
	if (0 == typeAttr)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "L'élément n'a pas d'attribut définissant son type.";
		throw Exception (errorMsg);
	}	// if (0 == typeAttr)

	return StrX (typeAttr->getValue ( )).localForm ( );
}	// XMLLoader::getElementType


bool XMLLoader::isElementOverloadable (const DOMElement& element)	// v 5.7.0
{
	DOMAttr*	typeAttr	= element.getAttributeNode (X (XMLLoader::overloadableAttrName.c_str ( )));
	if (0 == typeAttr)
		return true;

	return 0 == strcasecmp (StrX (typeAttr->getValue ( )).localForm ( ), "true") ? true : false;
}	// XMLLoader::isElementOverloadable


bool XMLLoader::isElementSafeguardable (const DOMElement& element)	// v 5.7.0
{
	DOMAttr*	typeAttr	= element.getAttributeNode (X (XMLLoader::safeguardableAttrName.c_str ( )));
	if (0 == typeAttr)
		return true;

	return 0 == strcasecmp (StrX (typeAttr->getValue ( )).localForm ( ), "true") ? true : false;
}	// XMLLoader::isElementSafeguardable


void XMLLoader::loadSection (Section& section, const DOMElement& node)
{
	updateComments (section, node);

	// Lecture des sous-sections :
	DOMNodeList*	children	= node.getChildNodes ( );
	for (XMLSize_t c = 0; c < children->getLength ( ); c++)
	{
		DOMNode*	currentNode	= children->item (c);
		CHECK_PTR (currentNode)

		// On ne s'interesse qu'aux "elements". Les DOMTextNode peuvent introduire des sections. C'est par exemple le cas des tabulations
		// en debut de section.
		if (DOMNode::ELEMENT_NODE != currentNode->getNodeType ( ))
			continue;
		DOMElement*	element	= dynamic_cast<DOMElement*>(currentNode);
		if (0 == element)
			continue;

		UTF8String	name (charset), type (charset), appType (charset);
		bool		overloadable	= isElementOverloadable (*element), safeguardable	= isElementSafeguardable (*element);
		try
		{
			appType	= 	StrX (currentNode->getNodeName ( )).localForm ( );		// CP v 5.5.4
//			appType	= UTF8String ((const char*)currentNode->getNodeName ( ), Charset::UTF_16);

			// Cas particulier : element "annotation" => c'est un commentaire :
			if (appType == UTF8String ("annotation", charset))
				continue;

			name	= getElementName (*element);
			type	= getElementType (*element);
		}
		catch (const Exception& e)
		{
			UTF8String	mess (charset);
			mess << "Erreur dans XMLLoader::loadSection en " << __FILE__ << ' ' << (unsigned long)__LINE__ << " : \n" << e.getFullMessage ( );
			ConsoleOutput::cout ( ) << mess << co_endl;
			continue;
		}
		catch (...)
		{
			UTF8String	mess (charset);
			mess << "Erreur non renseignée dans XMLLoader::loadSection en " << __FILE__ << ' ' << (unsigned long)__LINE__;
			ConsoleOutput::cout ( ) << mess << co_endl;
			continue;
		}

		if (type == "container")
		{
			if (true == section.hasSection (name))
			{
				Section&	s	= section.getSection (name);	// v 5.7.0
				if (true == s.isOverloadable ( ))				// v 5.7.0
					loadSection (s, *element);
			}
			else
			{
				Section*	s	= new Section (name, "", true, safeguardable);	// v 5.7.0
				CHECK_PTR (s)
				s->setAppType (appType);
				section.addSection (s);
				loadSection (*s, *element);
				s->setOverloadable (overloadable);	// v 5.7.0
			}	// else if (true == section.hasSection (name))
		}	// if (type == section.getType ( ))
		else
			updateNamedValue (section, name, *element, type, appType, overloadable, safeguardable);
	}	// for (XMLSize_t c = 0; c < children->getLength ( ); c++)
}	// XMLLoader::loadSection


void XMLLoader::updateNamedValue (Section& section, const UTF8String& name, const DOMElement& node, const string& type, const UTF8String& appType, bool overloadable, bool safeguardable)
{
	NamedValue*	nv	= 0;	// v 5.7.0
	if (true == section.hasNamedValue (name))
	{
		if ((0 != type.length ( )) && (false == typesAreCompatible (section.getNamedValue (name).getType ( ), type)))
		{
			UTF8String	mess (charset);
			mess << "ATTENTION, la valeur " << name  << " lue en configuration n'est pas mise à jour : types incompatibles (" 
			     << section.getNamedValue (name).getType ( ) << "/" << type << ").";
			ConsoleOutput::cout ( ) << mess << co_endl;
			return;
		}	// if ((0 != type.length ( )) && ...

		if (false == section.getNamedValue (name).isOverloadable ( ))
		{	// v 5.7.0
			UTF8String	mess (charset);
			mess << "ATTENTION, la valeur " << name  << " lue en configuration n'est pas mise à jour : valeur non surchargeable.";
			ConsoleOutput::cout ( ) << mess << co_endl;
			return;			
		}	// if (false == section.getNamedValue (name).isOverloadable ( ))
	}	// if (true == section.hasNamedValue (name))
	else
	{
		nv	= createNamedValue (name, type, true, safeguardable);
		CHECK_PTR (nv)
		nv->setAppType (appType);
		section.addNamedValue (nv);
	}	// else if (true == section.hasNamedValue (name))

	if (true == isSimpleType (section.getNamedValue (name)))
		updateSimpleValue (section.getNamedValue (name), node);
	else
		updateComplexValue (section.getNamedValue (name), node);
	if (0 != nv)
		nv->setOverloadable (overloadable);	// v 5.7.0
}	// XMLLoader::updateNamedValue


NamedValue* XMLLoader::createNamedValue (const UTF8String& name, const string& type, bool overloadable, bool safeguardable)
{
	if ((type == "string") || (type == StringNamedValue::typeName))
		return new StringNamedValue (name, "", "", overloadable, safeguardable);

	if ((type == "boolean") || (type == BoolNamedValue::typeName))
		return new BoolNamedValue (name, false, "", overloadable, safeguardable);

	if ((type == "color") || (type == ColorNamedValue::typeName))
		return new ColorNamedValue (name, 0., 0., 0., "", overloadable, safeguardable);

	if ((type == "long") || (type == LongNamedValue::typeName))
		return new LongNamedValue (name, 0, "", overloadable, safeguardable);

	if ((type == "unsignedLong") || (type == UnsignedLongNamedValue::typeName))
		return new UnsignedLongNamedValue (name, 0, "", overloadable, safeguardable);

	if ((type == "double") || (type == DoubleNamedValue::typeName))
		return new DoubleNamedValue (name, 0., "", overloadable, safeguardable);

	if ((type == "doubleTriplet") || (type == DoubleTripletNamedValue::typeName))
		return new DoubleTripletNamedValue (name, 0., 0., 0., "", overloadable, safeguardable);

	UTF8String	errorMsg (charset);
	errorMsg << "Impossibilité de créer la valeur nommée " << name << " : type " << type << " inconnu.";
	throw Exception (errorMsg);
}	// XMLLoader::createNamedValue


void XMLLoader::updateSimpleValue (NamedValue& namedValue, const DOMElement& node)
{
	updateComments (namedValue, node);
	const UTF8String	value	= getStrValue (node);

	if (StringNamedValue::typeName == namedValue.getType ( ))
	{
		StringNamedValue*	strValue	= dynamic_cast<StringNamedValue*>(&namedValue);
		CHECK_PTR (strValue)
		strValue->setValue (value);
		return;
	}	// if (StringNamedValue::typeName == namedValue.getType ( ))

	if (BoolNamedValue::typeName == namedValue.getType ( ))
	{
		BoolNamedValue*	boolValue	= dynamic_cast<BoolNamedValue*>(&namedValue);
		CHECK_PTR (boolValue)
		if ((value != BoolNamedValue::trueStr) && (value != BoolNamedValue::falseStr))
		{
			UTF8String	errorMsg (charset);
			errorMsg  << "Impossibilité de mettre à jour la valeur nommée " << namedValue.getName ( ) << " de type booléen : "
				      << value << " n'est pas un booléen ("  << BoolNamedValue::trueStr << "/" << BoolNamedValue::falseStr << ").";
			throw Exception (errorMsg);
		}	// if ((value != BoolNamedValue::trueStr) && ...
		boolValue->setValue (value == BoolNamedValue::trueStr ? true : false);
		return;
	}	// if (BoolNamedValue::typeName == namedValue.getType ( ))

	const string	asciiValue	= value.ascii ( );
	istringstream	stream (asciiValue.c_str ( ));
	if (LongNamedValue::typeName == namedValue.getType ( ))
	{
		long	newValue;
		stream >> newValue;
		CHECK_ISTREAM (stream,value,namedValue.getName( ),namedValue.getType( ))
		LongNamedValue*	longValue	= dynamic_cast<LongNamedValue*>(&namedValue);
		CHECK_PTR (longValue)
		longValue->setValue (newValue);
		return;
	}	// if (LongNamedValue::typeName == namedValue.getType ( ))

	if (UnsignedLongNamedValue::typeName == namedValue.getType ( ))
	{
		unsigned long	newValue;
		stream >> newValue;
		CHECK_ISTREAM (stream,value,namedValue.getName( ),namedValue.getType( ))
		UnsignedLongNamedValue*	longValue	= dynamic_cast<UnsignedLongNamedValue*>(&namedValue);
		CHECK_PTR (longValue)
		longValue->setValue (newValue);
		return;
	}	// if (UnsignedLongNamedValue::typeName == namedValue.getType ( ))

	if (DoubleNamedValue::typeName == namedValue.getType ( ))
	{
		double	newValue;
		stream >> newValue;
		CHECK_ISTREAM (stream,value,namedValue.getName( ),namedValue.getType( ))
		DoubleNamedValue*	doubleValue	= dynamic_cast<DoubleNamedValue*>(&namedValue);
		CHECK_PTR (doubleValue)
		doubleValue->setValue (newValue);
		return;
	}	// if (DoubleNamedValue::typeName == namedValue.getType ( ))

	UTF8String	errorMsg (charset);
	errorMsg << "Impossibilité d'affecter la valeur " << value << " à la valeur nommée " << namedValue.getName ( )
	         << " : type inconnu (" << namedValue.getType ( ) << ").";
	throw Exception (errorMsg);
}	// updateSimpleValue


UTF8String XMLLoader::getStrValue (const DOMElement& node)
{
	// La valeur d'un element simple est stockée dans le sous-element de nom XMLLoader::valueElementName :
	DOMNodeList*	children	= node.getChildNodes ( );
	for (XMLSize_t c = 0; c < children->getLength ( ); c++)
	{
		DOMNode*	current	= children->item (c);
		CHECK_PTR (current)

		DOMElement*	currentNode	= dynamic_cast<DOMElement*>(current);
		if (0 == currentNode)
			continue;

		UTF8String	name (StrX (currentNode->getNodeName ( )).localForm ( ));	// CP v 5.5.4
//		UTF8String	name ((const char*)currentNode->getNodeName ( ), Charset::UTF_16);
		if (XMLLoader::valueElementName != name.ascii ( ))
			continue;

		// On a le bon element : recherche de sa valeur :
		DOMNodeList*	valueChildren	= currentNode->getChildNodes ( );
		for (XMLSize_t i = 0; i < valueChildren->getLength ( ); i++)
		{
			DOMNode*	textNode	= valueChildren->item (i);
			CHECK_PTR (textNode)

			if (DOMNode::TEXT_NODE != textNode->getNodeType ( ))
				continue;
			DOMText*	valueNode	= dynamic_cast<DOMText*>(textNode);
			assert (0 != valueNode);

			return UTF8String (StrX (valueNode->getNodeValue ( )).localForm ( ));	// CP v 5.5.4
//			return UTF8String ((const char*)valueNode->getNodeValue ( ), Charset::UTF_16);
		}	// for (XMLSize_t i = 0; i < valueChildren->getLength ( ); i++)
	}	// for (XMLSize_t c = 0; c < children->getLength ( ); c++)

//	throw Exception ("L'élément n'a pas de valeur.");
	return UTF8String (charset);
}	// XMLLoader::getStrValue


void XMLLoader::updateComplexValue (NamedValue& namedValue, const DOMElement& node)
{
	updateComments (namedValue, node);

	DOMNodeList*	children	= node.getChildNodes ( );
	if (0 == children)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Le noeud " << namedValue.getName ( ) << " n'a pas d'enfant. Impossible de reconstituer la valeur nommée complexe.";
		throw Exception (errorMsg);
	}	// if (0 == children)

	bool	done3 [3]	= {false, false, false};
	if (ColorNamedValue::typeName == namedValue.getType ( ))
	{
		ColorNamedValue*	color	=  dynamic_cast<ColorNamedValue*>(&namedValue);
		CHECK_PTR (color)
		double				red		= color->getRed ( ), green	= color->getGreen ( ), blue	= color->getBlue ( );

		for (XMLSize_t i = 0; i < children->getLength ( ); i++)
		{
			DOMNode*	child	= children->item (i);
			DOMElement*	current	= dynamic_cast<DOMElement*> (child);
			if (0 == current)
				continue;

			const string	name	=  StrX (current->getNodeName ( )).localForm ( );
			if ((name != "red") && (name != "green") && (name != "blue"))
				continue;

			DOMNodeList*	values	= current->getChildNodes ( );
			for (XMLSize_t j = 0; j < values->getLength ( ); j++)
			{
				DOMNode*	valueNode	= values->item (j);
				if (DOMNode::TEXT_NODE != valueNode->getNodeType ( ))
					continue;

				DOMText*	textNode	= dynamic_cast<DOMText*>(valueNode);
				assert (0 != textNode);
				const string	value	= StrX (textNode->getNodeValue ( )).localForm ( );
				double		newValue;
				istringstream	stream (value.c_str ( ));
				stream >> newValue;
				CHECK_ISTREAM (stream, value, namedValue.getName ( ), namedValue.getType ( ))

				if (name == "red")
				{ red   = newValue; done3 [0]   = true; }
				else if (name == "green")
				{ green = newValue; done3 [1]   = true; }
				else if (name == "blue")
				{ blue  = newValue; done3 [2]   = true; }
			}	// for (XMLSize_t j = 0; j < values->getLength ( ); j++)
		}	// for (XMLSize_t i = 0; i < children->getLength ( ); i++)

		for (int j = 0; j < 3; j++)
			if (false == done3 [j])
			{
				UTF8String	errorMsg (charset);
				errorMsg << "Informations incomplètes pour la valeur nommée de type couleur " << namedValue.getName ( );
				throw Exception (errorMsg);
			}	// if (false == done3 [j])

		color->setValue (red, green, blue);

		return;
	}	// if (ColorNamedValue::typeName == namedValue.getType ( ))

	if (DoubleTripletNamedValue::typeName == namedValue.getType ( ))
	{
		DoubleTripletNamedValue*	dtValue	=  dynamic_cast<DoubleTripletNamedValue*>(&namedValue);
		CHECK_PTR (dtValue)
		double		x	= dtValue->getX ( ), y	= dtValue->getY ( ), z	= dtValue->getZ ( );

		for (XMLSize_t i = 0; i < children->getLength ( ); i++)
		{
			DOMNode*	child	= children->item (i);
			DOMElement*	current	= dynamic_cast<DOMElement*> (child);
			if (0 == current)
				continue;

			const string	name	= StrX (current->getNodeName ( )).localForm ( );
			if ((name != "x") && (name != "y") && (name != "z"))
				continue;

			DOMNodeList*	values	= current->getChildNodes ( );
			for (XMLSize_t j = 0; j < values->getLength ( ); j++)
			{
				DOMNode*	valueNode	= values->item (j);
				if (DOMNode::TEXT_NODE != valueNode->getNodeType ( ))
					continue;

				DOMText*	textNode	= dynamic_cast<DOMText*>(valueNode);
				assert (0 != textNode);
				const string	value	= StrX (textNode->getNodeValue ( )).localForm ( );
				double		newValue;
				istringstream	stream (value.c_str ( ));
				stream >> newValue;
				CHECK_ISTREAM (stream, value, namedValue.getName ( ), namedValue.getType ( ))

				if (name == "x")
				{ x   = newValue; done3 [0]   = true; }
				else if (name == "y")
				{ y = newValue; done3 [1]   = true; }
				else if (name == "z")
				{ z  = newValue; done3 [2]   = true; }
			}	// for (XMLSize_t j = 0; j < values->getLength ( ); j++)
		}	// for (XMLSize_t i = 0; i < children->getLength ( ); i++)

		for (int j = 0; j < 3; j++)
			if (false == done3 [j])
			{
				UTF8String	errorMsg (charset);
				errorMsg << "Informations incomplètes pour la valeur nommée de type triplet de double " << namedValue.getName ( );
				throw Exception (errorMsg);
			}	// if (false == done3 [j])

		dtValue->setValue (x, y, z);

		return;
	}	// if (DoubleTripletNamedValue::typeName == namedValue.getType ( ))
}	// XMLLoader::updateComplexValue


void XMLLoader::updateComments (Element& element, const DOMElement& node)
{
	DOMNodeList*	children	= node.getChildNodes ( );
	for (XMLSize_t c = 0; c < children->getLength ( ); c++)
	{
		DOMNode*	currentNode	= children->item (c);
		CHECK_PTR (currentNode)

		// On ne s'interesse qu'aux "elements"> Les DOMTextNode peuvent
		// introduire des sections. C'est par exemple le cas des tabulations
		// en debut de section.
		if (DOMNode::ELEMENT_NODE != currentNode->getNodeType ( ))
			continue;
		DOMElement*	currentElement	= dynamic_cast<DOMElement*>(currentNode);
		if (0 == currentElement)
			continue;

		const string	nodeName	= StrX (currentNode->getNodeName ( )).localForm ( );

		// Cas particulier : element "annotation" => c'est un commentaire :
		if (nodeName == "annotation")
		{
			DOMNodeList*	annotations	= currentNode->getChildNodes ( );
			for (XMLSize_t a = 0; a < annotations->getLength ( ); a++)
			{
				DOMNode*	current	= annotations->item (a);
				CHECK_PTR (current)
				const string	annName	= StrX (current->getNodeName ( )).localForm ( );
				if (annName != "documentation")
					continue;

				DOMNodeList*	docs	= current->getChildNodes ( );
				for (XMLSize_t d = 0; d < docs->getLength ( ); d++)
				{
					DOMNode*	doc	= docs->item (d);
					CHECK_PTR (doc)
					if (DOMNode::TEXT_NODE != doc->getNodeType ( ))
						continue;

					DOMText*	comment	= dynamic_cast<DOMText*>(doc);
					assert (0 != comment);
					UTF8String	us (StrX (comment->getNodeValue ( )).localForm ( ), Charset::UTF_8);	// CP v 5.6.0
//					UTF8String	us ((const char*)comment->getNodeValue ( ), Charset::UTF_16);
					element.setComment (us);
				}	// for (XMLSize_t d = 0; d < docs->getLength ( ); d++)
			}	// for (XMLSize_t a = 0; a < annotations->getLength ( ); a++)
		}	// if (nodeName == "annotation")
	}	// for (XMLSize_t c = 0; c < children->getLength ( ); c++)
}	// updateComments


void XMLLoader::addChildren (const Section& section, DOMElement& element, DOMDocument& doc, ENCODING encoding)
{
	vector<Section*>	subsections	= section.getSections ( );
	for (vector<Section*>::const_iterator its = subsections.begin ( ); subsections.end ( ) != its; its++)
	{
		if (true == (*its)->isSafeguardable ( ))	// v 5.7.0
		{
			DOMElement*	e	= createElement (**its, doc, encoding);

			element.appendChild (e);
			addChildren (**its, *e, doc, encoding);
		}	// if (true == (*its)->isSafeguardable ( ))
	}	// for (vector<Section*>::const_iterator its = subsections.begin ( );...

	vector<NamedValue*>	values	= section.getNamedValues ( );
	for (vector<NamedValue*>::const_iterator	itnv	= values.begin ( ); values.end ( ) != itnv; itnv++)
	{
		if (true == (*itnv)->isSafeguardable ( ))	// v 5.7.0
		{
			// Valeur de l'element. Si c'est un type XML schema non complexe on lui affecte en tant qu'attribut. Dans le cas contraire on lui affecte a 
			// l'aide d'elements.
			if (true == isSimpleType (**itnv))
				addSimpleNamedValue (**itnv, element, doc, encoding);
			else
				addComplexNamedValue (**itnv, element, doc, encoding);
		}	// if (true == (*its)->isSafeguardable ( ))
	}	// for (vector<NamedValue*>::const_iterator	itnv	= ...
}	// XMLLoader::addChildren


DOMElement* XMLLoader::addSimpleNamedValue (const NamedValue& element, DOMElement& container, DOMDocument& doc, ENCODING encoding)
{
	DOMElement*	e		= createElement (element, doc, encoding);
	assert (0 != e);

	// La valeur de l'element : contenue dans un DOMElement sans commentaire afin d'etre sur d'etre lu sans eventuels indentations ou sauts de lignes.
	DOMElement*	valElem	= doc.createElement (X (XMLLoader::valueElementName.c_str ( )));
	CHECK_PTR (valElem)
	DOMText*	value	= doc.createTextNode (X (element.getStrValue ( ).c_str ( )));
	CHECK_PTR (value)
	valElem->appendChild (value);
	e->appendChild (valElem);
	                 
	container.appendChild (e);

	return e;
}	// XMLLoader::addSimpleNamedValue


DOMElement* XMLLoader::addComplexNamedValue (const NamedValue& element, DOMElement& container, DOMDocument& doc, ENCODING encoding)
{
	const DoubleTripletNamedValue*	dtValue		= dynamic_cast<const DoubleTripletNamedValue*>(&element);
	const ColorNamedValue*			colorValue	= dynamic_cast<const ColorNamedValue*>(&element);

	if ((0 == dtValue) && (0 == colorValue))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Le type de préférence " << element.getType ( ) << " n'est pas un type XML schema complexe.";
		INTERNAL_ERROR (exc, errorMsg, "XMLLoader::addComplexNamedValue")
		throw exc;
	}

	DOMElement*	e	= createElement (element, doc, encoding);
	CHECK_PTR (e)

	if (0 != colorValue)
	{
		UTF8String	red (charset), green (charset), blue (charset);
		red << colorValue->getRed ( );
		green << colorValue->getGreen ( );
		blue << colorValue->getBlue ( );
		DOMElement*		redElem		= doc.createElement (X ("red"));
		DOMElement*		greenElem	= doc.createElement (X ("green"));
		DOMElement*		blueElem	= doc.createElement (X ("blue"));
		CHECK_PTR (redElem)
		CHECK_PTR (greenElem)
		CHECK_PTR (blueElem)
		DOMText*	redValue	= doc.createTextNode (X(red.iso( ).c_str( )));
		DOMText*	greenValue	= doc.createTextNode (X(green.iso( ).c_str( )));
		DOMText*	blueValue	= doc.createTextNode (X(blue.iso( ).c_str( )));
		CHECK_PTR (redValue)
		CHECK_PTR (greenValue)
		CHECK_PTR (blueValue)
		redElem->appendChild (redValue);
		greenElem->appendChild (greenValue);
		blueElem->appendChild (blueValue);
		e->appendChild (redElem);
		e->appendChild (greenElem);
		e->appendChild (blueElem);
	}	// if (0 != colorValue)
	else	// => DoubleTripletNamedValue
	{
		UTF8String	x (charset), y (charset), z (charset);
		x << dtValue->getX ( );
		y << dtValue->getY ( );
		z << dtValue->getZ ( );
		DOMElement*		xElem		= doc.createElement (X ("x"));
		DOMElement*		yElem	= doc.createElement (X ("y"));
		DOMElement*		zElem	= doc.createElement (X ("z"));
		CHECK_PTR (xElem)
		CHECK_PTR (yElem)
		CHECK_PTR (zElem)
		DOMText*	xValue	= doc.createTextNode (X(x.iso( ).c_str( )));
		DOMText*	yValue	= doc.createTextNode (X(y.iso( ).c_str( )));
		DOMText*	zValue	= doc.createTextNode (X(z.iso( ).c_str( )));
		CHECK_PTR (xValue)
		CHECK_PTR (yValue)
		CHECK_PTR (zValue)
		xElem->appendChild (xValue);
		yElem->appendChild (yValue);
		zElem->appendChild (zValue);
		e->appendChild (xElem);
		e->appendChild (yElem);
		e->appendChild (zElem);
	}	// DoubleTripletNamedValue

	container.appendChild (e);

	return e;
}	// XMLLoader::addComplexNamedValue


DOMElement* XMLLoader::createElement (const Element& element, DOMDocument& doc, ENCODING encoding)
{
	UTF8String	typeName	= XMLSchemaPrefsHelper::elementToXMLschemaTypeName (element);
//	DOMElement*	e	= XMLLoader::UTF_8 == encoding ? doc.createElement (X (typeName.ascii ( ).c_str ( ))) : doc.createElement (typeName.unicode ( ));
	DOMElement*	e	= doc.createElement(unicodeToXMLString (typeName,encoding));
	CHECK_PTR (e)

	// L'attribut "nom" :
	UTF8String	name	= element.getName ( );
	DOMAttr*		attr	= doc.createAttribute (X (XMLLoader::nameAttrName.c_str ( )));
	CHECK_PTR (attr)
//	attr->setValue (XMLLoader::UTF_8 == encoding ? X (name.ascii( ).c_str( )) : name.unicode ( ));
	attr->setValue (unicodeToXMLString (name, encoding));
	e->setAttributeNode (attr);
	
	// L'attribut "type" :
	attr	= doc.createAttribute (X (XMLLoader::typeAttrName.c_str ( )));
	CHECK_PTR (attr)
	attr->setValue (X (XMLSchemaPrefsHelper::elementToXMLschemaType (element).c_str ( )));
	e->setAttributeNode (attr);

	// L'attribut surchargeable :
	if (false == element.isOverloadable ( ))
	{	// v 5.7.0
		attr	= doc.createAttribute (X (XMLLoader::overloadableAttrName.c_str ( )));
		CHECK_PTR (attr)
		attr->setValue (X ("false"));
		e->setAttributeNode (attr);
	}	// if (false == element.isOverloadable ( ))

	// L'attribut enregistrable :
	if (false == element.isSafeguardable ( ))
	{	// v 5.7.0
		attr	= doc.createAttribute (X (XMLLoader::safeguardableAttrName.c_str ( )));
		CHECK_PTR (attr)
		attr->setValue (X ("false"));
		e->setAttributeNode (attr);
	}	// if (false == element.isSafeguardable ( ))
		
	addComments (element, *e, doc, encoding);

	return e;
}	// XMLLoader::createElement


void XMLLoader::addComments (const Element& element, DOMElement& node, DOMDocument& doc, ENCODING encoding)
{
	if (true == element.hasComment ( ))
	{
		DOMElement*	annotations		= doc.createElement (X ("annotation"));
		CHECK_PTR (annotations)
		node.appendChild (annotations);
		DOMElement*	documentation	= doc.createElement(X("documentation"));
		CHECK_PTR (documentation)
		annotations->appendChild (documentation);
//		DOMText*	comment		= XMLLoader::UTF_8 == encoding ?
//		doc.createTextNode (X (element.getComment ( ).ascii ( ).c_str ( ))) :
//		doc.createTextNode ((const XMLCh*)element.getComment ( ).unicode ( ));
		DOMText*	comment		=
		doc.createTextNode (unicodeToXMLString(element.getComment( ),encoding));

		CHECK_PTR (comment)
		documentation->appendChild (comment);
	}	// if (true == element.hasComment ( ))
}	// addComments


END_NAMESPACE_PREFS

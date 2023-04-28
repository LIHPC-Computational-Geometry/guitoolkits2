#ifndef XML_PREFS_SAX_ERROR_HANDLER_H
#define XML_PREFS_SAX_ERROR_HANDLER_H

#include <PrefsCore/prefs_config.h>
#include <PrefsXerces/XercesUtils.h>
#include <PrefsXerces/XMLPrefsErrorHandler.h>

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>



BEGIN_NAMESPACE_PREFS

/**
 * Classe de gestionnaire d'erreurs pour les parsers XML de type SAX. Conserve 
 * une liste de messages par type d'erreurs possibles. Formate les liste de 
 * messages sous forme de chaine de caractères.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class XMLPrefsSAXErrorHandler : 
				public XERCES_CPP_NAMESPACE_QUALIFIER ErrorHandler,
				public XMLPrefsErrorHandler
{
	public :

	/**
	 * Conctructeur. RAS.
	 */
	XMLPrefsSAXErrorHandler ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~XMLPrefsSAXErrorHandler ( );

	/**
	 * API SAX.
	 * Ajoute l'exception reçue en argument à la liste des avertissements
	 * déjà reçus.
	 */
	virtual void warning (
				const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& saxExc);

	/**
	 * API SAX.
	 * Ajoute l'exception reçue en argument à la liste des erreurs
	 * déjà reçues.
	 */
	virtual void error (
				const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& saxExc);

	/**
	 * API SAX.
	 * Ajoute l'exception reçue en argument à la liste des erreurs fatales
	 * déjà reçues.
	 */
	virtual void fatalError (
				const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& saxExc);

	/**
	 * API SAX.
	 * Annule toutes les erreurs et avertissements en cours.
	 */
	virtual void resetErrors ( );


	protected :

	/**
	 * @return		Le message correspondant à l'exception tranmise en argument.
	 */
	virtual IN_UTIL UTF8String formatMessage (
		const XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException& saxExc) const;
	

	private :

	/** Constructeurs de copie et opérateur = : interdits.
	 */
	XMLPrefsSAXErrorHandler (const XMLPrefsSAXErrorHandler&);
	XMLPrefsSAXErrorHandler& operator = (const XMLPrefsSAXErrorHandler&);
};	// class XMLPrefsErrorHandler


END_NAMESPACE_PREFS


#endif		// XML_PREFS_SAX_ERROR_HANDLER_H

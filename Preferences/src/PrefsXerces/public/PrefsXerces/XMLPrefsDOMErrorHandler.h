#ifndef XML_PREFS_DOM_ERROR_HANDLER_H
#define XML_PREFS_DOM_ERROR_HANDLER_H

#include <PrefsCore/prefs_config.h>
#include <PrefsXerces/XercesUtils.h>
#include <PrefsXerces/XMLPrefsErrorHandler.h>

#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/dom/DOMErrorHandler.hpp>




BEGIN_NAMESPACE_PREFS

/**
 * Classe de gestionnaire d'erreurs pour les parsers XML de type DOM. Conserve 
 * une liste de messages par type d'erreurs possibles. Formate les liste de 
 * messages sous forme de chaine de caractères.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class XMLPrefsDOMErrorHandler : 
				public XERCES_CPP_NAMESPACE_QUALIFIER DOMErrorHandler,
				public XMLPrefsErrorHandler
{
	public :

	/**
	 * Conctructeur. RAS.
	 */
	XMLPrefsDOMErrorHandler ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~XMLPrefsDOMErrorHandler ( );

	/**
	 * API DOM.
	 * Ajoute l'exception reçue en argument à la liste des avertissements,
	 * erreurs ou erreurs fatales déjà reçus.
	 * @return		true si le traitement doit continuer, sinon false.
	 *				Par défaut retourne true si l'erreur est de type
	 *				avertissement, sinon false.
	 */
	virtual bool handleError (
				const XERCES_CPP_NAMESPACE_QUALIFIER DOMError& error);


	protected :

	/**
	 * @return		Le message correspondant à l'exception tranmise en argument.
	 */
	virtual IN_UTIL UTF8String formatMessage (
		const XERCES_CPP_NAMESPACE_QUALIFIER DOMError& error) const;
	

	private :

	/** Constructeurs de copie et opérateur = : interdits.
	 */
	XMLPrefsDOMErrorHandler (const XMLPrefsDOMErrorHandler&);
	XMLPrefsDOMErrorHandler& operator = (const XMLPrefsDOMErrorHandler&);
};	// class XMLPrefsDOMErrorHandler


END_NAMESPACE_PREFS


#endif		// XML_PREFS_DOM_ERROR_HANDLER_H

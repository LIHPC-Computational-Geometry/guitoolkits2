#ifndef XML_PREFS_ERROR_HANDLER_H
#define XML_PREFS_ERROR_HANDLER_H

#include <PrefsCore/prefs_config.h>
#include <TkUtil/UTF8String.h>


#include <vector>


BEGIN_NAMESPACE_PREFS

/**
 * Classe proposant des services de base pour des gestionnaires d'erreurs pour 
 * les parsers XML. Conserve une liste de messages par type d'erreurs possibles.
 * Formate les liste de messages sous forme de chaine de caractères.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class XMLPrefsErrorHandler
{
	public :

	/**
	 * Conctructeur. RAS.
	 */
	XMLPrefsErrorHandler ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~XMLPrefsErrorHandler ( );

	/**
	 * Ajoute le message reçu en argument à la liste des avertissements
	 * déjà reçus.
	 */
	virtual void addWarning (const IN_UTIL UTF8String& warning);

	/**
	 * Ajoute le message reçu en argument à la liste des erreurs
	 * déjà reçues.
	 */
	virtual void addError (const IN_UTIL UTF8String& error);

	/**
	 * Ajoute le message reçu en argument à la liste des erreurs fatales
	 * déjà reçues.
	 */
	virtual void addFatalError (const IN_UTIL UTF8String& error);

	/**
	 * Annule toutes les erreurs et avertissements en cours.
	 */
	virtual void resetErrors ( );

	/**
	 * @return		true s'il y a des avertissement en cours, sinon false.
	 */
	virtual bool hasWarnings ( ) const
	{ return 0 == _warnings.size ( ) ? false : true; }

	/**
	 * @return		true s'il y a des erreurs en cours, sinon false.
	 */
	virtual bool hasErrors ( ) const
	{ return 0 == _errors.size ( ) ? false : true; }

	/**
	 * @return		true s'il y a des erreurs fatales en cours, sinon false.
	 */
	virtual bool hasFatalErrors ( ) const
	{ return 0 == _fatalErrors.size ( ) ? false : true; }

	/**
	 * @return		true en l'absence d'erreur ou avertissement, sinon false.
	 */
	virtual bool ok ( ) const;

	/**
	 * @return		La liste des avertissements en cours.
	 */
	const IN_STD vector<IN_UTIL UTF8String>& warnings ( ) const
	{ return _warnings; }
	
	/**
	 * @return		La liste des erreurs en cours.
	 */
	const IN_STD vector<IN_UTIL UTF8String>& errors ( ) const
	{ return _errors; }
	
	/**
	 * @return		La liste des erreurs fatales en cours.
	 */
	const IN_STD vector<IN_UTIL UTF8String>& fatalErrors ( ) const
	{ return _fatalErrors; }

	/**
	 * @return		Une chaine de caractères comportant l'ensemble des 
	 *				avertissements.
	 */
	virtual IN_UTIL UTF8String warningMessages ( ) const;

	/**
	 * @return		Une chaine de caractères comportant l'ensemble des 
	 *				erreurs.
	 */
	virtual IN_UTIL UTF8String errorMessages ( ) const;

	/**
	 * @return		Une chaine de caractères comportant l'ensemble des 
	 *				erreurs fatales.
	 */
	virtual IN_UTIL UTF8String fatalErrorMessages ( ) const;

	/**
	 * @return		Une chaine de caractères comportant l'ensemble des 
	 *				avertissements et erreurs.
	 */
	virtual IN_UTIL UTF8String wholeMessages ( ) const;


	protected :


	private :

	/** Constructeurs de copie et opérateur = : interdits.
	 */
	XMLPrefsErrorHandler (const XMLPrefsErrorHandler&);
	XMLPrefsErrorHandler& operator = (const XMLPrefsErrorHandler&);

	/** Les warnings, erreurs et erreurs fatales. */
	IN_STD vector<IN_UTIL UTF8String>	_warnings, _errors, _fatalErrors;
};	// class XMLPrefsErrorHandler


END_NAMESPACE_PREFS


#endif		// XML_PREFS_ERROR_HANDLER_H

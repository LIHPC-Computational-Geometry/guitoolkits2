#ifndef XERCES_SESSION_MANAGER_H
#define XERCES_SESSION_MANAGER_H

#include <PrefsCore/prefs_config.h>

#include <sys/types.h>		// size_t


BEGIN_NAMESPACE_PREFS


/** <P>Classe utilitaire chargée d'initialiser et de terminer automatiquement
 * les sessions Xerces.</P>
 * <P>Usage :<BR>
 * <CODE>
 * void A::function ( )<BR>
 * {<BR>
 * // ...<BR>
 * XercesSessionManager	manager;<BR>
 * // Code utilisant Xerces<BR>
 * // ...<BR>
 * }	// A::function<BR>
 * </CODE>
 * </P>
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class XercesSessionManager 
{
	public :

	/**
	 * Constructeur.
	 * Initialise la session Xerces si elle n'est pas déjà initialisée.
	 * @throw		Exception en cas d'échec de l'initialisation.
	 */
	XercesSessionManager ( );

	/**
	 * Termine la session Xerces si c'est la dernière ouverte.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	~XercesSessionManager ( );


	private :

	/**
	 * Constructeur de copie. Interdit.
	 */
	XercesSessionManager (const XercesSessionManager&)
	{ }

	/**
	 * Opérateur =. Interdit.
	 */
	XercesSessionManager& operator = (const XercesSessionManager&)
	{ return *this; }

	/** Le nombre de sessions ouvertes. */
	static size_t			_sessionCount;
};	// class XercesSessionManager




END_NAMESPACE_PREFS

#endif	// XERCES_SESSION_MANAGER_H

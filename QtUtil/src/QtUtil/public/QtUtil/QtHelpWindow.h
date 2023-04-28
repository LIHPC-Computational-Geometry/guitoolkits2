#ifndef QT_HELP_WINDOW_H
#define QT_HELP_WINDOW_H

#include <TkUtil/util_config.h>

#include "QtUtil/QtWebBrowser.h"


/** <P>Classe de fenêtre d'aide en ligne <B>HTML</B> pour une application Qt.
 * Cette classe n'accepte qu'une seule instance.</P>
 */
class QtHelpWindow : public QtWebBrowser
{
	public	:

	/**
	 * Initialisation du système d'aide. Fonction à appeler avant tout
	 * utilisation du sytème d'aide.
	 * @param		URL racine du système d'aide
	 * @param		index (relatif par rapport à homeURL) du système
	 *				d'aide.
	 * @param		Titre de la fenêtre
	 * @param		Nom de l'application (pour les messages d'erreur 
	 *				éventuels)
	 */
	static void initialize (const IN_STD string& homeURL, 
	                        const IN_STD string& indexURL,
	                        const IN_STD string& title,
	                        const IN_STD string& appTitle);

	/**
	 * Met fin au système d'aide.
	 */
	static void finalize ( );

	/**
	 * Affiche le fichier transmis en argument.
	 * @param		Nom relatif du fichier par rapport à la racine du
	 *				système d'aide.
	 * @param		Balise visée
	 * @exception	Lève une exception en cas d'erreur.
	 */
	virtual void setURL (const IN_STD string& url, const IN_STD string& tag);

	/**
	 * Affiche le fichier transmis en argument.
	 * @param		Nom relatif du fichier par rapport à la racine du
	 *				système d'aide.
	 * @exception	Lève une exception en cas d'erreur.
	 */
	static void displayURL (const IN_STD string& url);

	/**
	 * Affiche le fichier transmis en argument.
	 * @param		Nom relatif du fichier par rapport à la racine du
	 *				système d'aide.
	 * @param		Balise visée
	 * @exception	Lève une exception en cas d'erreur.
	 */
	static void displayURL (const IN_STD string& url, const IN_STD string& tag);


	protected	:

	/**
	 * Constructeur
	 * @param		Titre de la fenêtre
	 */
	QtHelpWindow (const IN_STD string& title);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtHelpWindow ( );

	/**
	 * @return		l'instance en cours
	 * @exception	Une exception est levée si le système d'aide n'est
	 *				pas opérationnel.
	 */
	static QtHelpWindow& getInstance ( );


	private	:

	/** 
	 * Constructeur de copie : interdit.
	 */
	QtHelpWindow (const QtHelpWindow&);

	/** 
	 * Opérateur = : interdit.
	 */
	QtHelpWindow& operator = (const QtHelpWindow&);

	/**
	 * L'unique instance de l'application.
	 */
	static QtHelpWindow*				_instance;

	/**
	 * La racine du système d'aide
	 */
	static IN_STD string				_helpRootURL;

	/**
	 * L'index du sytème d'aide.
	 */
	static IN_STD string				_helpIndexURL;

	/**
	 * Le titre de l'application.
	 */
	static IN_STD string				_appTitle;

	/**
	 * Le titre de la boite de la fenêtre.
	 */
	static IN_STD string				_windowTitle;
};	// class QtHelpWindow

#endif	// QT_HELP_WINDOW_H

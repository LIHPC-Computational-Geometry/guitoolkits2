#ifndef QT_PREFERENCES_DIALOG_H
#define QT_PREFERENCES_DIALOG_H

#include <PrefsCore/prefs_config.h>
#include <PrefsQt/QtPreferencesPanel.h>

#ifndef QT_5
#include <QtGui/QDialog>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#else	// QT_5
#include <QDialog>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#endif	// QT_5

#include <string>


/**
 * Boite de dialogue Qt permettant d'éditer une configuration. Son menu
 * permet de charger et sauvegarder des fichiers de configuration au
 * format XML.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 * @see		XMLLoader
 */
class QtPreferencesDialog : public QDialog 
{
	Q_OBJECT


	public :

	/** Constructeur. Appelle createMenu.
	 * @param		widget parent. Ne doit pas être nul.
	 * @param		Section à éditer. Cette section sera détruite
	 *				par le destructeur si releaseSection n'a pas été
	 *				appelé.
	 * @param		Fichier d'où est issue la section.
	 * @param		true si le panneau est éditable, sinon false.
	 * @param		true si le menu doit avoir un item "Appliquer", sinon false.
	 * @param		true si la boite de dialogue est modale, sinon false.
	 * @param		titre de la boite de dialogue.
	 * @param		titre de l'application (pour les messages d'erreur).
	 */
	QtPreferencesDialog (QWidget* parent, IN_PREFS Section* section,
	                     const IN_STD string& fromFile, bool editable, 
	                     bool withApply,
	                     bool modal, const IN_UTIL UTF8String& dlgTitle,
	                     const IN_UTIL UTF8String& appTitle);

	/** 
	 * Destructeur. Détruit la section gérée si celle-ci n'a pas
	 * été récupérée par releaseSection.
	 */
	virtual ~QtPreferencesDialog ( );

	/**
	 * @return		la section gérée et vide le panneau de la
	 *				configuration en cours.
	 */
	virtual IN_PREFS Section* releaseSection ( );

	/**
	 * @return		true si une configuration est chargée, sinon false.
	 */
	virtual bool hasSection ( ) const;

	/**
	 * @return		une référence sur la section racine de la
	 *				configuration chargée et affichée.
	 * @exception	Une exception est levée s'il n'y a pas de configuration
	 *				de chargée.
	 */
	virtual const IN_PREFS Section& getSection ( ) const;

	/**
	 * @return		une référence sur la section racine de la
	 *				configuration à enregistrer. Par défaut, retourne 
	 *				getSection ( ).
	 * @exception	Une exception est levée s'il n'y a pas de configuration
	 *				de chargée.
	 */
	virtual const IN_PREFS Section& getSafeguardedSection ( );

	/**
	 * @return		true si la configuration est éditable, sinon false.
	 */
	virtual bool isEditable ( ) const;


	protected :

	/**
	 * Crée un menu associé à la boite de dialogue.
	 * @param		true si le panneau est éditable, sinon false.
	 * @param		true si le menu doit avoir un item "Appliquer", sinon false.
	 */
	virtual QMenuBar* createMenu (bool editable, bool withApply);

	/**
	 * Affiche le message d'erreur reçu en argument.
	 */
	virtual void displayErrorMessage (const IN_UTIL UTF8String& message);

	/**
	 * @return		Le menu de la boîte de dialogue.
	 */
	virtual QMenuBar* getMenuBar ( ) const
	{ return _menuBar; }

	/**
	 * @return		Le menu "Aide".
	 */
	virtual QMenu* getHelpMenu ( ) const
	{ return _helpMenu; }


	protected slots :

	/**
	 * Appelé lorsque l'utilisateur active l'item "Ouvrir ..." du menu 
	 * "Fichier". Décharge la configuration en cours et charge celle 
	 * demandée par l'utilisateur.
	 */
	virtual void loadCallback ( );

	/**
	 * Appelé lorsque l'utilisateur active l'item "Enregistrer" du menu
	 * "Fichier".
	 * Enregistre la configuration en cours dans son fichier associé.
	 */
	virtual void saveCallback ( );

	/**
	 * Appelé lorsque l'utilisateur active l'item "Enregistrer sous ..." du 
	 * menu "Fichier".
	 * Enregistre la configuration en cours dans un fichier dont le nom est
	 * renseigné par l'utilisateur.
	 */
	virtual void saveAsCallback ( );

	/**
	 * Appelé lorsque l'utilisateur active l'item "Appliquer".
	 * Ne fait rien par défaut, cette méthode est à surcharger.
	 */
	virtual void applyCallback ( );

	/**
	 * Appellé lorsque l'utilisateur active l'item "Insérer une section"
	 * du menu "Edition".
	 * Ajoute une section à la section sélectionnéee.
	 */
	virtual void insertSectionCallback ( );

	/**
	 * Appellées lorsque l'utilisateur active un item d'insertion d'une 
	 * valeur nomméee.
	 * Ajoute une valeur nommée àa section sélectionnée.
	 */
	virtual void insertBooleanCallback ( );
	virtual void insertColorCallback ( );
	virtual void insertStringCallback ( );
	virtual void insertLongCallback ( );
	virtual void insertUnsignedLongCallback ( );
	virtual void insertDoubleCallback ( );
	virtual void insertDoubleTripletCallback ( );

	/**
	 * Appelé lorsque l'utilisateur active l'item "Supprimer" du menu
	 * "Edition".
	 * Supprime la sélection courrante.
	 */
	virtual void removeCallback ( );

	/**
	 * Appelé lorsque l'utilisateur active l'item "Commentaires ..." 
	 * du menu "Edition".
	 * Affiche une boite de dialogue d'édition des commentaires.
	 */
	virtual void editCommentCallback ( );

	/**
	 * Appelé lorsque l'utilisateur active l'item "A propos de ..." 
	 * du menu "Aide".
	 * Affiche une boite de dialogue d'informations sur cette classe.
	 */
	virtual void aboutCallback ( );


	private :

	/** Constructeur de copie. Interdit. */
	QtPreferencesDialog (const QtPreferencesDialog&);

	/** Opérateur de copie. Interdit. */
	QtPreferencesDialog& operator = (const QtPreferencesDialog&);

	/** La section gérée. */
	QtPreferencesPanel*		_prefsPanel;

	/** La zone d'affichage du nom du fichier de preferences en cours. */
	QStatusBar*				_statusBar;

	/** Le titre de l'application (pour les messages d'erreur). */
	IN_UTIL UTF8String		_appTitle;

	/** Le fichier associé à la configuration. */
	IN_STD string			_sectionFile;

	/** Le menu de la boîte de dialogue. */
	QMenuBar*				_menuBar;

	/** Le menu "Aide". */
	QMenu*					_helpMenu;
};	// class QtPreferencesDialog



#endif	// QT_PREFERENCES_DIALOG_H

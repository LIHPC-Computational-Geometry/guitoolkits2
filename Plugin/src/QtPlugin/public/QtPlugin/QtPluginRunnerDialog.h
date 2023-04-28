
#ifndef QT_PLUGIN_RUNNER_DIALOG
#define QT_PLUGIN_RUNNER_DIALOG


#include <PrefsQt/QtPreferencesDialog.h>
#include <PluginCore/Plugin.h>


/**
 * \mainpage    Page principale de la bibliothèque <I>QtPlugin</I>.
 *
 * Boite de dialogue d'édition / lancement de plugins.
 */
class QtPluginRunnerDialog : public QtPreferencesDialog
{
	Q_OBJECT


	public :

	/**
	 * @param		Widget parent. Ne doit pas être nul.
	 * @param		Section à éditer. Cette section sera détruite
	 *				par le destructeur si releaseSection n'a pas été
	 *				appelé.
	 * @param		Plugin associé. Ne doit pas être nul.
	 *				Fichier d'où est issue la section.
	 * @param		true si le menu doit avoir un item "Appliquer", sinon false.
	 * @param		true si la boite de dialogue est modale, sinon false.
	 * @param		titre de la boite de dialogue.
	 * @param		titre de l'application (pour les messages d'erreur).
	 * @exception	Une exception est levée en cas d'argument invalide.
	 */
	QtPluginRunnerDialog (QWidget* parent, IN_PREFS Section* section,
	                      PLUGIN::Plugin* plugin,
	                      const IN_STD string& fromFile, bool withApply,
	                      bool modal, const IN_STD string& dlgTitle,
	                      const IN_STD string& appTitle);

	/**
	 * Destructeur. Détruit la section gérée si celle-ci n'a pas été
	 * récupérée par releaseSection.
	 */
	virtual ~QtPluginRunnerDialog ( );

	/**
	 * @return		une référence sur la section racine de la configuration
	 *				chargée du plugin. Cette section intègre l'éventuelle 
	 *				section parente.
	 * @exception	Une exception est levée s'il n'y a pas de configuration
	 *				de chargée.
	 * @see			getPluginParentSection
	 * @see			setPluginParentSection
	 */
	virtual const IN_PREFS Section& getSafeguardedSection ( );

	/**
	 * @return		Le plugin.
	 */
	PLUGIN::Plugin& getPlugin ( );
	const PLUGIN::Plugin& getPlugin ( ) const;

	/**
	 * @return		L'éventuelle section dans laquelle le plugin doit être 
	 *				enregistré.
	 * @see			setPluginParentSection
	 */
	virtual IN_PREFS Section* getPluginParentSection ( ) const;

	/**
	 * @param		Section dans laquelle sera enregistrée celle du plugin
	 *				en cas d'opération de sauvegarde de l'utilisateur. Dans le
	 *				cas où cette section a une sous-section, la section du
	 *				plugin est ajoutée à la sous-section. Cette section tranmise
	 *				en argument est détruite par cette instance.
	 */
	virtual void setPluginParentSection (IN_PREFS Section* section);

	/**
	 * @return		La version de cette bibliothèque.
	 */
	static const IN_UTIL Version& getVersion ( )
	{ return _version; }


	protected slots :

	/**
	 * Appelé lorsque l'utilisateur active l'item "exécuter" du menu
	 * "plugin". Réinitialise puis exécute le plugin.
	 * @see		reinitializePlugin
	 */
	virtual void runPluginCallback ( );


	protected :

	/**
	 * Réinitialise la fonction plugin conformément à la définition
	 * renseignée dans l'IHM.
	 * @exception		Un exception est levée en cas d'erreur.
	 */
	virtual void reinitializePlugin ( );


	private :

	/** Constructeur de copie et opérateur = : interdits. */
	QtPluginRunnerDialog (const QtPluginRunnerDialog&);
	QtPluginRunnerDialog& operator = (const QtPluginRunnerDialog&);

	/** Le plugin à exécuter. */
	PLUGIN::Plugin*					_plugin;

	/** Section parente de la section de sauvegarde des paramètres du plugin,
	 * à des fins de sauvegarde. */
	IN_PREFS Section*				_pluginParentSection;

	/** Copie temporaire des paramètres du plugin à des fins de sauvegarde. */
	IN_PREFS Section*				_safeGuardSection;

	/** La version de cette bibliothèque. */
	static const IN_UTIL Version	_version;
};	// class QtPluginRunnerDialog

#endif	// QT_PLUGIN_RUNNER_DIALOG

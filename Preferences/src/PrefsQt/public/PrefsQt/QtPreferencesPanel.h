#ifndef QT_PREFERENCES_PANEL_H
#define QT_PREFERENCES_PANEL_H

#include <PrefsCore/prefs_config.h>
#include <PrefsCore/Section.h>

// MOC_SKIP_BEGIN
#ifndef QT_5
#include <QtGui/QTreeWidget>
#else	// QT_5
#include <QTreeWidget>
#endif	// QT_5
// MOC_SKIP_END

#include <string>


/**
 * Panneau Qt permettant d'éditer une configuration.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
// MOC_SKIP_BEGIN
class QtPreferencesPanel : public QTreeWidget
// MOC_SKIP_END
{
	Q_OBJECT


	public :

	/** Constructeur
	 * @param		widget parent. Ne doit pas être nul.
	 * @param		Section à éditer. Cette section sera détruite
	 *				par le destructeur si releaseSection n'a pas été
	 *				appelé.
	 * @param		true si le panneau est éditable, sinon false.
	 * @param		titre de l'application (pour les messages d'erreur).
	 */
	QtPreferencesPanel (QWidget* parent, IN_PREFS Section* section,
	                    bool editable, const IN_STD string& appTitle);

	/** 
	 * Destructeur. Détruit la section gérée si celle-ci n'a pas
	 * été récupérée par releaseSection.
	 */
	virtual ~QtPreferencesPanel ( );

	/**
	 * @return		true si une configuration est chargée, sinon false.
	 */
	virtual bool hasSection ( ) const;

	/**
	 * @return		Une référence sur la section racine de la 
	 *				configuration chargée.
	 * @exception	Une exception est levée s'il n'y a pas de configuration
	 *				chargée.
	 */
	virtual const IN_PREFS Section& getSection ( ) const;

	/**
	 * @return		la section gérée et vide le panneau de la
	 *				configuration en cours.
	 */
	virtual IN_PREFS Section* releaseSection ( );

	/**
	 * Affiche la configuration reçue en argument, après avoir détruit
	 * l'ancienne.
	 * @exception	En cas d'erreur.
	 */
	virtual void setRootSection (IN_PREFS Section* section);

	/**
	 * Ajoute une section à la section sélectionnéee.
	 */
	virtual void insertSection ( );

	/**
	 * Ajoute une valeur nommée àa section sélectionnée.
	 */
	virtual void insertBooleanNamedValue ( );
	virtual void insertColorNamedValue ( );
	virtual void insertStringNamedValue ( );
	virtual void insertLongNamedValue ( );
	virtual void insertUnsignedLongNamedValue ( );
	virtual void insertDoubleNamedValue ( );
	virtual void insertDoubleTripletNamedValue ( );

	/**
	 * Supprime la sélection courrante.
	 */
	virtual void removeSelection ( );

	/**
	 * Affiche une boite de dialogue de consultation/modifiction des
	 * commentaires associés à l'élément sélectionné.
	 */
	virtual void editComment ( );

	/**
	 * @return		true si la configuration est éditable, sinon false.
	 */
	virtual bool isEditable ( ) const
	{ return _editable; }


	/**
	 * @return		Le premier item de la sélection.
	 */
// MOC_SKIP_BEGIN
	virtual QTreeWidgetItem* selectedItem ( );
// MOC_SKIP_END


	public slots :

	/**
	 * Appelé lorsque le nom ou la valeur d'un élément vient d'être
	 * modifié par l'utilisateur. Invoque nameModified, valueModified ou
	 * commentModified de l'item concerné.
	 * @param		Item concerné.
	 * @param		Colonne concernée.
	 */
// MOC_SKIP_BEGIN
	virtual void elementRenamed (QTreeWidgetItem* item, int col);
// MOC_SKIP_END


	protected :

	/**
	 * Affiche le message d'erreur reçu en argument dans une boite de 
	 * dialogue.
	 */
	virtual void displayErrorMessage (const IN_UTIL UTF8String& message);


	private :

	/** Constructeur de copie. Interdit. */
	QtPreferencesPanel (const QtPreferencesPanel&);

	/** Opérateur de copie. Interdit. */
	QtPreferencesPanel& operator = (const QtPreferencesPanel&);

	/** La section gérée. */
	IN_PREFS Section*		_mainSection;

	/** La configuration est elle éditable ? */
	bool					_editable;

	/** Le titre de l'application. */
	IN_STD string			_appTitle;
};	// class QtPreferencesPanel



#endif	// QT_PREFERENCES_PANEL_H

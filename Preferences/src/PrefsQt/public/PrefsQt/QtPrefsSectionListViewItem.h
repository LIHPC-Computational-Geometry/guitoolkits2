#ifndef QT_PREFS_SECTION_LIST_VIEW_ITEM_H
#define QT_PREFS_SECTION_LIST_VIEW_ITEM_H

#include <PrefsCore/prefs_config.h>
#include <PrefsQt/QtPrefsListViewItem.h>
#include <PrefsCore/Section.h>

#include <string>


/**
 * Classe d'item de liste représentant une section de configuration.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class QtPrefsSectionListViewItem : public QtPrefsListViewItem
{
	public :

	/**
	 * Constructeur. La fonction createChildren doit être appellée
	 * après.
	 * @param		Parent.
	 * @param		Section gérée. Ne doit pas être nulle.
	 * @param		true si l'élément est éditable, sinon false.
	 */
// MOC_SKIP_BEGIN
	QtPrefsSectionListViewItem (QTreeWidget* parent, 
	                            IN_PREFS Section* section, bool editable);
// MOC_SKIP_END

	/**
	 * Constructeur. La fonction createChildren doit être appellée
	 * après.
	 * @param		Parent.
	 * @param		Section gérée. Ne doit pas être nulle.
	 * @param		true si l'élément est éditable, sinon false.
	 */
	QtPrefsSectionListViewItem (QtPrefsSectionListViewItem* parent, 
	                            IN_PREFS Section* section, bool editable);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtPrefsSectionListViewItem ( );

	/**
	 * Vérifie que le nom saisi par l'utilisateur est valide. Si oui,
	 * l'élément associé est renommé.
	 * @exception	Lève une exception en cas d'erreur.
	 */
	virtual void nameModified ( );

	/**
	 * @return		La section gérée.
	 */
	virtual IN_PREFS Section& getSection ( ) const;

	/**
	 * Appelé lors de la construction. Créé sa descendance.
	 * @exception	Une exception peut être levée, par exemple si un
	 * élément de la section est de type inconnu.
	 */
	virtual void createChildren ( );


	private :

	/**
	 * Constructeur de copie. Interdit.
	 */
	QtPrefsSectionListViewItem (const QtPrefsSectionListViewItem&);

	/**
	 * Opérateur =. Interdit.
	 */
	QtPrefsSectionListViewItem& operator = (
					const QtPrefsSectionListViewItem&);
};	// class QtPrefsSectionListViewItem

#endif	// QT_PREFS_SECTION_LIST_VIEW_ITEM_H

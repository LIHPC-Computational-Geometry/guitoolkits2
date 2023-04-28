#ifndef QT_PREFS_NAMED_VALUE_LIST_VIEW_ITEM_H
#define QT_PREFS_NAMED_VALUE_LIST_VIEW_ITEM_H

#include <PrefsCore/prefs_config.h>
#include <PrefsQt/QtPrefsListViewItem.h>
#include <PrefsQt/QtPrefsSectionListViewItem.h>
#include <PrefsCore/NamedValue.h>

#include <string>


/**
 * Classe d'item de liste représentant une valeur nommée de 
 * configuration.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class QtPrefsNamedValueListViewItem : public QtPrefsListViewItem
{
	public :

	/**
	 * Constructeur.
	 * @param		Parent.
	 * @param		NamedValue gérée. Ne doit pas être nulle.
	 * @param		true si l'élément est éditable, sinon false.
	 */
	QtPrefsNamedValueListViewItem (QtPrefsSectionListViewItem* parent, 
	                               IN_PREFS NamedValue* section, bool editable);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtPrefsNamedValueListViewItem ( );

	/**
	 * Vérifie que la valeur saisie par l'utilisateur est valide. Si oui,
	 * la valeur de l'élément associé est mise à jour.
	 * @exception	Lève une exception en cas d'erreur.
	 */
	virtual void valueModified ( );

	/**
	 * @return		La valeur nommée gérée.
	 */
	virtual IN_PREFS NamedValue& getNamedValue ( ) const;


	private :

	/**
	 * Constructeur de copie. Interdit.
	 */
	QtPrefsNamedValueListViewItem (const QtPrefsNamedValueListViewItem&);

	/**
	 * Opérateur =. Interdit.
	 */
	QtPrefsNamedValueListViewItem& operator = (
					const QtPrefsNamedValueListViewItem&);
};	// class QtPrefsNamedValueListViewItem

#endif	// QT_PREFS_NAMED_VALUE_LIST_VIEW_ITEM_H

#ifndef QT_PREFS_LIST_VIEW_ITEM_H
#define QT_PREFS_LIST_VIEW_ITEM_H

#include <PrefsCore/prefs_config.h>
#include <PrefsCore/Element.h>

#include <string>

#include <QValidator>
#ifndef QT_5
#include <QtGui/QTreeWidget>
#else	// QT_5
#include <QTreeWidget>
#endif	// QT_5


class QtPrefsSectionListViewItem;


/**
 * Classe abstraite de QListViewItem représentant un élément de configuration.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
// MOC_SKIP_BEGIN
class QtPrefsListViewItem : public QTreeWidgetItem
// MOC_SKIP_END
{
	public :

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtPrefsListViewItem ( );

	/**
	 * @return		L'élément géré.
	 */
	virtual IN_PREFS Element& getElement ( ) const;

	/**
	 * Vérifie que le nom saisi par l'utilisateur est valide. Si oui, l'élément associé est renommé.
	 * @exception	Lève une exception en cas d'erreur.
	 */
	virtual void nameModified ( );

	/**
	 * @param		Nouveau commentaire associé à l'élément
	 * @exception	Lève une exception en cas d'erreur.
	 */
	virtual void setComment (const IN_UTIL UTF8String& comment);

	/**
	 * Modifie le commentaire associé à l'élément conformément à la saisie de l'utilisateur.
	 * @exception	Lève une exception en cas d'erreur.
	 */
	virtual void commentModified ( );

	/**
	 * @return		true si l'élément peut être modifié, sinon false.
	 */
	virtual bool isEditable ( ) const
	{ return _editable; }

	/**
	 * @return		Validateur de nom d'éléments à utiliser.
	 */
	virtual QValidator* getNameValidator ( ) const;

	/**
	 * Détruit le validateur précédant s'il n'a pas de parent.
	 * @param		Validateur de nom d'éléments à utiliser. Sera détruit par cette instance s'il n'a pas de parent. Peut être nul
	 *				(=> absence de validation).
	 */
	virtual void setNameValidator (QValidator* validator);

	/**
	 * Ajoute l'item reçu en argument à la liste de ses enfants.
	 */
// MOC_SKIP_BEGIN
	virtual void insertItem (QTreeWidgetItem* item)
	{ addChild (item); }
// MOC_SKIP_END


	protected :

	/**
	 * Constructeur.
	 * @param		Parent.
	 * @param		Elément géré. Ne doit pas être nul.
	 * @param		true si l'élément est éditable, sinon false.
	 */
// MOC_SKIP_BEGIN
	QtPrefsListViewItem (QTreeWidget* parent, IN_PREFS Element* element,
	                     bool editable = true);
// MOC_SKIP_END

	/**
	 * Constructeur.
	 * @param		Parent.
	 * @param		Elément géré. Ne doit pas être nul.
	 * @param		true si l'élément est éditable, sinon false.
	 */
	QtPrefsListViewItem (QtPrefsSectionListViewItem* parent, 
	                     IN_PREFS Element* element, bool editable = true);


	private :

	/**
	 * Constructeur de copie. Interdit.
	 */
	QtPrefsListViewItem (const QtPrefsListViewItem&);

	/**
	 * Opérateur =. Interdit.
	 */
	QtPrefsListViewItem& operator = (const QtPrefsListViewItem&);

	/** L'élément géré. */
	IN_PREFS Element*		_element;

	/** Caractère éditable de l'élément. */
	bool					_editable;

	/** Le validateur utilisé pour vérifier les noms saisis. */
	QValidator*				_nameValidator;
};	// class QtPrefsListViewItem

#endif	// QT_PREFS_LIST_VIEW_ITEM_H

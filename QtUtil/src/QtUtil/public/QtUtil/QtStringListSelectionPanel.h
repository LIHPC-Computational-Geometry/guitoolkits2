#ifndef QT_STRING_LIST_SELECTION_PANEL_H
#define QT_STRING_LIST_SELECTION_PANEL_H

#include <TkUtil/util_config.h>
#include <QtUtil/QtGroupBox.h>

// MOC_SKIP_BEGIN
#ifndef QT_5
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#else	// QT_5
#include <QListWidget>
#include <QPushButton>
#endif	// QT_5
// MOC_SKIP_END

#include <vector>
#include <string>


/**
 * Panneau Qt permettant la sélection d'une liste de chaines de 
 * caractères.
 */
class QtStringListSelectionPanel : public QtGroupBox 
{
	Q_OBJECT

	public :

	/** Constructeur
	 * @param		widget parent. Ne doit pas être nul.
	 * @param		liste des chaines de caractères non sélectionnées
	 * @param		liste des chaines de caractères sélectionnées
	 */
	QtStringListSelectionPanel (QWidget* parent, 
	                            const IN_STD vector <IN_STD string>& unselected,
	                            const IN_STD vector <IN_STD string>& selected);

	/** Destructeur. */
	virtual ~QtStringListSelectionPanel ( );

	/** 
	 * @return			la liste des chaines de caracteres sélectionnées.
	 */
	virtual IN_STD vector <IN_STD string> getSelection ( ) const;


	public slots :

	/** Transfert les chaines selectionnées de la première liste à 
	 * la seconde liste.
	 */
	virtual void addStringsCallback ( );

	/** Transfert les chaines selectionnées de la seconde liste à la
	 * première liste.
	 */
	virtual void removeStringsCallback ( );

	/**
	 * Transfert la chaine de caractères sélectionnée par double-clic
	 * de la première liste vers la seconde.
	 */
// MOC_SKIP_BEGIN
	virtual void addStringCallback (QListWidgetItem* item);
// MOC_SKIP_END

	/**
	 * Transfert la chaine de caractères sélectionnée par double-clic
	 * de la seconde liste vers la première.
	 */
// MOC_SKIP_BEGIN
	virtual void removeStringCallback (QListWidgetItem* item);
// MOC_SKIP_END


	protected :


	private :

	/** Constructeur de copie. Interdit. */
	QtStringListSelectionPanel (const QtStringListSelectionPanel&);

	/** Opérateur de copie. Interdit. */
	QtStringListSelectionPanel& operator = (const QtStringListSelectionPanel&);

	/** La liste des chaines de caractères non sélectionnées. */
// MOC_SKIP_BEGIN
	QListWidget*			_unselectedListView;
// MOC_SKIP_END

	/** La liste des chaines de caractères sélectionnées. */
// MOC_SKIP_BEGIN
	QListWidget*			_selectedListView;
// MOC_SKIP_END

	/** Les boutons "Sélectionner" et "Désélectionner". */
	QPushButton				*_addButton, *_removeButton;
};	// class QtStringListSelectionPanel



#endif	// QT_STRING_LIST_SELECTION_PANEL_H

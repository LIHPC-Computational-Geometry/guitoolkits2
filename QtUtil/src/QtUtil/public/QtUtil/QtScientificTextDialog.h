// NO_QT3_MOC  Tag utilisé par le filtre MocFilter.py

#ifndef QT_SCIENTIFIC_TEXT_DIALOG_H
#define QT_SCIENTIFIC_TEXT_DIALOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/UTF8String.h>
#include <QtUtil/QtTextDialog.h>

#ifndef QT_5
#include <QtGui/QPushButton>
#else	// QT_5
#include <QPushButton>
#endif	// QT_5


class QtGreekPushButton;

/**
 * Classe de boite de dialogue modale de saisie d'une page de texte
 * scientifique, à savoir pouvant contenir des caractères grecques dans la
 * version courante.
 */
class QtScientificTextDialog : public QtTextDialog
{
	Q_OBJECT

	friend class QtGreekPushButton;	// Ugly !

	public	:

	/**
	 * Constructeur
	 * @param		Widget parent
	 * @param		Titre de la fenêtre
	 * @param		Texte initial.
	 * @param		true si le texte est éditable, sinon false.
	 */
	QtScientificTextDialog (
					QWidget* parent, const IN_UTIL UTF8String& title,
					const IN_UTIL UTF8String& text, bool editable);

	/**
	 * Constructeur
	 * @param		Widget parent
	 * @param		Titre de la fenêtre
	 * @param		Texte initial.
	 * @param		true si le texte est éditable, sinon false.
	 * @param		Fonte utilisée
	 * @param		Couleur de la fonte
	 * @param		Couleur d'arrière plan
	 */
	QtScientificTextDialog (
					QWidget* parent, const IN_UTIL UTF8String& title,
					const IN_UTIL UTF8String& text, bool editable,
					const QFont& font, const QColor& foreground = Qt::black,
					const QColor& background = Qt::white);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtScientificTextDialog ( );


	protected slots :

	/**
	 * Appellé lorsque d'un changement du texte.
	 */
	virtual void contentsChange (int pos, int charsRemoved, int charsAdded);


	private	:

	/**
	 * Constructeur de copie : interdit.
	 */
	QtScientificTextDialog (const QtScientificTextDialog&);

	/**
	 * Opérateur = : interdit.
	 */
	QtScientificTextDialog& operator = (const QtScientificTextDialog&);

	/**
	 * Créé l'IHM.
	 * @param		<I>true</I> si le texte est modifiable, <I>false</I> dans
	 *				le ca contraire.
	 */
	void createGui (bool editable);

	/** L'alphabet grec. */
	QWidget*			_greekPanel;
};	// class QtTextDialog


/*
 * Usage interne à QtUtil.
 */
class QtGreekPushButton : public QPushButton
{
	Q_OBJECT

    public :

    QtGreekPushButton (
		QWidget* parent, const QString& character, QtScientificTextDialog& dlg);


    private slots :

	virtual void insertCharacter (bool checked);


	private :

	QtScientificTextDialog&	_dialog;
};  // class QtGreekPushButton

#endif	// QT_SCIENTIFIC_TEXT_DIALOG_H


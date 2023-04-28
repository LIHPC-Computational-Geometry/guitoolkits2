#ifndef QT_EXT_MESSAGE_BOX_H
#define QT_EXT_MESSAGE_BOX_H

#ifndef QT_5
#include <QtGui/QDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>
#else	// QT_5
#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QCheckBox>
#endif	// QT_5
#include <TkUtil/UTF8String.h>


/**
 * Widget type "MessageBox" contenant une case à cocher "Ne plus poser cette
 * question".
 * La méthode exec retourne :
 * - 1 si le premier bouton a été activé,
 * - 2 si le second bouton a été activé,
 * - 3 si le second bouton a été activé.
 */
class QtExtMessageBox : public QDialog 
{
	Q_OBJECT

	public :

	/** Constructeur 1 - Chaînes ASCII/ISO 8859.
	 * @param		titre de l'application
	 * @param		parent
	 * @param		texte à afficher
	 * @param		libellé du premier bouton (ou chaine nulle)
	 * @param		libellé du second bouton (ou chaine nulle)
	 * @param		libellé du troisième bouton (ou chaine nulle)
	 * @param		libellé de la case à cocher (ou chaine nulle)
	 * @param		icône
	 * @param		indice du bouton par défaut (premier indice : 1)
	 * @param		valeur par défaut de la case à cocher
	 */
	QtExtMessageBox (const char* title,
	                 QWidget* parent,
	                 const char* text,
	                 const char* button1,
	                 const char* button2,
	                 const char* button3,
	                 const char* checkBox,
	                 QMessageBox::Icon icon = QMessageBox::Information,
	                 int defaultButton = 1,
	                 bool defaultCheckBox = false);

	/** Constructeur 2- Chaînes UTF.
	 * @param		titre de l'application
	 * @param		parent
	 * @param		texte à afficher
	 * @param		libellé du premier bouton (ou chaine vide)
	 * @param		libellé du second bouton (ou chaine vide)
	 * @param		libellé du troisième bouton (ou chaine vide)
	 * @param		libellé de la case à cocher (ou chaine vide)
	 * @param		icône
	 * @param		indice du bouton par défaut (premier indice : 1)
	 * @param		valeur par défaut de la case à cocher
	 */
	QtExtMessageBox (const IN_UTIL UTF8String& title,
	                 QWidget* parent,
	                 const IN_UTIL UTF8String& text,
	                 const IN_UTIL UTF8String& button1,
	                 const IN_UTIL UTF8String& button2,
	                 const IN_UTIL UTF8String& button3,
	                 const IN_UTIL UTF8String& checkBox,
	                 QMessageBox::Icon icon = QMessageBox::Information,
	                 int defaultButton = 1,
	                 bool defaultCheckBox = false);

	/** Destructeur.
	 */
	virtual ~QtExtMessageBox ( );

	/**
	 * @return		true (case à cocher cochée) ou false dans le cas
	 *				contraire).
	 */
	virtual bool getCheckBoxValue ( ) const;


	protected :

	/** Créé le panneau.
	 * @param		texte à afficher
	 * @param		libellé du premier bouton (ou chaine vide)
	 * @param		libellé du second bouton (ou chaine vide)
	 * @param		libellé du troisième bouton (ou chaine vide)
	 * @param		libellé de la case à cocher (ou chaine vide)
	 * @param		icône
	 * @param		indice du bouton par défaut (premier indice : 1)
	 * @param		valeur par défaut de la case à cocher
	 */
	virtual void createGui (
	                 const IN_UTIL UTF8String& text,
	                 const IN_UTIL UTF8String& button1,
	                 const IN_UTIL UTF8String& button2,
	                 const IN_UTIL UTF8String& button3,
	                 const IN_UTIL UTF8String& checkBox,
	                 QMessageBox::Icon icon,
	                 int defaultButton,
	                 bool defaultCheckBox);

	/** @return		la case à cocher. */
	const QCheckBox* getCheckBox ( ) const
	{ return _checkBox; }
	QCheckBox* getCheckBox ( )
	{ return _checkBox; }


	protected slots :

	/** Appelé lorsque l'utilisateur clique sur un bouton. Invoque done
	 * avec l'indice du bouton.
	 */
	virtual void buttonClicked ( );


	private :

	/** Constructeur de copie. Interdit. */
	QtExtMessageBox (const QtExtMessageBox&);

	/** Opérateur de copie. Interdit. */
	QtExtMessageBox& operator = (const QtExtMessageBox&);

	/** Les boutons. */
	QPushButton*				_buttons [3];

	/** La case à cocher. */
	QCheckBox*				_checkBox;
};	// class QtExtMessageBox



#endif	// QT_EXT_MESSAGE_BOX_H

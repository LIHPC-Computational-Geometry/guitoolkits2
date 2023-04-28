// NO_QT3_MOC  Tag utilisé par le filtre MocFilter.py

#ifndef QT_MESSAGE_BOX_H
#define QT_MESSAGE_BOX_H

#include <TkUtil/UTF8String.h>

#ifndef QT_5
#include <QtGui/QWidget>
#include <QtGui/QMessageBox>
#else	// QT_5
#include <QWidget>
#include <QMessageBox>
#endif	// QT_5



/**
 * Fonctions permettant d'afficher une boite de dialogue contenant un
 * message.
 */
class QtMessageBox
{
	public :

	/**
	 * Boite de dialogue modale affichant un message d'information. Formate le
	 * message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 */
	static void displayInformationMessage (
							QWidget* parent,
							const IN_UTIL UTF8String& title,
							const IN_UTIL UTF8String& message,
							size_t columnNum = 100);

	/**
	 * Boite de dialogue modale affichant un message d'information. Formate le
	 * message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 * @param		Libellés des boutons
	 * @param		Bouton actif par défaut
	 * @return		L'identifiant du bouton ayant provoqué la femeture de la
	 *				boite de dialogue.
	 */
	static int displayInformationMessage (
						QWidget* parent,
						const IN_UTIL UTF8String& title,
						const IN_UTIL UTF8String& message, size_t columnNum,
						const char* button0Text = 0,
						const char* button1Text = 0,
						const char* button2Text = 0,
						int defaultButtonNumber = 0
					);

	/**
	 * Boite de dialogue modale affichant un message d'avertissement. Formate le
	 * message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 */
	static void displayWarningMessage (
							QWidget* parent,
							const IN_UTIL UTF8String& title,
							const IN_UTIL UTF8String& message,
							size_t columnNum = 100);

	/**
	 * Boite de dialogue modale affichant un message d'avertissement. Formate le
	 * message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Libellés des boutons
	 * @param		Bouton actif par défaut
	 * @param		Nombre de colonnes du message.
	 * @return		L'identifiant du bouton ayant provoqué la femeture de la
	 *				boite de dialogue.
	 */
	static int displayWarningMessage (
						QWidget* parent,
						const IN_UTIL UTF8String& title,
						const IN_UTIL UTF8String& message, size_t columnNum,
						const char* button0Text = 0,
						const char* button1Text = 0,
						const char* button2Text = 0,
						int defaultButtonNumber = 0
					);

	/**
	 * Boite de dialogue modale affichant un message d'erreur. Formate le
	 * message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 * @return		L'identifiant du bouton ayant provoqué la femeture de la
	 *				boite de dialogue.
	 */
	static void displayErrorMessage (
							QWidget* parent,
							const IN_UTIL UTF8String& title,
							const IN_UTIL UTF8String& message,
							size_t columnNum = 100);

	/**
	 * Boite de dialogue modale affichant un message d'erreur. Formate le
	 * message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 * @param		Libellés des boutons
	 * @param		Bouton actif par défaut
	 * @return		L'identifiant du bouton ayant provoqué la femeture de la
	 *				boite de dialogue.
	 */
	static int displayErrorMessage (
						QWidget* parent,
						const IN_UTIL UTF8String& title,
						const IN_UTIL UTF8String& message, size_t columnNum,
						const char* button0Text = 0,
						const char* button1Text = 0,
						const char* button2Text = 0,
						int defaultButtonNumber = 0
					);

	/**
	 * Boite de dialogue modale affichant une question. Formate le
	 * message avant affichage.
	 * @param		Widget parent
	 * @param		Titre de la boite de dialogue.
	 * @param		Message à afficher.
	 * @param		Nombre de colonnes du message.
	 * @param		Libellés des boutons
	 * @param		Bouton actif par défaut
	 * @return		L'identifiant du bouton ayant provoqué la femeture de la
	 *				boite de dialogue.
	 */
	static int displayQuestionMessage (
						QWidget* parent,
						const IN_UTIL UTF8String& title,
						const IN_UTIL UTF8String& message, size_t columnNum,
						const char* button0Text = 0,
						const char* button1Text = 0,
						const char* button2Text = 0,
						int defaultButtonNumber = 0
					);


	private :

	/** Constructeurs et opérateur = : interdits.
	 */
	QtMessageBox ( );
	QtMessageBox (const QtMessageBox&);
	QtMessageBox& operator = (const QtMessageBox&);

	/**
	 * Destructeur : interdit.
	 */
	~QtMessageBox ( );
};	// class QtMessageBox



#ifndef QT_5
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#else	// QT_5
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#endif	// QT_5


class QtMessageDialog : public QDialog
{
	Q_OBJECT

	public :

	QtMessageDialog (QMessageBox::Icon icon, QWidget* parent,
				const QString& title, const QString& text, int columns,
				const char* button1, const char* button2, const char* button3,
				int defaultButton = 0);

	virtual ~QtMessageDialog ( );


	protected slots :

	virtual void buttonClicked ( );


	private :

	QtMessageDialog (const QtMessageDialog&);
	QtMessageDialog& operator = (const QtMessageDialog&);

	QPushButton*			_buttons [3];
};	// class QtMessageDialog



#endif	// QT_MESSAGE_BOX_H

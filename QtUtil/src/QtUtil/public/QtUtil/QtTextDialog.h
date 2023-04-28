#ifndef QT_TEXT_DIALOG_H
#define QT_TEXT_DIALOG_H

#include <TkUtil/util_config.h>
#include <TkUtil/UTF8String.h>
#include <QtUtil/QtDlgClosurePanel.h>

#ifndef QT_5
#include <QtGui/QDialog>
#include <QtGui/QLayout>
#include <QtGui/QTextEdit>
#else	// QT_5
#include <QDialog>
#include <QLayout>
#include <QTextEdit>
#endif	// QT_5


/** Classe de boite de dialogue modale de saisie d'une page de texte.
 */
class QtTextDialog : public QDialog
{
	Q_OBJECT

	public	:

	/**
	 * Constructeur
	 * @param		Widget parent
	 * @param		Titre de la fenêtre
	 * @param		Texte initial.
	 * @param		true si le texte est éditable, sinon false.
	 */
	QtTextDialog (QWidget* parent, const IN_UTIL UTF8String& title, 
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
	QtTextDialog (QWidget* parent, const IN_UTIL UTF8String& title, 
	              const IN_UTIL UTF8String& text, bool editable,
	              const QFont& font, const QColor& foreground = Qt::black,
	              const QColor& background = Qt::white);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtTextDialog ( );

	/**
	 * @return		Le texte affiché.
	 */
	IN_UTIL UTF8String getText ( ) const;

	/**
	 * @return		Le texte affiché.
	 */
	QString getQText ( ) const;

	/**
	 * @return		Le gestionnaire de mise en forme central.
	 */	
	const QVBoxLayout&	getCentralLayout ( ) const;
	QVBoxLayout&	getCentralLayout ( );

	/**
	 * @return		L'éditeur de texte.
	 */
	const QTextEdit& getTextEditor ( ) const;
	QTextEdit& getTextEditor ( );


	signals	:

	/**
	 * Envoyé lorsque le bouton "Appliquer" est cliqué.
	 * @param		l'instance siège de l'évènement.
	 */
	void applied (QtTextDialog* dialog);

	/**
	 * Envoyé lorsque le bouton "Annuler" est cliqué.
	 * @param		l'instance siège de l'évènement.
	 */
	void canceled (QtTextDialog* dialog);


	protected slots :

	/**
	 * Appellé lorsque d'un changement du texte.
	 */
	virtual void textModified ( );

	/**
	 * Appelé quand l'utilisateur appuie sur le bouton "Appliquer".
	 * Invoque Dialog::accept. Envoie le signal "applied" avec en argument 
	 * cette instance.
	 */
	virtual void apply ( );

	/**
	 * Appelé quand l'utilisateur appuie sur le bouton "Annuler".
	 * Invoque Dialog::accept. Envoie le signal "canceled" avec en argument 
	 * cette instance.
	 */
	virtual void reject ( );


	private :

	/** 
	 * Constructeur de copie : interdit.
	 */
	QtTextDialog (const QtTextDialog&);

	/** 
	 * Opérateur = : interdit.
	 */
	QtTextDialog& operator = (const QtTextDialog&);

	/** Créé l'IHM.
	 * @param		Titre de la fenêtre
	 * @param		Texte initial.
	 * @param		true si le texte est éditable, sinon false.
	 */
	void createGui (const IN_UTIL UTF8String& title, 
	                const IN_UTIL UTF8String& text, bool editable);

	/** Le gestionnaire de mise en forme central. */
	QVBoxLayout*						_centralLayout;

	/**	
	 * La fenêtre contenant le texte.
	 */
	QTextEdit*							_textWindow;

	/**
	 * Le panneau contenant les boutons "OK" / "Annuler".
	 */
	QtDlgClosurePanel*					_closurePanel;

	/**
	 * Le text a-t-il été modifié ? 
	 */
	bool								_modified;
};	// class QtTextDialog

#endif	// QT_TEXT_DIALOG_H

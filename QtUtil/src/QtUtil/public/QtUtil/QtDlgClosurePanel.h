#ifndef QT_CLOSURE_PANEL_H
#define QT_CLOSURE_PANEL_H

#include <TkUtil/util_config.h>

#ifndef QT_5
#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#else	// QT_5
#include <QWidget>
#include <QPushButton>
#endif	// QT_5

#include <string>


/**
 * Panneau Qt contenant, sur une même ligne, les boutons "Appliquer",
 * "Fermer", "Annuler". Ce panneau peut également comporté un bouton
 * "aide" reposant sur l'API QtHelpWindow.
 */
class QtDlgClosurePanel : public QWidget
{
	Q_OBJECT


	public :

	/** Constructeur. Un bouton "aide" est créé si helpURL est non nulle.
	 * Le bouton "Annuler" n'est créé que si le texte de button3 n'est pas vide.
	 * @param		widget parent. Ne doit pas être nul.
	 * @param		Si true le panneau aura un bouton "Fermer". Dans le cas
	 *				contraire, il n'aura que les boutons "Appliquer" et
	 *				"Annuler".
	 * @param		Intitulé du premier bouton.
	 * @param		Intitulé du second bouton.
	 * @param		Intitulé du troisième bouton.
	 * @param		URL de l'aide
	 * @param		Balise de l'aide
	 */
	QtDlgClosurePanel (QWidget* parent, bool hasClose = true,
	                   const IN_STD string& button1 = "Appliquer",
	                   const IN_STD string& button2 = "Fermer",
	                   const IN_STD string& button3 = "Annuler",
                       const IN_STD string& helpURL = "",
                       const IN_STD string& helpTag = "");

	/** Destructeur. */
	virtual ~QtDlgClosurePanel ( );

	/**
	 * @return			le bouton "Appliquer".
	 */
	virtual QPushButton* getApplyButton ( ) const;

	/**
	 * @return			le bouton "Fermer".
	 * @warning			Peut retourner 0 (cf. constructeur).
	 */
	virtual QPushButton* getCloseButton ( ) const;

	/**
	 * @return			le bouton "Annuler".
	 */
	virtual QPushButton* getCancelButton ( ) const;

	/**
	 * @return			le bouton "Aide".
	 */
	virtual QPushButton* getHelpButton ( ) const;


	public slots :

	/** Affiche l'aide associée au panneau. */
	virtual void displayHelp ( );


	private :

	/** Constructeur de copie. Interdit. */
	QtDlgClosurePanel (const QtDlgClosurePanel&);

	/** Opérateur de copie. Interdit. */
	QtDlgClosurePanel& operator = (const QtDlgClosurePanel&);

	/** Le bouton "Appliquer". */
	QPushButton*			_applyButton;

	/** Le bouton "Fermer". */
	QPushButton*			_closeButton;

	/** Le bouton "Annuler". */
	QPushButton*			_cancelButton;

	/** Le bouton "Aide". */
	QPushButton*			_helpButton;

	/** L'URL de l'aide et son éventuelle balise associée. */
	IN_STD string			_helpURL, _helpTag;
};	// class QtDlgClosurePanel



#endif	// QT_CLOSURE_PANEL_H

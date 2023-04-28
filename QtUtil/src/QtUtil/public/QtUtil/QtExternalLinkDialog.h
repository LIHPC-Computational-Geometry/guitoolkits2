/**
 * @file		QtExternalLinkDialog.h
 * @author		Charles PIGNEROL
 * @date		04/04/2014
 */


#ifndef QT_EXTERNAL_LINK_DIALOG_H
#define QT_EXTERNAL_LINK_DIALOG_H

#ifndef QT_5
#include <QtGui/QDialog>
#else	// QT_5
#include <QDialog>
#endif	// QT_5

#include <TkUtil/UTF8String.h>


/**
 * <P>
 * Boite de dialogue modale permettant l'affichage un lien dans le navigateur 
 * internet par défaut de la session.
 * </P>
 * <P>Bogue connu (I>Qt 4.7.4</I>): si le navigateur (<I>firefox</I>)est ouvert
 * dans un autre espace de travail alors la page n'est pas affiché, mais la
 * classe considère que la mission est remplie.
 * </P>
 */
class QtExternalLinkDialog : public QDialog
{
	public :

	/**
	 * @param	Widget parent
	 * @param	URL du lien à afficher
	 * @param	Texte alternatif
	 * @param	Titre de l'application
	 */
	QtExternalLinkDialog (
			QWidget* parent, const TkUtil::UTF8String& link, const TkUtil::UTF8String& text,
			const TkUtil::UTF8String& title);

	/**
	 * Destructeur.
	 * RAS.
	 * */
	virtual ~QtExternalLinkDialog ( );

	/**
	 * Affiche l'URL transmise en argument avec le navigateur par défaut de la
	 * session de travail. En cas d'échec affiche une boite de dialogue
	 * permettant une nouvelle tentative et renseignant l'URL à afficher.
	 * @param	Widget parent
	 * @param	URL du lien à afficher
	 * @param	Texte alternatif
	 * @param	Titre de l'application
	 * @warning	Pas hyper-fiable, considère que le travail est fait si le
	 * 			navigateur est lancé, même s'il n'affiche pas la page.
	 */
	static void displayURL (QWidget* parent, const std::string& link,
	                        const std::string& text, const std::string& title);


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	QtExternalLinkDialog (const QtExternalLinkDialog&);
	QtExternalLinkDialog& operator = (const QtExternalLinkDialog&);
};	// class QtExternalLinkDialog


#endif	// QT_EXTERNAL_LINK_DIALOG_H

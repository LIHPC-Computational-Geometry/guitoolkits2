#ifndef QT_WEB_BROWSER_H
#define QT_WEB_BROWSER_H

#include <TkUtil/util_config.h>

#ifndef QT_5
#include <QtGui/QMainWindow>
#include <QtGui/QToolButton>
#else	// QT_5
#include <QMainWindow>
#include <QToolButton>
#endif	// QT_5
#ifdef USE_QT_WEBKIT
#include <QWebView>
#else	// USE_QT_WEBKIT
#ifndef QT_5
#include <QtGui/QTextBrowser>
#else	// QT_5
#ifdef USE_QT_WEBENGINE
#include <QWebEngineView>
#else
#include <QTextBrowser>
#endif	// USE_QT_WEBENGINE
#endif	// QT_5
#endif	// USE_QT_WEBKIT

#include <QPrinter>

#include <string>


/** <P>Classe de fenêtre <I>navigateur web</I> pour une application Qt.</P>
 *
 * <P>Cette classe repose sur la classe <I>QWebView</I> de <I>Qt Web kit</I>
 * si la version utilisée de <I>Qt</I> est supérieure ou égale à <I>4.7.0</I>.
 * </P>
 * @warning		Le protocole <I>http</I> n'est pas supporté, et le <I>html</I>
 *				doit être "simple".
 */
class QtWebBrowser : public QMainWindow
{
	Q_OBJECT

	public	:

	/**
	 * Constructeur
	 * @param		Titre de la fenêtre
	 */
	QtWebBrowser (const IN_STD string& title);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtWebBrowser ( );

	/**
	 * Affiche le fichier transmis en argument.
	 * @param		Nom du fichier à afficher.
	 * @param		Balise visée
	 * @exception	Lève une exception en cas d'erreur.
	 */
	virtual void setURL (const IN_STD string& url, const IN_STD string& tag);


	public slots :

	/**
	 * Imprime la page courante.
	 */
	virtual void print ( );


	protected slots :

	/**
	 * Appellé lorsque d'un changement (nouvelle URL, lien, ...).
	 * Met à jour l'URL affichée dans la barre de status.
	 * @param		Nouvelle URL
	 */
	virtual void sourceChanged (const QUrl& source);


	/**
	 * Affiche la première page de l'historique.
	 * \warning		Code disposible uniquement si la version de <I>Qt</I> est
	 *				supérieure ou égale à 4.7.0 (fonction maintenue en toutes
	 *				circonstances pour <I>moc</I>.
	 */	
	virtual void homeCallback ( );

	/**
	 * Si une requête d'affichage de page au niveau d'une balise particulière
	 * a été formulée, se positionne alors sur la balise.
	 * \warning		Code disposible uniquement si la version de <I>Qt</I> est
	 *				supérieure ou égale à 4.7.0 (fonction maintenue en toutes
	 *				circonstances pour <I>moc</I>.
	 */
	virtual void pageLoaded (bool ok);


	protected	:

	/**
	 * Créé la barre d'icônes.
	 */
	virtual void createToolBar ( );


	private	:

	/** 
	 * Constructeur de copie : interdit.
	 */
	QtWebBrowser (const QtWebBrowser&);

	/** 
	 * Opérateur = : interdit.
	 */
	QtWebBrowser& operator = (const QtWebBrowser&);

	/**	
	 * La fenêtre texte d'affichage de l'aide.
	 */
#ifdef USE_QT_WEBKIT
	QWebView*							_textBrowser;
#else	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
	QWebEngineView*						_textBrowser;
#else	// USE_QT_WEBENGINE
	QTextBrowser*						_textBrowser;
#endif	// USE_QT_WEBENGINE
#endif	// USE_QT_WEBKIT

	/** L'imprimante utilisée (semble requis avec web engine. L'impression est
	  * faite dans le même thread. */
	QPrinter*							_printer;

	/**
	 * Les boutons de la barre d'outils.
	 */
	QToolButton							*_backwardButton, *_forwardButton,
										*_homeButton, *_printButton;

#ifdef USE_QT_WEBKIT
	/**
	 * La balise à atteindre lorsque la page sera chargée.
	 * @see			pageLoaded
	 */
	std::string							_anchorTag;
#endif	// USE_QT_WEBKIT
#ifdef USE_QT_WEBENGINE
	/**
	 * La balise à atteindre lorsque la page sera chargée.
	 * @see			pageLoaded
	 */
	std::string							_anchorTag;
#endif	// USE_QT_WEBENGINE
};	// class QtWebBrowser

#endif	// QT_WEB_BROWSER_H

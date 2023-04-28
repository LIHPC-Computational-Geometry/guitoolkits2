#ifndef URL_FIFO_H
#define URL_FIFO_H

#include <TkUtil/util_config.h>

#include <deque>
#include <fstream>


BEGIN_NAMESPACE_UTIL

/**
 * <P>Classe gérant une liste de taille limitée d'URLs, par exemple pour stocker
 * la liste des 10 derniers fichiers chargés.
 * </P>
 * <P><U>Exemple d'utilisation :</U><BR><BR>
 * <CODE>
 * class MyApp
 * {
 * 		public :
 *
 *		void MyApp::load (const string& fileName);
 *
 *		private :
 *
 *		UrlFifo	_urlsFifo;
 * };	// class MyApp
 *
 * void MyApp::load (const string& fileName)
 * {
 *		ifstream	ifs (fileName);
 *		...
 *		// Recensement de cette URL parmi les dernières URLs chargées.
 *		_urlsFifo.add (fileName, true);
 *		// Sauvegarde dans un fichier dédié à cet effet :
 * 		UnicodeString	recentFiles;
 *		recentFiles << UserData ( ).getHome ( ) << "/" << ".myapp";
 * 		_urlsFifo.save (recentFiles);
 * }	// MyApp::load
 * 
 * void MyApp::createMenuFile ( )
 * {
 *		...
 *		menuFile	= new Menu ("Fichier");
 *		...
 *		// Ajout de la liste des derniers fichiers chargés :
 *		UnicodeString	recentFiles;
 *		recentFiles << UserData ( ).getHome ( ) << "/" << ".myapp";
 *		_urlsFifo.load (recentFiles);
 *		for (size_t ru = 0; ru < _urlsFifo.count ( ); ru++)
 *		{
 *			menuFile->addItem (_urlsFifo.url (ru));
 *		}	// for (size_t ru = 0; ru < _urlsFifo.count ( ); ru++)
 * }	// void MyApp::createMenuFile ( )
 * </CODE>
 * </P>
 * <P><B>Remarque :</B> l'ensemble des méthodes de cette classe, y compris le
 * constructeur, sont susceptibles de lever une exception.
 * </P>
 */
class UrlFifo
{
	public :

	/**
	 * Constructeur. Essaye de charger les URLs depuis le fichier <I>file</I>
	 * si <I>file</I> n'est pas vide.
	 * @param		Nom maximum d'URLs dans la FIFO.
	 * @param		Fichier initialisant la liste d'URLs.
	 * @see			load
	 */
	UrlFifo (size_t maxURLs = 10, const IN_STD string file = "");

	/**
	* Constructeur de copie, opérateur =. RAS.
	*/
	UrlFifo (const UrlFifo&);
	UrlFifo& operator = (const UrlFifo&);

	/**
	 * Destructeur. RAS. <B>Ne lève pas d'exception.</B>
	 */
	virtual ~UrlFifo ( );

	/**
	 * @return		La taille de la liste d'URLs.
	 */
	virtual size_t capacity ( ) const
	{ return _capacity; }

	/**
	 * @return		Le nombre d'éléments.
	 */
	virtual size_t count ( ) const
	{ return _fifo.size ( ); }

	/**
	 * Vide la FIFO puis charge le contenu de la FIFO d'URLs depuis le fichier
	 * <I>fileName</I>.
	 * @see		save
	 */
	virtual void load (const IN_STD string& fileName);

	/**
	 * Sauve le contenu de la FIFO d'URLs dans le fichier <I>fileName</I>.
	 * L'éventuel contenu de <I>fileName</I> est perdu.
	 * @see		load
	 */
	virtual void save (const IN_STD string& fileName);

	/**
	 * @return		<I>true</I> si l'URL transmise en argument est déjà
	 *				stockée.
	 */
	virtual bool has (const IN_STD string& url) const;

	/**
	 * Ajoute l'URL transmise en argument. Supprime éventuellement de la liste
	 * sa dernière URL.
	 * @param		URL a ajouter.
	 * @param		<I>true</I> si l'insertion doit se faire en tête,
	 *				<I>false</I> si elle doit être effectuée en queue.
	 * @warning		Si l'URL était déjà recensée, elle est déplacée en tête ou
	 * 				queue de liste (selon <I>atFront</I>).
	 */
	virtual void add (const IN_STD string& url, bool atFront);

	/**
	 * @return		L'URL dont l'indice est transmis en argument.
	 */
	virtual IN_STD string url (size_t i) const;


	private :

	/** Le nombre maximum d'URL dans la FIFO. */
	size_t							_capacity;

	/** La FIFO d'URLs. */
	IN_STD deque<IN_STD string>		_fifo;
};	// class UrlFifo

END_NAMESPACE_UTIL

#endif	// URL_FIFO_H

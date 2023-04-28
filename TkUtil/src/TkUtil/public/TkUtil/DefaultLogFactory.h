#ifndef DEFAULT_LOG_FACTORY_H
#define DEFAULT_LOG_FACTORY_H

#include <TkUtil/util_config.h>
#include <TkUtil/LogOutputStream.h>


BEGIN_NAMESPACE_UTIL


/**
 * Classe fabrication permettant d'instancier des flux de logs.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class DefaultLogFactory
{
	public :

	/**
	 * @param		Description du flux à créer : <I>stdout</I> (sortie
	 *				standard c++ cout), <I>stderr</I> (sortie standard c++
	 *				cerr), ou nom de fichier.
	 * @param		Dans le cas d'un fichier, true s'il doit être préalablement
	 *				vidé, false si les écritures commencent à la fin du fichier.
	 * @param		Masque appliqué.
	 * @param		true si les dates doivent être affichées
	 * @param		true si les heures doivent être affichées
	 * @return		flux créé. Sa destruction est à effectuer par l'appellant.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	static LogOutputStream* createOutputStream (
						const UTF8String& name,
						Log::TYPE mask = Log::PRODUCTION,
						bool truncate = true,
						bool enableDate = true, bool enableTime = true);


	private :

	/**
	 * Constructeur, constructeur de copie, opérateur = et destructeur :
	 * interdits.
	 */
	DefaultLogFactory ( );
	DefaultLogFactory (const DefaultLogFactory&);
	DefaultLogFactory& operator = (const DefaultLogFactory&);
	virtual ~DefaultLogFactory ( );
};	// class DefaultLogFactory



END_NAMESPACE_UTIL

#endif	// DEFAULT_LOG_FACTORY_H

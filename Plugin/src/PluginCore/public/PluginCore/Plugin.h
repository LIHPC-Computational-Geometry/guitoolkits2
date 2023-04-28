/**
 * \mainpage    Page principale de la bibliothèque <I>Plugin</I>.
 *
 * Classe permettant d'executer un plugin respectant cette API.
 * La signature des fonctions plugin de cette API est comme suit :
 * <SRC>
 * extern "C"
 * {
 * void (*function)(PluginFunction*);
 * }
 * </SRC>
 * @author		Charles PIGNEROL, CEA/DAM/DSSI, d'après
 *				Gilles GROSPELLIER et Christian LE BRETON
 */


#ifndef PLUGIN_H
#define PLUGIN_H

#include <PluginCore/PluginFunction.h>
#include <TkUtil/util_config.h>	// IN_STD
#include <TkUtil/Version.h>
#include <string>



namespace PLUGIN
{

/**
 * Classe permettant d'exécuter une fonction d'une bibliothèque que l'on 
 * charge dynamiquement.
 */
class Plugin
{
	public :


	/**
	 * Constructeur par défaut.
	 * @param		Librairie dynamique à charger.
	 * @param		Symbôle à exécuter.
	 */
	Plugin (const IN_STD string& library, const IN_STD string& symbol);

	/**
	 * Destructeur. Ferme la bibliothèque dynamique.
	 */
	virtual ~Plugin ( );

	/**
	 * @return		Le nom de la bibliothèque.
	 */
	virtual const IN_STD string& getLibraryName ( ) const
	{ return _libraryName; }

	/**
	 * Ferme la bibliothèque si elle est déjà chargée.
	 * @param		Le nom de la bibliothèque.
	 */
	virtual void setLibraryName (const IN_STD string& libraryName);

	/**
	 * @return		Le nom du symbôle à exécuter. 
	 */
	virtual IN_STD string getSymbolName ( ) const;

	/**
	 * @param		Le nom du symbôle à exécuter. 
	 */
	virtual void setSymbolName (const IN_STD string& symbolName);

	/**
	 * Exécute le plugin.
	 * @param		Lève une exception de type IN_UTIL::Exception en cas 
	 *				d'erreur.
	 * @see			getWarningMessage
	 * @see			getErrorMessage
	 */
	virtual void execute ( );

	/**
	 * @return		La chaine de caractères correspondant à un éventuel
	 *				avertissement du plugin lors de son exécution.
	 */
	virtual IN_STD string getWarningMessage ( ) const;

	/**
	 * @return		La chaine de caractères correspondant à une éventuelle
	 *				erreur du plugin lors de son exécution.
	 */
	virtual IN_STD string getErrorMessage ( ) const;

	/**
	 * @return		La fonction plugin.
	 */
	PluginFunction& getFunction ( )
	{ return _function; }
	const PluginFunction& getFunction ( ) const
	{ return _function; }

	/**
	 * Ferme la bibliothèque dynamique et réinitialise tout.
	 * @param		Lève une exception de type IN_UTIL::Exception en cas 
	 *				d'erreur.
	 */
	virtual void reset ( );

	/**
	 * @return		La version de cette bibliothèque.
	 */
	static IN_UTIL Version getVersion ( );


	protected :

	/**
	 * Ouvre la bibliothèque dynamique.
	 * @param		Lève une exception de type IN_UTIL::Exception en cas 
	 *				d'erreur.
	 */
	virtual void open ( );

	/**
	 * Ferme la bibliothèque dynamique.
	 * @param		Lève une exception de type IN_UTIL::Exception en cas 
	 *				d'erreur.
	 */
	virtual void close ( );

	/**
	 * Le handle sur la bibliotèque.
	 */
	typedef void*	LibHandleType;
	LibHandleType getLibraryHandle ( )
	{ return _libraryHandle; }
	

	private :

	/** Constructeurs de copie et opérateur = : interdits. */
	Plugin (const Plugin&);
	Plugin& operator = (const Plugin&);

	/**
	 * Le nom de la bibliothèque dynamique.
	 */
	IN_STD string					_libraryName;

	/** Le handle sur la bibliotèque. */
	LibHandleType					_libraryHandle;

	/** La fonction plugin et ses arguments. */
	PluginFunction					_function;
};	// Plugin

};	// namespace PLUGIN

#endif	// PLUGIN_H

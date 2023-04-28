#ifndef PROCESS_MEMORY_H
#define PROCESS_MEMORY_H

#include <TkUtil/util_config.h>

#include <malloc.h>
#include <string>


BEGIN_NAMESPACE_UTIL


/**
 * Classe permettant d'obtenir des informations sur la mémoire utilisée
 * par le processus.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class ProcessMemory
{
	public :

	/**
	 * Constructeur par défaut. Appelle reset.
	 */
	ProcessMemory ( ) ;

	/**
	 * Constructeur de copie. RAS.
	 */
	ProcessMemory (const ProcessMemory& mem);

	/**
	 * Opérateur =. RAS.
	 */
	ProcessMemory& operator = (const ProcessMemory& mem);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~ProcessMemory ( );

	/**
	 * Récupère des informations sur l'état de la mémoire du
	 * processus par <B>mallinfo</B>.
	 */
	virtual void reset ( );

	/**
	 * @return		La mémoire totale utilisée par le processus (mémoire
	 *				"ordinaire" + mémoire pour blocs de taille importante).
	 * @see			getFastMemorySpace
	 * @see			getMappedMemorySpace
	 */
	virtual unsigned long getTotalSpace ( ) const;

	/**
	 * @return		La mémoire "ordinaire" totale utilisée par le processus.
	 * @see			getTotalSpace
	 * @see			getMappedMemorySpace
	 */
	virtual unsigned long getFastMemorySpace ( ) const;

	/**
	 * @return		La mémoire  pour blocs de taille importante utilisée par le
	 *				processus.
	 * @see			getTotalSpace
	 * @see			getFastMemorySpace
	 */
	virtual unsigned long getMappedMemorySpace ( ) const;

	/**
	 * @return		La mémoire (ordinaire) utilisée par le processus.
	 */
	virtual unsigned long getUsedSpace ( ) const;

	/**
	 * @return		La mémoire (ordinaire) inutilisée par le processus.
	 */
	virtual unsigned long getFreeSpace ( ) const;

	/**
	 * @return		Un rapport contenant l'état de la mémoire du
	 *				processus.
	 */
	virtual IN_STD string getReport ( ) const;


	private :

	/** Informations sur l'état de la mémoire. */
	struct mallinfo		_mallinfo;
};	// class ProcessMemory


END_NAMESPACE_UTIL


#endif	// PROCESS_MEMORY_H

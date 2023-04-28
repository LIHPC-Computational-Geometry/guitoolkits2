#ifndef MEMORY_ERROR_H
#define MEMORY_ERROR_H

#include <TkUtil/util_config.h>
#include <TkUtil/LocalizedError.h>
#include <TkUtil/UTF8String.h>

#include <string>
#include <stdio.h>


BEGIN_NAMESPACE_UTIL


/**
 * Exception renseignant sur une erreur d'allocation du logiciel.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class MemoryError : public LocalizedError
{
	public :
	
	/**
	 * @param		Raison d'être de l'exception
	 * @param		Fichier où est levée l'exception
	 * @param		Ligne où est levée l'exception
	 */
	MemoryError (const UTF8String& what,
	             const IN_STD string fileName, int line)
		: LocalizedError (what, fileName, line)
	{ }

	/**
	 * Constructeur de copie et opérateur = : RAS
	 */
	MemoryError (const MemoryError& exc)
		: LocalizedError (exc)
	{ }
	MemoryError& operator = (const MemoryError& exc)
	{
		if (&exc != this)
		{
			LocalizedError::operator = (exc);
		}

		return *this;
	}

	/**
	 * Destructeur : ARS
	 */
	virtual ~MemoryError ( )
	{ }

	/**
	 * @return		La raison d'être de l'exception, précédée d'un message
	 *				d'erreur d'allocation ou de pointeur nul et de la
	 *				localisation de la levée d'exception.
	 */
	virtual IN_UTIL UTF8String getFullMessage ( ) const;
};	// class MemoryError


/**
 * Macro simplifiant la création d'une exception de type MemoryError. Ex :
 * <CODE>
 * MEMORY_ERROR(exc, "Création impossible du tableau")<BR>
 * throw exc;<BR>
 * </CODE>
 */
#ifndef MEMORY_ERROR
#define	MEMORY_ERROR(varName,what)	MemoryError	varName(what, __FILE__, __LINE__);
#endif	// MEMORY_ERROR

/**
 * Macro évaluant la non nullité d'un pointeur. Ex :
 * <P><CODE>
 * T*	ptr	= new T (...);<BR>
 * CHECK_NULL_PTR_ERROR (ptr)<BR>
 * </CODE></P><BR>
 * <BR><BR><BR>
 * Ex 2 :
 * <P><CODE>
 * T*	ptr	= new T ( );<BR>
 * ...<BR>
 * CHECK_NULL_PTR_ERROR (ptr)<BR>
 * ptr->doIt ( );<BR>
 * </CODE></P>
 */
#ifndef CHECK_NULL_PTR_ERROR
#define CHECK_NULL_PTR_ERROR(ptr)	if (0==ptr) throw IN_UTIL MemoryError (IN_UTIL UTF8String ("Allocation memoire impossible ou pointeur nul.", IN_UTIL UTF8String::defaultCharset), __FILE__, __LINE__);
#endif	// CHECK_NULL_PTR_ERROR


END_NAMESPACE_UTIL


#endif	// MEMORY_ERROR_H

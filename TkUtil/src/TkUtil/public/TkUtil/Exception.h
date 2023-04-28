#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <TkUtil/util_config.h>
#include <TkUtil/UTF8String.h>

#include <string>
#include <stdexcept>


BEGIN_NAMESPACE_UTIL


/** Classe ne derivant pas de std::exception afin d'eviter des conflits avec
 * la structure exception de math.h, vtk sous OSF, ...
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class Exception
{
	public :

	/**
	 * @param		Raison d'être de l'exception
	 */
	Exception (const IN_UTIL UTF8String& what)
		: _what (what)
	{ }
	Exception ( )
		: _what ( )
	{ }

	/**
	 * <P>Stocke comme message :</BR>
	 * <CODE>what :
	 * exc.getFullMessage ( );</CODE>
	 * </P>
	 * @param		Raison d'être de l'exception
	 * @param		Origine de l'exception
	 */
	Exception (const IN_UTIL UTF8String& what, const Exception& exc)
		: _what ( )
	{
		_what	= what + ":\n" + exc.getFullMessage ( );
	}

	/**
	 * <P>Stocke comme message :</BR>
	 * <CODE>what :
	 * exc.what ( );</CODE>
	 * </P>
	 * @param		Raison d'être de l'exception
	 * @param		Origine de l'exception
	 */
	Exception (const IN_UTIL UTF8String& what, const std::exception& exc)
		: _what ( )
	{
		_what	= what + ":\n" + exc.what ( );
	}

	/**
	 * Constructeur de copie et opérateur = : RAS
	 */
	Exception (const Exception& exc)
		: _what (exc._what)
	{ }
	Exception& operator = (const Exception& exc)
	{
		if (&exc != this)
			_what	= exc._what;

		return *this;
	}

	/**
	 * Destructeur : RAS
	 */
	virtual ~Exception ( )
	{ }

	/**
	 * @return		La raison d'être de l'exception.
	 */
	virtual const IN_UTIL UTF8String& getMessage ( ) const
	{ return _what; }
	virtual IN_UTIL UTF8String getFullMessage ( ) const
	{ return getMessage ( ); }


	protected :

	/**
	 * @param		Raison d'être de l'exception
	 */
	virtual void setMessage (IN_UTIL UTF8String& message)
	{ _what = message; }


	private :

	IN_UTIL UTF8String		_what;
};	// class Exception


END_NAMESPACE_UTIL


#endif	// EXCEPTION_H

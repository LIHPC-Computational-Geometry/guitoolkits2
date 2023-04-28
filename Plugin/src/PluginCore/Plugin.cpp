#include "PluginCore/Plugin.h"
#include <TkUtil/Exception.h>
#include <TkUtil/UTF8String.h>

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <dlfcn.h>
#include <assert.h>


USING_STD
USING_UTIL


static const Charset	charset ("àéèùô");


namespace PLUGIN
{

Version Plugin::getVersion ( )
{
	return Version (PLUGIN_VERSION);
}	// Plugin::getVersion


Plugin::Plugin (const string& library, const string& symbol)
	: _libraryName (library),
	  _libraryHandle (NULL), _function ( )
{
	_function._name	= (char*)malloc ((symbol.length ( ) + 1) * sizeof (char));
	strcpy (_function._name, symbol.c_str ( ));
}	// Plugin::Plugin


Plugin::Plugin (const Plugin&)
{
	assert (0 && "Plugin::Plugin (const Plugin&) is not allowed.");
}	// Plugin::Plugin (const Plugin&)


Plugin& Plugin::operator = (const Plugin&)
{
	assert (0 && "Plugin::operator = (const Plugin&) is not allowed.");
	return *this;
}	// Plugin::operator =


Plugin::~Plugin ( )
{
	try
	{
		close ( );
	}
	catch (const Exception& exc)
	{
		UTF8String	mess (charset);
		mess << "Exception levée dans Plugin::~Plugin (" << __FILE__ 
		     << ' ' << (unsigned long)__LINE__ << ") pour la fonction "
		     << getSymbolName ( )
		     << " de la bibliothèque " << getLibraryName ( ) << " : "
		     << exc.getFullMessage ( );
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Erreur non documentée dans Plugin::~Plugin (" << __FILE__ 
		     << ' ' << (unsigned long)__LINE__ << ") pour la fonction "
		     << getSymbolName ( )
		     << " de la bibliothèque " << getLibraryName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// Plugin::~Plugin


void Plugin::setLibraryName (const string& libraryName)
{
	try
	{
		close ( );
	}
	catch (const Exception& exc)
	{
		UTF8String	mess (charset);
		mess << "Exception levée dans Plugin::setLibraryName (" << __FILE__ 
		     << ' ' << (unsigned long)__LINE__ << ") pour la fonction "
		     << getSymbolName ( )
		     << " de la bibliothèque " << getLibraryName ( ) << " : "
		     << exc.getFullMessage ( );
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Erreur non documentée dans Plugin::setLibraryName (" 
		     << __FILE__ << ' ' << (unsigned long)__LINE__ 
		     << ") pour la fonction " << getSymbolName ( )
		     << " de la bibliothèque " << getLibraryName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}

	_libraryName	= libraryName;
}	// setLibraryName


void Plugin::setSymbolName (const string& symbolName)
{
	PluginFunction&	function	= getFunction ( );
	setFunctionName (&function, symbolName.c_str ( ));
}	// Plugin::setSymbolName


string Plugin::getSymbolName ( ) const
{
	return getFunction ( )._name;
}	// Plugin::getSymbolName


void Plugin::execute ( )
{
	PluginFunction&	funct	= getFunction ( );
	setFunctionErrorMessage (&funct, NULL);
	setFunctionWarningMessage (&funct, NULL);
	open ( );
	void (*function)(PluginFunction*);
	function	= (void (*)(PluginFunction*))dlsym (
							getLibraryHandle ( ), getSymbolName( ).c_str( ));
	const char*	error	= dlerror ( );
	if (NULL != error)
	{
		UTF8String	errorMsg (
					"Erreur lors de l'exécution de la fonction ", charset);
		errorMsg	+= getSymbolName ( ) + " de la bibliothèque dynamique ";
		errorMsg	+= getLibraryName ( ) + " : ";
		errorMsg	+= error;
		throw Exception (errorMsg);
	}	// if (NULL != error)

	(*function)(&getFunction ( ));
}	// Plugin::execute


string Plugin::getWarningMessage ( ) const
{
	const PluginFunction&	function	= getFunction ( );
	return NULL == getFunctionWarningMessage (&function) ? 
	       string ( ) : string (getFunctionWarningMessage (&function));
}	// Plugin::getWarningMessage


string Plugin::getErrorMessage ( ) const
{
	const PluginFunction&	function	= getFunction ( );
	return NULL == getFunctionErrorMessage (&function) ? 
	       string ( ) : string (getFunctionErrorMessage (&function));
}	// Plugin::getErrorMessage


void Plugin::open ( )
{
	if (NULL == _libraryHandle)
	{
//		_libraryHandle	= dlopen (getLibraryName ( ).c_str ( ), RTLD_LAZY);
		_libraryHandle	= dlopen (getLibraryName ( ).c_str ( ), RTLD_NOW|RTLD_GLOBAL);
		if (NULL == _libraryHandle)
		{
			UTF8String	errorMsg (
				"Erreur à l'ouverture de la bibliothèque dynamique ", charset);
			errorMsg	+= getLibraryName ( ) + " : ";
			errorMsg	+= dlerror ( );
			throw Exception (errorMsg);
		}	// if (NULL == _libraryHandle)
	}	// if (NULL == _libraryHandle)
}	// Plugin::open


void Plugin::close ( )
{
	if (NULL != _libraryHandle)
	{
		int	result	= dlclose (_libraryHandle);
		_libraryHandle	= NULL;
		if (0 != result)
		{
			UTF8String	errorMsg (
				"Erreur à la fermeture de la bibliothèque dynamique ", charset);
			errorMsg	+= getLibraryName ( ) + " : ";
			errorMsg	+= dlerror ( );
			throw Exception (errorMsg);
		}	// if (0 != result)
	}	// if (NULL != _libraryHandle)
}	// Plugin::close


void Plugin::reset ( )
{
	close ( );
	setLibraryName ("");
	setSymbolName ("");
	PluginFunction&	function	= getFunction ( );
	freeArgList (&function);
	setFunctionErrorMessage (&function, NULL);
	setFunctionWarningMessage (&function, NULL);
}	// Plugin::reset


};	// namespace PLUGIN

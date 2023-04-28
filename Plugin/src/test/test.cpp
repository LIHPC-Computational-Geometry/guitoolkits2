#include "PluginCore/Plugin.h"
#include "TkUtil/Exception.h"

#include <iostream>


USING_STD
USING_UTIL
using namespace PLUGIN;


//
// Cet executable va charger la bibliotheque dynamique libName et en executer
// la fonction symbolName.
// Pour que cette fonction soit en mesure d'appeler des fonctions de
// l'executable il faut que ces fonctions soient egalement chargees
// dynamiquement => cet executable est linke avec libpluginapi.so qui
// implemente ces fonctions utilisables depuis des plugins. 
// Les fonctions de libpluginapi.so peuvent utiliser des fonctions
// "statiques" de l'executable
// => Cette libpluginapi.so n'est a charger que si des plugins seront
// charges.
//
// PB sous SGI : le path de la librairie a charger dynamiquement n'est pas
// pris en compte => doit figurer par ex dans le LD_LIBRARY_PATH.


// Implementation de core.h
void writeMessage (const string& message)
{
	cout << message << endl;
}	// writeMessage


#define CHECK_ERR(msg)                                                      \
	if (NULL != msg)                                                        \
		cout << "ERREUR : " << msg << endl;                                 \
	msg	= NULL;


int main (int argc, char* argv [])
{
//	string		libName ("lib/SunOS/libtestplugin.so");
	string	path (LIBPLUGIN_PATH);
	string	suffix (GSCC_SUFFIX);
	string		libName (path + "/liblibtestplugin" + suffix + ".so");
	string		symbolName ("a_function");
//	string		lib_cName ("lib/SunOS/libtestplugin_c.so");
	string		lib_cName (path + "/liblibtestplugin_c" + suffix + ".so");
	string		symbol_cName ("a_c_function");

	try
	{
		cout << "Exécution du symbôle " << symbolName 
		     << " de la bibliothèque " << libName << " ..." << endl;
		Plugin plugin (libName, symbolName);
		PluginFunction&	function	= plugin.getFunction ( );

		cout << endl << "Affectations d'arguments sans erreur : " << endl;
		const char*	err	= setBooleanArg (&function, "boolArg", 1);
		CHECK_ERR (err)
		err	= setStringArg (&function, "strArg", "a string");
		CHECK_ERR (err)
		err	= setLongArg (&function, "longArg", -89);
		CHECK_ERR (err)
		err	= setULongArg (&function, "unsignedLongArg", 765);
		CHECK_ERR (err)
		err	= setDoubleArg (&function, "doubleArg", -1.02E-5);
		CHECK_ERR (err)

		cout << endl << "Affectations d'arguments avec erreurs : " << endl;
		err	= setStringArg (&function, "longArg", "a string");
		CHECK_ERR (err)
		err	= setLongArg (&function, "strArg", -89);
		CHECK_ERR (err)
		err	= setULongArg (&function, "doubleArg", 765);
		CHECK_ERR (err)
		err	= setDoubleArg (&function, "unsignedLongArg", -1.02E-5);
		CHECK_ERR (err)

		cout << "Exécution du plugin " << plugin.getSymbolName ( ) 
		     << " ..." << endl;
		plugin.execute ( );
		cout << "Plugin " << plugin.getSymbolName ( ) << " de la bibliothèque "
		     << plugin.getLibraryName ( ) << " exécuté ";
		if (NULL != getFunctionErrorMessage (&function))
			cout << "avec l'erreur : " << getFunctionErrorMessage (&function)
			     << endl;
		else if (NULL != getFunctionWarningMessage (&function))
			cout << "avec l'avertissement : " 
			     << getFunctionWarningMessage (&function) << endl;
		else 
			cout << "avec succès." << endl;
		cout << endl;

		cout << "Exécution du symbôle " << symbol_cName 
		     << " de la bibliothèque " << lib_cName << " ..." << endl;
		Plugin cPlugin (lib_cName, symbol_cName);
		PluginFunction& cFunction	= cPlugin.getFunction ( );

		cout << endl << "Affectations d'arguments sans erreur : " << endl;
		err	= setBooleanArg (&cFunction, "boolArg", 1);
		CHECK_ERR (err)
		err	= setStringArg (&cFunction, "strArg", "a string");
		CHECK_ERR (err)
		err	= setLongArg (&cFunction, "longArg", -89);
		CHECK_ERR (err)
		err	= setULongArg (&cFunction, "unsignedLongArg", 765);
		CHECK_ERR (err)
		err	= setDoubleArg (&cFunction, "doubleArg", -1.02E-5);
		CHECK_ERR (err)

		cout << endl << "Affectations d'arguments avec erreurs : " << endl;
		err	= setStringArg (&cFunction, "longArg", "a string");
		CHECK_ERR (err)
		err	= setLongArg (&cFunction, "strArg", -89);
		CHECK_ERR (err)
		err	= setULongArg (&cFunction, "doubleArg", 765);
		CHECK_ERR (err)
		err	= setDoubleArg (&cFunction, "unsignedLongArg", -1.02E-5);
		CHECK_ERR (err)

		cout << "Exécution du plugin " << cPlugin.getSymbolName ( ) 
		     << " ..." << endl;
		cPlugin.execute ( );
		cout << "Plugin " << cPlugin.getSymbolName ( ) << " de la bibliothèque "
		     << cPlugin.getLibraryName ( ) << " exécuté ";
		if (NULL != getFunctionErrorMessage (&cFunction))
			cout << "avec l'erreur : " << getFunctionErrorMessage (&cFunction)
			     << endl;
		else if (NULL != getFunctionWarningMessage (&cFunction))
			cout << "avec l'avertissement : " 
			     << getFunctionWarningMessage (&cFunction) << endl;
		else 
			cout << "avec succès." << endl;
		cout << endl;
	}
	catch (const Exception& exc)
	{
		cout << exc.getFullMessage ( ) << endl;
		return -1;
	}
	catch (...)
	{
		cout << "Erreur non documentée." << endl;
		return -2;
	}
	
	return 0;
}	// main

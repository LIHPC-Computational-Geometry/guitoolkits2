#include <iostream>
#include <stdlib.h>

#include "TkUtil/util_config.h"
#include "PluginCore/PluginFunction.h"
#include "app_api.h"


USING_STD


#define CHECK_ERR(msg)                                                      \
    if (NULL != msg)                                                        \
        cout << "ERREUR : " << msg << endl;                                 \
	msg	= NULL;



extern "C"
{

void a_function (PluginFunction* function)
{
	cout << "Plugin function name is " << function->_name << endl;	

	cout << "Arguments are : " << endl;
	unsigned char	aBool	= 255;
	const char*	err	= getBooleanArg (function, "boolArg", &aBool);
	CHECK_ERR (err)
	cout << "aBool = " << (unsigned short)aBool << endl;
	char*	aString	= NULL;
	err	= getStringArg (function, "strArg", &aString);
	CHECK_ERR (err)
	cout << "strArg = " << aString << endl;
	free (aString);		aString	= NULL;
	long	aLong	= 0;
	err	= getLongArg (function, "longArg", &aLong);
	CHECK_ERR (err)
	cout << "longArg = " << aLong << endl;
	unsigned long	aULong	= 0;
	err	= getULongArg (function, "unsignedLongArg", &aULong);
	CHECK_ERR (err)
	cout << "unsignedLongArg = " << aULong << endl;
	double	aDouble	= 0.;
	err	= getDoubleArg (function, "doubleArg", &aDouble);
	CHECK_ERR (err)
	cout << "doubleArg = " << aDouble << endl;

	cout << endl << "Trying to get invalid arguments ..." << endl;
	err	= getBooleanArg (function, "boolAr", &aBool);
	CHECK_ERR (err)
	err	= getBooleanArg (function, "strArg", &aBool);
	CHECK_ERR (err)
	err	= getStringArg (function, "strAr", &aString);
	CHECK_ERR (err)
	err	= getLongArg (function, "unsignedLongArg", &aLong);
	CHECK_ERR (err)
	err	= getULongArg (function, "uLongArg", &aULong);
	CHECK_ERR (err)
	err	= getDoubleArg (function, "doublearg", &aDouble);
	CHECK_ERR (err)

	cout << endl << "Calling hello from main program ..." << endl;
	hello ( );
	cout << endl << "getLongValue from main program returns " 
	     << getLongValue ( ) << endl;
	setFunctionErrorMessage (function, NULL);
	setFunctionWarningMessage (function, "ERREUR de récupération de l'argument doublearg : valeur par défaut utilisée.");
	cout << "a_function from file " << __FILE__ << " completed." << endl;
}	// a_function

}	// extern "C"

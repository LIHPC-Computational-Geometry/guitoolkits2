#include <stdio.h>

#include "PluginCore/PluginFunction.h"
#include "app_api.h"


#define CHECK_ERR(msg)                                                      \
    if (NULL != msg)                                                        \
        printf ("ERREUR : %s\n", msg);                                      \
	msg	= NULL;



void a_c_function (struct PluginFunction* function)
{
	const char*		err		= NULL;
	char*			aString	= NULL;
	unsigned char	aBool	= 255;
	long			aLong	= 0;
	unsigned long	aULong	= 0;
	double			aDouble	= 0.;

	printf ("\nPlugin function name is %s\n", getFunctionName (function));	

	printf ("Arguments are :\n");
	err	= getBooleanArg (function, "boolArg", &aBool);
	CHECK_ERR (err)
	printf ("aBool = %d\n", (int)aBool);
	err	= getStringArg (function, "strArg", &aString);
	CHECK_ERR (err)
	printf ("strArg = %s\n", aString);
	free (aString);		aString	= NULL;
	err	= getLongArg (function, "longArg", &aLong);
	CHECK_ERR (err)
	printf ("longArg = %ld\n", aLong);
	err	= getULongArg (function, "unsignedLongArg", &aULong);
	CHECK_ERR (err)
	printf ("unsignedLongArg = %lu\n", aULong);
	err	= getDoubleArg (function, "doubleArg", &aDouble);
	CHECK_ERR (err)
	printf ("doubleArg = %g\n", aDouble);

	printf ("\nTrying to get invalid arguments ...\n");
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

	printf ("\nCalling hello from main program ...\n");
	hello ( );
	printf ("\ngetLongValue from main program returns %ld\n",
	        getLongValue ( ));
	setFunctionErrorMessage (function, "ERREUR de récupération de l'argument doublearg : traitement annulé.");
	setFunctionWarningMessage (function, NULL);
	printf ("\na_function from file %s completed\n", __FILE__);
}	// a_c_function

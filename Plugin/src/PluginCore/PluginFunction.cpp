#include <iostream>
#include "PluginCore/PluginFunction.h"
#include <TkUtil/InternalError.h>
#include <TkUtil/UTF8String.h>

#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>


USING_UTIL
USING_STD

static const Charset	charset ("àéèùô");


// ==========================================================================
//                         LA CLASSE FunctionArgument
// ==========================================================================

/**
 * Classe décrivant un argument de fonction.
 */
class FunctionArgument
{
	public :

	/**
	 * Les différents types d'arguments supportés.
	 */
	enum TYPE {BOOL, STRING, LONG, ULONG, DOUBLE };

	/**
	 * Constructeurs avec valeur.
	 * @param		Nom de l'argument.
	 * @param		Valeur de l'argument.
	 */
	FunctionArgument (const string& name, const string& value);
	FunctionArgument (const string& name, bool value);
	FunctionArgument (const string& name, long value);
	FunctionArgument (const string& name, unsigned long value);
	FunctionArgument (const string& name, double value);

	/**
	 * Constructeur de copie et opérateur = : RAS.
	 */
	FunctionArgument (const FunctionArgument& arg);
	FunctionArgument& operator = (const FunctionArgument& arg);

	/**
	 * Destructeur : RAS.
	 */
	~FunctionArgument ( );

	/**
	 * @return		Le nom de l'argument
	 */
	const string& getName ( ) const
	{ return _name; }

	/**
	 * @return		Le type de l'argument
	 */
	TYPE getType ( ) const
	{ return _type; }

	/**
	 * @return		L'argument en tant que booléen
	 * @throw		Une exception est levée si l'argument n'est pas de ce type
	 */
	bool getBooleanValue ( ) const;

	/**
	 * @param		Nouvelle valeur de l'argument.
	 * @throw		Une exception est levée si l'argument n'est pas de ce type
	 */
	void setBooleanValue (bool value);

	/**
	 * @return		L'argument en tant que chaine de caractères
	 * @throw		Une exception est levée si l'argument n'est pas de ce type
	 */
	const string& getStringValue ( ) const;

	/**
	 * @param		Nouvelle valeur de l'argument.
	 * @throw		Une exception est levée si l'argument n'est pas de ce type
	 */
	void setStringValue (const string& value);

	/**
	 * @return		L'argument en tant qu'entier long
	 * @throw		Une exception est levée si l'argument n'est pas de ce type
	 */
	long getLongValue ( ) const;

	/**
	 * @param		Nouvelle valeur de l'argument.
	 * @throw		Une exception est levée si l'argument n'est pas de ce type
	 */
	void setLongValue (long value);

	/**
	 * @return		L'argument en tant qu'entier long non signé
	 * @throw		Une exception est levée si l'argument n'est pas de ce type
	 */
	unsigned long getUnsignedLongValue ( ) const;

	/**
	 * @param		Nouvelle valeur de l'argument.
	 * @throw		Une exception est levée si l'argument n'est pas de ce type
	 */
	void setUnsignedLongValue (unsigned long value);

	/**
	 * @return		L'argument en tant que double
	 * @throw		Une exception est levée si l'argument n'est pas de ce type
	 */
	double getDoubleValue ( ) const;

	/**
	 * @param		Nouvelle valeur de l'argument.
	 * @throw		Une exception est levée si l'argument n'est pas de ce type
	 */
	void setDoubleValue (double value);


	private :

	/** Le type de l'arguments. */
	TYPE			_type;

	/** Le nom de l'argument. */
	string			_name;

	/** La valeur de l'argument. */
	bool			_booleanValue;
	string			_stringValue;
	long			_longValue;
	unsigned long	_uLongValue;
	double			_doubleValue;
};	// class FunctionArgument


FunctionArgument::FunctionArgument (const string& name, const string& value)
	: _type (STRING), _name (name), _booleanValue (false), _stringValue (value),
	  _longValue (0), _uLongValue (0), _doubleValue (0.)
{
}	// FunctionArgument::FunctionArgument


FunctionArgument::FunctionArgument (const string& name, bool value)
	: _type (BOOL), _name (name), _booleanValue (value), _stringValue ( ), 
	  _longValue (0), _uLongValue (0), _doubleValue (0.)
{
}	// FunctionArgument::FunctionArgument


FunctionArgument::FunctionArgument (const string& name, long value)
	: _type (LONG), _name (name), _booleanValue (false), _stringValue ( ), 
	  _longValue (value), _uLongValue (0), _doubleValue (0.)
{
}	// FunctionArgument::FunctionArgument


FunctionArgument::FunctionArgument (const string& name, unsigned long value)
	: _type (ULONG), _name (name), _booleanValue (false), _stringValue ( ), 
	  _longValue (0), _uLongValue (value), _doubleValue (0.)
{
}	// FunctionArgument::FunctionArgument


FunctionArgument::FunctionArgument (const string& name, double value)
	: _type (DOUBLE), _name (name), _booleanValue (false), _stringValue ( ), 
	  _longValue (0), _uLongValue (0), _doubleValue (value)
{
}	// FunctionArgument::FunctionArgument


FunctionArgument::FunctionArgument (const FunctionArgument& arg)
	: _type (arg._type), _name (arg._name), 
	  _booleanValue (arg._booleanValue), 
	  _stringValue (arg._stringValue),
	  _longValue (arg._longValue),
	  _uLongValue (arg._uLongValue),
	  _doubleValue (arg._doubleValue)
{
}	// FunctionArgument (const FunctionArgument&)


FunctionArgument& FunctionArgument::operator = (const FunctionArgument& arg)
{
	if (&arg != this)
	{
		_type			= arg._type;
		_name			= arg._name;
		_booleanValue	= arg._booleanValue;
		_stringValue	= arg._stringValue;
		_longValue		= arg._longValue;
		_uLongValue		= arg._uLongValue;
		_doubleValue	= arg._doubleValue;
	}	// if (&arg != this)

	return *this;
}	// FunctionArgument::operator =


FunctionArgument::~FunctionArgument ( )
{
}	// FunctionArgument::~FunctionArgument


bool FunctionArgument::getBooleanValue ( ) const
{
	if (BOOL != _type)
	{
		UTF8String	error ("L'argument ", charset);
		error	+= getName ( ) + " n'est pas de type booléen.";
		throw Exception (error);
	}	// if (BOOL != _type)

	return _booleanValue;
}	// FunctionArgument::getBooleanValue


void FunctionArgument::setBooleanValue (bool value)
{
	if (BOOL != _type)
	{
		UTF8String	error ("L'argument ", charset);
		error	+= getName ( ) + " n'est pas de type booléen.";
		throw Exception (error);
	}	// if (BOOL != _type)

	_booleanValue	= value;
}	// FunctionArgument::setBooleanValue


const string& FunctionArgument::getStringValue ( ) const
{
	if (STRING != _type)
	{
		UTF8String	error ("L'argument ", charset);
		error	+= getName ( ) + " n'est pas de type chaine de caractères.";
		throw Exception (error);
	}	// if (STRING != _type)

	return _stringValue;
}	// FunctionArgument::getStringValue


void FunctionArgument::setStringValue (const string& value)
{
	if (STRING != _type)
	{
		UTF8String	error ("L'argument ", charset);
		error	+= getName ( ) + " n'est pas de type chaine de caractères.";
		throw Exception (error);
	}	// if (STRING != _type)

	_stringValue	= value;
}	// FunctionArgument::setStringValue


long FunctionArgument::getLongValue ( ) const
{
	if (LONG != _type)
	{
		UTF8String	error ("L'argument ", charset);
		error	+= getName ( ) + " n'est pas de type long.";
		throw Exception (error);
	}	// if (LONG != _type)

	return _longValue;
}	// FunctionArgument::getLongValue


void FunctionArgument::setLongValue (long value)
{
	if (LONG != _type)
	{
		UTF8String	error ("L'argument ", charset);
		error	+= getName ( ) + " n'est pas de type long.";
		throw Exception (error);
	}	// if (LONG != _type)

	_longValue	= value;
}	// FunctionArgument::setLongValue


unsigned long FunctionArgument::getUnsignedLongValue ( ) const
{
	if (ULONG != _type)
	{
		UTF8String	error ("L'argument ", charset);
		error	+= getName ( ) + " n'est pas de type long non signé.";
		throw Exception (error);
	}	// if (ULONG != _type)

	return _uLongValue;
}	// FunctionArgument::getUnsignedLongValue


void FunctionArgument::setUnsignedLongValue (unsigned long value)
{
	if (ULONG != _type)
	{
		UTF8String	error ("L'argument ", charset);
		error	+= getName ( ) + " n'est pas de type long non signé.";
		throw Exception (error);
	}	// if (ULONG != _type)

	_uLongValue	= value;
}	// FunctionArgument::setUnsignedLongValue


double FunctionArgument::getDoubleValue ( ) const
{
	if (DOUBLE != _type)
	{
		UTF8String	error ("L'argument ", charset);
		error	+= getName ( ) + " n'est pas de type double.";
		throw Exception (error);
	}	// if (DOUBLE != _type)

	return _doubleValue;
}	// FunctionArgument::getDoubleValue


void FunctionArgument::setDoubleValue (double value)
{
	if (DOUBLE != _type)
	{
		UTF8String	error ("L'argument ", charset);
		error	+= getName ( ) + " n'est pas de type double.";
		throw Exception (error);
	}	// if (DOUBLE != _type)

	_doubleValue	= value;
}	// FunctionArgument::setDoubleValue


// ==========================================================================
//                         LA STRUCTURE PluginFunction
// ==========================================================================


static vector<FunctionArgument>& getArgs (PluginFunction& function)
{
	if (0 == function._args)
	{
		INTERNAL_ERROR (exc, "Liste des arguments nulle.", "PluginFunction::getArgs")
		throw exc;
	}

	vector<FunctionArgument>*	argsList	= 
				reinterpret_cast<vector<FunctionArgument>*> (function._args);
	if (0 == argsList)
	{
		INTERNAL_ERROR (exc, "Liste des arguments de type invalide.", "PluginFunction::getArgs")
		throw exc;
	}

	return *argsList;
}	// getArgs


static const vector<FunctionArgument>& getArgs (const PluginFunction& function)
{
	if (0 == function._args)
	{
		INTERNAL_ERROR (exc, "Liste des arguments nulle.", "PluginFunction::getArgs")
		throw exc;
	}

	vector<FunctionArgument>*	argsList	= 
				reinterpret_cast<vector<FunctionArgument>*> (function._args);
	if (0 == argsList)
	{
		INTERNAL_ERROR (exc, "Liste des arguments de type invalide.", "PluginFunction::getArgs")
		throw exc;
	}

	return *argsList;
}	// getArgs


PluginFunction::PluginFunction ( )
	: _name (NULL), _args (0), _errorMessage (NULL), _warningMessage (NULL)
{
	_args	= new vector<FunctionArgument>;
}	// PluginFunction::PluginFunction


PluginFunction::PluginFunction (const PluginFunction& function)
	: _name (NULL), _args (0), _errorMessage (NULL), _warningMessage (NULL)
{
	if (NULL != function._name)
	{
		_name	= (char*)malloc ((strlen (function._name) + 1) * sizeof (char));
		strcpy (_name, function._name);
	}	// if (NULL != function._name)
	if (NULL != function._errorMessage)
	{
		_errorMessage	= (char*)malloc ((
					strlen (function._errorMessage) + 1) * sizeof (char));
		strcpy (_errorMessage, function._errorMessage);
	}	// if (NULL != function._errorMessage)
	if (NULL != function._warningMessage)
	{
		_warningMessage	= (char*)malloc ((
					strlen (function._warningMessage) + 1) * sizeof (char));
		strcpy (_warningMessage, function._warningMessage);
	}	// if (NULL != function._warningMessage)

	vector<FunctionArgument>*	args	= new vector<FunctionArgument>;
	_args	= args;
	const vector<FunctionArgument>&	copied	= 
							getArgs ((PluginFunction&)function);
	for (vector<FunctionArgument>::const_iterator it = copied.begin ( );
	     copied.end ( ) != it; it++)
		args->push_back (*it);
}	// PluginFunction::PluginFunction


PluginFunction& PluginFunction::operator = (const PluginFunction& function)
{
	if (&function != this)
	{
		if (NULL != _name)
			{ free (_name); _name	= NULL; }
		if (NULL != _errorMessage)
			{ free (_errorMessage); _errorMessage	= NULL; }
		if (NULL != _warningMessage)
			{ free (_warningMessage); _warningMessage	= NULL; }
		if (NULL != function._name)
		{
			_name	= (char*)malloc (
							(strlen (function._name) + 1) * sizeof (char));
			strcpy (_name, function._name);
		}	// if (NULL != function._name)
		if (NULL != function._errorMessage)
		{
			_errorMessage	= (char*)malloc (
					(strlen (function._errorMessage) + 1) * sizeof (char));
			strcpy (_errorMessage, function._errorMessage);
		}	// if (NULL != function._errorMessage)
		if (NULL != function._warningMessage)
		{
			_warningMessage	= (char*)malloc (
					(strlen (function._warningMessage) + 1) * sizeof (char));
			strcpy (_warningMessage, function._warningMessage);
		}	// if (NULL != function._warningMessage)

		vector<FunctionArgument>&	args	= getArgs (*this);
		vector<FunctionArgument>&	copied	= getArgs ((PluginFunction&)function);
		args.clear ( );
		for (vector<FunctionArgument>::const_iterator it = copied.begin ( );
		     copied.end ( ) != it; it++)
			args.push_back (*it);
	}	// if (&function != this)

	return *this;
}	// PluginFunction::operator =


PluginFunction::~PluginFunction ( )
{
	if (NULL != _name)
		free (_name);
	if (NULL != _errorMessage)
		free (_errorMessage);
	if (NULL != _warningMessage)
		free (_warningMessage);
	vector<FunctionArgument>*	args	= &(getArgs (*this));
	delete args;
}	// PluginFunction::~PluginFunction


// ==========================================================================
//             FONCTIONS ASSOCIEES A LA STRUCTURE PluginFunction
// ==========================================================================


// Chaine destinée à stocker les éventuels messages d'erreur :
static string	errorMsg;

#define CHECK_FUNCTION(ptr,functionName)                                     \
	if (NULL == ptr)                                                         \
	{                                                                        \
		UTF8String	msg	("Impossible de modifier les arguments de la fonction ", charset);\
		msg	+= " par la fonction" + functionName;                            \
		msg	+= " : pointeur nul.";                                           \
		throw Exception (msg);                                               \
    }	/* if (NULL == ptr) */                                               \
	if (NULL == ptr->_name)                                                  \
	{                                                                        \
		UTF8String	msg	("Impossible de modifier les arguments de la fonction ", charset);\
		msg	+= " par la fonction" + functionName;                            \
		msg	+= " : la fonction n'a pas de nom.";                             \
		throw Exception (msg);                                               \
    }	/* if (NULL == ptr->_name) */ 

#define CHECK_ARG(ptr,argName,functionName)                                  \
	if (NULL == ptr)                                                         \
	{                                                                        \
		UTF8String	msg	("Impossible d'affecter la valeur de l'argument ", charset);      \
		msg	+= argName + string (" par la fonction") + functionName          \
			+ " : pointeur nul.";                                            \
		throw Exception (msg);                                               \
    }


static FunctionArgument& getBooleanArg (vector<FunctionArgument>& args,
                                        const string& name, bool create)
{
	for (vector<FunctionArgument>::iterator it = args.begin ( );
	     args.end ( ) != it; it++)
	{
		if ((*it).getName ( ) == name)
			if ((*it).getType ( ) == FunctionArgument::BOOL)
				return *it;
			else
			{
				UTF8String	errorMsg ("L'argument ", charset);
				errorMsg	+= name + " n'est pas de type booléen.";
				throw Exception (errorMsg);
			}
	}	// for (vector<FunctionArgument>::iterator it = args.begin ( ); ...

	if (true == create)
	{
		FunctionArgument arg (name, false);
		args.push_back (arg);
		return getBooleanArg (args, name, false);
	}	// if (true == create)

	UTF8String	message ("L'argument booléen ");
	message	+= name + " n'existe pas dans la liste des arguments.";
	throw Exception (message);
}	// getBooleanArg


static FunctionArgument& getStringArg (vector<FunctionArgument>& args,
                                       const string& name, bool create)
{
	for (vector<FunctionArgument>::iterator it = args.begin ( );
	     args.end ( ) != it; it++)
	{
		if ((*it).getName ( ) == name)
			if ((*it).getType ( ) == FunctionArgument::STRING)
				return *it;
			else
			{
				UTF8String	errorMsg ("L'argument ", charset);
				errorMsg	+= name + " n'est pas de type chaine.";
				throw Exception (errorMsg);
			}
	}	// for (vector<FunctionArgument>::iterator it = args.begin ( ); ...

	if (true == create)
	{
		FunctionArgument arg (name, string ( ));
		args.push_back (arg);
		return getStringArg (args, name, false);
	}	// if (true == create)

	UTF8String	message ("L'argument de type chaine ", charset);
	message	+= name + " n'existe pas dans la liste des arguments.";
	throw Exception (message);
}	// getStringArg


static FunctionArgument& getLongArg (vector<FunctionArgument>& args,
                                     const string& name, bool create)
{
	for (vector<FunctionArgument>::iterator it = args.begin ( );
	     args.end ( ) != it; it++)
	{
		if ((*it).getName ( ) == name)
			if ((*it).getType ( ) == FunctionArgument::LONG)
				return *it;
			else
			{
				UTF8String	errorMsg ("L'argument ", charset);
				errorMsg	+= name + " n'est pas de type entier long.";
				throw Exception (errorMsg);
			}
	}	// for (vector<FunctionArgument>::iterator it = args.begin ( ); ...

	if (true == create)
	{
		FunctionArgument arg (name, (long)0);
		args.push_back (arg);
		return getLongArg (args, name, false);
	}	// if (true == create)

	UTF8String	message ("L'argument de type entier long ", charset);
	message	+= name + " n'existe pas dans la liste des arguments.";
	throw Exception (message);
}	// getLongArg


static FunctionArgument& getULongArg (vector<FunctionArgument>& args,
                                      const string& name, bool create)
{
	for (vector<FunctionArgument>::iterator it = args.begin ( );
	     args.end ( ) != it; it++)
	{
		if ((*it).getName ( ) == name)
			if ((*it).getType ( ) == FunctionArgument::ULONG)
				return *it;
			else
			{
				UTF8String	errorMsg ("L'argument ", charset);
				errorMsg	+= name+" n'est pas de type entier long non signé.";
				throw Exception (errorMsg);
			}
	}	// for (vector<FunctionArgument>::iterator it = args.begin ( ); ...

	if (true == create)
	{
		FunctionArgument arg (name, (unsigned long)0);
		args.push_back (arg);
		return getULongArg (args, name, false);
	}	// if (true == create)

	UTF8String	message ("L'argument de type entier long non signé ", charset);
	message	+= name + " n'existe pas dans la liste des arguments.";
	throw Exception (message);
}	// getULongArg


static FunctionArgument& getDoubleArg (vector<FunctionArgument>& args,
                                       const string& name, bool create)
{
	for (vector<FunctionArgument>::iterator it = args.begin ( );
	     args.end ( ) != it; it++)
	{
		if ((*it).getName ( ) == name)
			if ((*it).getType ( ) == FunctionArgument::DOUBLE)
				return *it;
			else
			{
				UTF8String	errorMsg ("L'argument ", charset);
				errorMsg	+= name + " n'est pas de type réel.";
				throw Exception (errorMsg);
			}
	}	// for (vector<FunctionArgument>::iterator it = args.begin ( ); ...

	if (true == create)
	{
		FunctionArgument arg (name, (double)0.);
		args.push_back (arg);
		return getDoubleArg (args, name, false);
	}	// if (true == create)

	UTF8String	message ("L'argument réel ", charset);
	message	+= name + " n'existe pas dans la liste des arguments.";
	throw Exception (message);
}	// getDoubleArg


#define BEGIN_SET_GET_TRY_CATCH_BLOCK(msg)                                    \
	bool	hasError	= false;                                              \
	                                                                          \
	UTF8String	message (#msg, charset);                                      \
	try                                                                       \
	{


#define COMPLETE_SET_GET_TRY_CATCH_BLOCK                                      \
	}                                                                         \
	catch (const Exception& exc)                                              \
	{                                                                         \
		hasError	= true;                                                   \
		message		+= exc.getFullMessage ( );                                \
	}                                                                         \
	catch (const exception& stdExc)                                           \
	{                                                                         \
		hasError	= true;                                                   \
		message		+= stdExc.what ( );                                       \
	}                                                                         \
	catch (...)                                                               \
	{                                                                         \
		hasError	= true;                                                   \
		message		+= "erreur non documentée.";                              \
	}                                                                         \
	                                                                          \
	if (true == hasError)                                                     \
	{                                                                         \
		errorMsg	= message;                                                \
		return errorMsg.c_str ( );                                            \
	}	/* if (true == hasError) */                                           \
	                                                                          \
	errorMsg	= "";                                                         \
	                                                                          \
	return NULL;


extern "C"
{

const char* getFunctionName (const PluginFunction* function)
{
	try
	{
		CHECK_FUNCTION (function, string ("getFunctionName"))
		return function->_name;
	}
	catch (...)
	{
		return NULL;
	}
}	// getFunctionName


void setFunctionName (PluginFunction* function, const char* name)
{
	if (NULL == name)
		return;

	try
	{
		CHECK_FUNCTION (function, string ("setFunctionName"))
		if (NULL != function->_name)
			free (function->_name);
		function->_name	= (char*)malloc ((strlen (name) + 1) * sizeof (char));
		strcpy (function->_name, name);
	}
	catch (...)
	{
	}
}	// setFunctionName


const char* getFunctionErrorMessage (const PluginFunction* function)
{
	try
	{
		CHECK_FUNCTION (function, string ("getFunctionErrorMessage"))
		return function->_errorMessage;
	}
	catch (...)
	{
		return NULL;
	}
}	// getFunctionErrorMessage


void setFunctionErrorMessage (PluginFunction* function, const char* message)
{
	try
	{
		CHECK_FUNCTION (function, string ("setFunctionErrorMessage"))
		if (NULL != function->_errorMessage)
			free (function->_errorMessage);
		function->_errorMessage	= NULL;
		if (NULL != message)
		{
			function->_errorMessage	= 
				(char*)malloc ((strlen (message) + 1) * sizeof (char));
			strcpy (function->_errorMessage, message);
		}	// if (NULL != message)
	}
	catch (...)
	{
	}
}	// setFunctionErrorMessage


const char* getFunctionWarningMessage (const PluginFunction* function)
{
	try
	{
		CHECK_FUNCTION (function, string ("getFunctionWarningMessage"))
		return function->_warningMessage;
	}
	catch (...)
	{
		return NULL;
	}
}	// getFunctionWarningMessage


void setFunctionWarningMessage (PluginFunction* function, const char* message)
{
	try
	{
		CHECK_FUNCTION (function, string ("setFunctionWarningMessage"))
		if (NULL != function->_warningMessage)
			free (function->_warningMessage);
		function->_warningMessage	= NULL;
		if (NULL != message)
		{
			function->_warningMessage	= 
				(char*)malloc ((strlen (message) + 1) * sizeof (char));
			strcpy (function->_warningMessage, message);
		}	// if (NULL != message)
	}
	catch (...)
	{
	}
}	// setFunctionWarningMessage


const char* getBooleanArg (const PluginFunction* function, const char* name,
                           unsigned char* value)
{
	BEGIN_SET_GET_TRY_CATCH_BLOCK("Impossible de récupérer l'argument ")
		CHECK_FUNCTION (function, string ("getBooleanArg"))
		CHECK_ARG (value, name, string ("getBooleanArg"))
		message	+= name + string (" de la fonction ") + string (function->_name)
				+ " : ";
 
		const vector<FunctionArgument>&	args	= getArgs (*function);
		FunctionArgument&				arg		= 
				getBooleanArg ((vector<FunctionArgument>&)args, name, false);
		*value	= true == arg.getBooleanValue ( ) ? 1 : 0;
	COMPLETE_SET_GET_TRY_CATCH_BLOCK
}	// getBooleanArg


const char* setBooleanArg (PluginFunction* function, const char* name,
                           unsigned char value)
{
	BEGIN_SET_GET_TRY_CATCH_BLOCK("Impossible de modifier l'argument ")
		CHECK_FUNCTION(function, string ("setBooleanArg"))
		message	+= name + UTF8String (" à la fonction ", charset) +
		           string (function->_name)
				+ " : ";

		vector<FunctionArgument>&	args	= getArgs (*function);
		FunctionArgument&			arg		= getBooleanArg (args, name, true);
		arg.setBooleanValue (1 == value ? true : 0);
	COMPLETE_SET_GET_TRY_CATCH_BLOCK
}	// setBooleanArg


const char* getStringArg (const PluginFunction* function, const char* name,
                          char** value)
{
	BEGIN_SET_GET_TRY_CATCH_BLOCK("Impossible de récupérer l'argument ")
		CHECK_FUNCTION (function, string ("getStringArg"))
		CHECK_ARG (value, name, string ("getStringArg"))
		message	+= name + UTF8String (" de la fonction ", charset)
		        + string (function->_name) + " : ";
 
		const vector<FunctionArgument>&	args	= getArgs (*function);
		FunctionArgument&				arg		= 
				getStringArg ((vector<FunctionArgument>&)args, name, false);
		string						strValue= arg.getStringValue ( );
		*value	= (char*)malloc (strValue.length ( ) + 1);
		strcpy (*value, strValue.c_str ( ));
	COMPLETE_SET_GET_TRY_CATCH_BLOCK
}	// getStringArg


const char* setStringArg (PluginFunction* function, const char* name,
                          const char* value)
{
	BEGIN_SET_GET_TRY_CATCH_BLOCK("Impossible de modifier l'argument ")
		CHECK_FUNCTION (function, string ("setStringArg"))
		message	+= name + UTF8String (" à la fonction ", charset)
		        + string (function->_name) + " : ";
 
		vector<FunctionArgument>&	args	= getArgs (*function);
		FunctionArgument&			arg		= getStringArg (args, name, true);
		arg.setStringValue (value);
	COMPLETE_SET_GET_TRY_CATCH_BLOCK
}	// setStringArg


const char* getLongArg (
				const PluginFunction* function, const char* name, long* value)
{
	BEGIN_SET_GET_TRY_CATCH_BLOCK("Impossible de récupérer l'argument ")
		CHECK_FUNCTION (function, string ("getLongArg"))
		CHECK_ARG (value, name, string ("getLongArg"))
		message	+= name + UTF8String (" de la fonction ", charset)
		        + string (function->_name) + " : ";
 
		const vector<FunctionArgument>&	args	= getArgs (*function);
		FunctionArgument&				arg		= 
					getLongArg ((vector<FunctionArgument>&)args, name, false);
		*value	= arg.getLongValue ( );
	COMPLETE_SET_GET_TRY_CATCH_BLOCK
}	// getLongArg


const char* setLongArg (PluginFunction* function, const char* name, long value)
{
	BEGIN_SET_GET_TRY_CATCH_BLOCK("Impossible de modifier l'argument ")
		CHECK_FUNCTION (function, string ("setLongArg"))
		message	+= name + UTF8String (" à la fonction ", charset)
		        + string (function->_name) + " : ";
 
		vector<FunctionArgument>&	args	= getArgs (*function);
		FunctionArgument&			arg		= getLongArg (args, name, true);
		arg.setLongValue (value);
	COMPLETE_SET_GET_TRY_CATCH_BLOCK
}	// setLongArg


const char* getULongArg (const PluginFunction* function, const char* name,
                         unsigned long* value)
{
	BEGIN_SET_GET_TRY_CATCH_BLOCK("Impossible de récupérer l'argument ")
		CHECK_FUNCTION (function, string ("getULongArg"))
		CHECK_ARG (value, name, string ("getULongArg"))
		message	+= name + UTF8String (" de la fonction ", charset)
		        + string (function->_name) + " : ";
 
		const vector<FunctionArgument>&	args	= getArgs (*function);
		FunctionArgument&				arg		= 
					getULongArg ((vector<FunctionArgument>&)args, name, false);
		*value	= arg.getUnsignedLongValue ( );
	COMPLETE_SET_GET_TRY_CATCH_BLOCK
}	// getULongArg


const char* setULongArg (PluginFunction* function, const char* name,
                         unsigned long value)
{
	BEGIN_SET_GET_TRY_CATCH_BLOCK("Impossible de modifier l'argument ")
		CHECK_FUNCTION (function, string ("setULongArg"))
		message	+= name + UTF8String (" à la fonction ", charset)
		        + string (function->_name) + " : ";
 
		vector<FunctionArgument>&	args	= getArgs (*function);
		FunctionArgument&			arg		= getULongArg (args, name, true);
		arg.setUnsignedLongValue (value);
	COMPLETE_SET_GET_TRY_CATCH_BLOCK
}	// setULongArg


const char* getDoubleArg (const PluginFunction* function, const char* name,
                          double* value)
{
	BEGIN_SET_GET_TRY_CATCH_BLOCK("Impossible de récupérer l'argument ")
		CHECK_FUNCTION (function, string ("getDoubleArg"))
		CHECK_ARG (value, name, string ("getDoubleArg"))
		message	+= name + UTF8String (" de la fonction ", charset)
		        + string (function->_name) + " : ";
 
		const vector<FunctionArgument>&	args	= getArgs (*function);
		FunctionArgument&				arg		= 
					getDoubleArg ((vector<FunctionArgument>&)args, name, false);
		*value	= arg.getDoubleValue ( );
	COMPLETE_SET_GET_TRY_CATCH_BLOCK
}	// getDoubleArg


const char* setDoubleArg (PluginFunction* function, const char* name,
                          double value)
{
	BEGIN_SET_GET_TRY_CATCH_BLOCK("Impossible de modifier l'argument ")
		CHECK_FUNCTION (function, string ("setDoubleArg"))
		message	+= name + UTF8String (" à la fonction ", charset)
		        + string (function->_name) + " : ";
 
		vector<FunctionArgument>&	args	= getArgs (*function);
		FunctionArgument&			arg		= getDoubleArg (args, name, true);
		arg.setDoubleValue (value);
	COMPLETE_SET_GET_TRY_CATCH_BLOCK
}	// setDoubleArg


const char* freeArgList (struct PluginFunction* function)
{
	BEGIN_SET_GET_TRY_CATCH_BLOCK("Impossible de supprimer les arguments ")
		CHECK_FUNCTION (function, string ("freeArgList"))
		message	+= UTF8String (" de la fonction ", charset)
		        + string (function->_name) + " : ";

		vector<FunctionArgument>&	args	= getArgs (*function);
		args.clear ( );
	COMPLETE_SET_GET_TRY_CATCH_BLOCK
}	// freeArgList

};	// extern "C"

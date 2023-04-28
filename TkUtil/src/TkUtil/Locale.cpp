#include "TkUtil/Locale.h"
#include "TkUtil/Process.h"
#include "TkUtil/Exception.h"
#include "TkUtil/MemoryError.h"
#include "TkUtil/NumericConversions.h"


#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <memory>


USING_STD


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


static const char* typeToTypeVar (Locale::TYPE type);
static int typeToTypeIVar (Locale::TYPE type);
static bool useSetLocale (Locale::ORIGIN origin, int pass);


/**
 * Mémo :
 *
 * <P>Les locales sont définies comme suit :<BR>
 * langage[_territoire][.jeu_de_caractères][@modifieur]<BR>
 * 
 * Mais on peut voir des locales comme C, iso_8859_1, ...
 * </P>
 * <PListe des locales : <I>locale -a</I>
 * </P>
 * <P>Les programmes sont supposés affecter "C" à toutes les variables
 * LC_.
 * </P>
 */

const char* Locale::defaultLocale	= "C";


Locale::Locale ( )
{
	assert (0 && "Locale::Locale is not allowed.");
}	// Locale::Locale


Locale::Locale (const Locale&)
{
	assert (0 && "Locale::Locale is not allowed.");
}	// Locale::Locale


Locale& Locale::operator = (const Locale&)
{
	assert (0 && "Locale::operator = is not allowed.");
	return *this;
}	// Locale::operator =


Locale::~Locale ( )
{
	assert (0 && "Locale::~Locale is not allowed.");
}	// Locale::~Locale


string Locale::toStr (ORIGIN origin)
{
	string	infos;
	infos	+= "Langage                       : " 
			+ getLanguage (origin) + "\n";
	infos	+= "Territoire                    : " + 
			getTerritory (ALL, origin) + "\n";
	infos	+= "Type de chaines de caracteres : " 
			+ getLocale (C_TYPE, origin) + "\n";
	infos	+= "Jeu de caracteres             : " 
			+ getCharset (origin) + "\n";
	infos	+= string ("Jeu de caractères ISO 8859    : ")
			+ (true == isIsoCharset (origin) ? "oui" : "non") + "\n";
	if (true == isIsoCharset (origin))
		infos	+= string ("Chaine normalisée du jeu      : ") + 
				   getNormalizedIsoString (origin) + string ("\n");
	infos	+= "Numerique                     : " 
			+ getLocale (NUMERIC, origin) + "\n";
	infos	+= "Séparateur décimal numérique  : " 
			+ getDecimalPoint (origin) + "\n";
	infos	+= "Heure/Date                    : " 
			+ getLocale (TIME, origin) + "\n";
	infos	+= "Ordre de comparaison          : " 
			+ getLocale (COLLATE, origin) + "\n";
	infos	+= "Monnaie                       : " 
			+ getLocale (MONETARY, origin) +"\n";
	infos	+= "Messages                      : " 
			+ getLocale (MESSAGES, origin) + "\n";
	
	return infos;
}	// Locale::toStr


string Locale::getLocale (Locale::TYPE type, ORIGIN origin)
{
	return getVar (type, origin);
}	// Locale::getLocale


void Locale::setLocale (Locale::TYPE type, const string& locale)
{
	char*	result	= setlocale (typeToTypeIVar (type), locale.c_str ( ));

	if (0 == result)
	{
		UTF8String	msg (charset);
		msg << "Erreur de changement de locale pour la variable "
		    << typeToTypeVar (type)
		    << ". Valeur " + locale + " non supportée.";
		throw Exception (UTF8String (msg, charset));
	}	// if (0 == result)
}	// Locale::setLocale


string Locale::getLanguage (ORIGIN origin)
{
	return getLanguage (ALL, origin);
}	// Locale::getLanguage


string Locale::getLanguage (Locale::TYPE type, ORIGIN origin)
{
	string	var (getVar (type, origin));

	string::size_type	underscore	= var.find ('_');
	string::size_type	dot			= var.find ('.');
	string::size_type	arobas		= var.find ('@');
	string::size_type	last		= string::npos;
	if (arobas != string::npos)
		last	= arobas - 1;
	if (dot != string::npos)
		last	= dot;
	if (underscore != string::npos)
		last	= underscore;

	return var.substr (0, last);
}	// Locale::getLanguage (Locale::TYPE)


string Locale::getTerritory (Locale::TYPE type, ORIGIN origin)
{
	string	var (getVar (type, origin));

	string::size_type	underscore	= var.find ('_');
	if ((string::npos == underscore) || ((underscore + 1) >= var.length ( )))
		return defaultLocale;

	string::size_type	dot			= var.find ('.');
	string::size_type	arobas		= var.find ('@');
	string::size_type	last		= string::npos;
	if (arobas != string::npos)
		last	= arobas - 1;
	if (dot != string::npos)
		last	= dot - 1;

	return var.substr (underscore + 1, last - underscore);
}	// Locale::getTerritory


string Locale::getCharset (ORIGIN origin)
{
	return getCodeset (C_TYPE, origin);
}	// Locale::getCharset


Charset Locale::detectCharset (const string& refString)	// v 4.0.0
{
	// refString est supposé contenir "àéèùô" (cf. classe Charset).
	bool	detected	= true;
	if (0 != refString.length ( ))
	{
		detected	= false;
		Charset	charset (refString.c_str ( ));
		if (Charset::UNKNOWN != charset.charset ( ))
			return charset;

		cerr << "ATTENTION : jeu de caracteres non detecte a partir de la "
		     << "chaine de caracteres de reference. Tentative de detection "
		     << "depuis l'environnement ..." << endl;
	}	// if (0 != refString.length ( ))

	const string	strCharset	= getCharset (USER_ENV);
	Charset	charset (Charset::UNKNOWN);
	if (true == isIsoCharset (USER_ENV))
		charset	= Charset (Charset::ISO_8859);
	else if ((0 == strcasecmp (strCharset.c_str ( ), "C")) ||
	         (0 == strcasecmp (strCharset.c_str ( ), "posix")))
		charset	= Charset (Charset::ASCII);
	else if ((0 == strcasecmp (strCharset.c_str ( ), "utf8")) ||
	         (0 == strcasecmp (strCharset.c_str ( ), "utf-8")))
		charset	= Charset (Charset::UTF_8);
	else if ((0 == strcasecmp (strCharset.c_str ( ), "utf16")) ||
	         (0 == strcasecmp (strCharset.c_str ( ), "utf-16")))
		charset	= Charset (Charset::UTF_16);
	if (false == detected)
	{
		if (Charset::UNKNOWN != charset.charset ( ))
			cerr << "Jeu caracteres detecte dans l'environnement : "
			     << charset.name ( ) << endl;
		else
			cerr << "Jeu caracteres non detecte a partir de l'environnement."
			     << endl;
	}	// if (false == detected)

	return charset;
}	// Locale::detectCharset


bool Locale::isIsoCharset  (ORIGIN origin)
{
	string	charset	= getCharset (origin);

	if (0 == strncasecmp (charset.c_str ( ), "iso8859", 7))
		return true;
	if (0 == strncasecmp (charset.c_str ( ), "euro", 4))	// v 4.0.0
		return true;

	return false;
}	// Locale::isIsoCharset


size_t Locale::getIsoCharSetPart (ORIGIN origin)
{
	if (false == isIsoCharset (origin))
		throw Exception (UTF8String ("Locale::getIsoCharSetPart : le jeu de caractères n'est pas un jeu ISO 8859.", charset));

	string		cs	= getCharset (origin);
	const char*	ptr		= cs.c_str ( ) + 7;

	switch (*ptr)
	{
		case ' ' :
		case '-' :
		case '_' :
			ptr++;
	}	// switch (*ptr)

	try
	{
		return	NumericConversions::strToULong (ptr);
	}
	catch (...)
	{
		throw Exception (UTF8String ("Locale::getIsoCharSetPart : impossible d'extraire la partie du jeu ISO.", charset));
	}
}	// Locale::getIsoCharSetPart


string Locale::getNormalizedIsoString (ORIGIN origin)
{
	if (false == isIsoCharset (origin))
		throw Exception (UTF8String ("Locale::getNormalizedIsoString : le jeu de caractères n'est pas un jeu ISO 8859.", charset));

	UTF8String	completeCharset;
	completeCharset << "ISO-8859";

	try
	{
		size_t	part	= getIsoCharSetPart (origin);
		completeCharset << "-" << (unsigned long)part;
	}
	catch (...)
	{
	}

	return completeCharset.iso ( );
}	// Locale::getNormalizedIsoString


string Locale::getCodeset (Locale::TYPE type, ORIGIN origin)
{
	string	var (getVar (type, origin));

	string::size_type	underscore	= var.find ('_');
	string::size_type	dot			= var.find ('.');
//	if (string::npos == dot)	// v 4.0.0 Ex : fr_FR@euro
//		return defaultLocale;	// v 4.0.0
		
	string::size_type	arobas		= var.find ('@');
	string::size_type	last		= var.length ( );
	if (arobas != string::npos)
		last	= arobas - 1;

	if ((string::npos == dot) && (string::npos == arobas))	// v 4.0.0
		return defaultLocale;	// v 4.0.0
	if (string::npos == dot)	// v 4.0.0
		dot	= arobas;			// v 4.0.0

	return var.substr (dot + 1, last - dot);
}	// Locale::getCodeset


string Locale::getModifier (TYPE type, ORIGIN origin)
{
	string	var (getVar (type, origin));

	string::size_type	arobas		= var.find ('@');
	if ((arobas != string::npos) && (arobas + 1 < var.length ( )))
		return  var.substr (arobas + 1);

	return defaultLocale;
}	// Locale::getModifier


string Locale::getDecimalPoint (ORIGIN)
{
	struct lconv*	convertParams	= localeconv ( );
	if (0 == convertParams)
		throw Exception (UTF8String ("Locale::getDecimalPoint : structure lconv nulle.", charset));
	if (0 == convertParams->decimal_point)	// Ne doit pas arriver
		throw Exception (UTF8String ("Locale::getDecimalPoint : champ decimal_point de la structure lconv nul.", charset));

	return convertParams->decimal_point;
}	// Locale::getDecimalPoint


vector<string> Locale::getAvailableLocales ( )
{
	vector<string>		locales;
	unique_ptr<Process>	process (new Process ("locale"));
	CHECK_NULL_PTR_ERROR (process.get ( ))

	process->getOptions ( ).addOption ("-a");
	process->enableChildToSonCommunications ( );

	try
	{
		process->execute (false);
		string	aLocale	= process->getChildLine ( );
		while (0 != aLocale.length ( ))
		{
			locales.push_back (aLocale);
			aLocale	= process->getChildLine ( );
		}	
		process->wait ( );
	}
	catch (...)
	{
		if (0 != process->getCompletionCode ( ))
		{
			UTF8String	msg (charset);
			msg << "Locale::getAvailableLocales, erreur : "
			    << process->getErrorMessage ( );
			throw Exception (UTF8String (msg, charset));
		}	// if (0 != process->getCompletionCode ( ))
	}

	return locales;
}	// Locale::getAvailableLocales


UTF8String Locale::setUserLocales ( )
{
	UTF8String	msg (charset);

	TYPE	type	= C_TYPE;
	string	env		= getLocale (type, USER_ENV);
	string	exe		= getLocale (type, EXE);
	if (env != exe)
	{
		setLocale (type, env);
		msg	+= env + " affectée à la variable " + typeToTypeVar (type) + ".";
	}

	type	= COLLATE;
	env		= getLocale (type, USER_ENV);
	exe		= getLocale (type, EXE);
	if (env != exe)
	{
		setLocale (type, env);
		if (0 != msg.length ( ))
			msg	+= '\n';
		msg	+= env + " affectée à la variable " + typeToTypeVar (type) + ".";
	}

	type	= TIME;
	env		= getLocale (type, USER_ENV);
	exe		= getLocale (type, EXE);
	if (env != exe)
	{
		setLocale (type, env);
		if (0 != msg.length ( ))
			msg	+= '\n';
		msg	+= env + " affectée à la variable " + typeToTypeVar (type) + ".";
	}

	type	= NUMERIC;
	env		= getLocale (type, USER_ENV);
	exe		= getLocale (type, EXE);
	if (env != exe)
	{
		setLocale (type, env);
		if (0 != msg.length ( ))
			msg	+= '\n';
		msg	+= env + " affectée à la variable " + typeToTypeVar (type) + ".";
	}

	type	= MONETARY;
	env		= getLocale (type, USER_ENV);
	exe		= getLocale (type, EXE);
	if (env != exe)
	{
		setLocale (type, env);
		if (0 != msg.length ( ))
			msg	+= '\n';
		msg	+= env + " affectée à la variable " + typeToTypeVar (type) + ".";
	}

	type	= MESSAGES;
	env		= getLocale (type, USER_ENV);
	exe		= getLocale (type, EXE);
	if (env != exe)
	{
		setLocale (type, env);
		if (0 != msg.length ( ))
			msg	+= '\n';
		msg	+= env + " affectée à la variable " + typeToTypeVar (type) + ".";
	}

	return msg;
}	// Locale::setUserLocales


string Locale::getVar (TYPE type, Locale::ORIGIN origin)
{
	// Recherche en une passe si origin vaut EXE ou USER_ENV, sinon en 2 passes
	// setlocal, utilisée ici en consultation, effectue la recherche 
	// catégorielle.
	for (int pass = 0; pass < 2; pass++)
	{	
		const bool	useLocale	= useSetLocale (origin, pass);
		const char*	allVar		= typeToTypeVar (ALL);
		// En cas d'usage de setlocal, on passe directement type en arguement
		// car setlocale se charge d'effectuer la recherche catégorielle.
		int			iVar		= typeToTypeIVar (type);
		const char*	all			= true == useLocale ?
								  setlocale (iVar, 0) : getenv (allVar);
		if (0 != all)
			return all;
		if (true == useLocale)
			continue;

		const char*	typeVar	= typeToTypeVar (type);
		iVar				= typeToTypeIVar (type);
// coverity : useLocale != true (cf. ci-dessus)
//		const char*	t		= true == useLocale ?
//							  setlocale (iVar, 0) : getenv (typeVar);
		const char*	t	= getenv (typeVar);
		if (0 != t)
			return t;

		const char*	lang	= getenv ("LANG");
		if (0 != lang)
			return lang;
	}	// for (int pass = 0; pass < 2; pass++)

	return defaultLocale;
}	// Locale::getVar


static const char* typeToTypeVar (Locale::TYPE type)
{
	static const char* types [7]	=
		{ "LC_ALL", "LC_CTYPE", "LC_COLLATE", "LC_TIME", "LC_NUMERIC", 
		  "LC_MONETARY", "LC_MESSAGES" };

	assert (((int)type >= 0) && ((int)type <= 6));
	return types [(int)type];
}	// typeToTypeVar


static int typeToTypeIVar (Locale::TYPE type)
{
	assert (((int)type >= 0) && ((int)type <= 6));

	switch (type)
	{
		case Locale::C_TYPE		: return LC_CTYPE;
		case Locale::COLLATE	: return LC_COLLATE;
		case Locale::TIME		: return LC_TIME;
		case Locale::NUMERIC	: return LC_NUMERIC;
		case Locale::MONETARY	: return LC_MONETARY;
		case Locale::MESSAGES	: return LC_MESSAGES;
		default					: 
		case Locale::ALL		: return LC_ALL;
	}	// switch (type)
}	// typeToTypeIVar


static bool useSetLocale (Locale::ORIGIN origin, int pass)
{
	if (Locale::EXE == origin)
		return true;
	if (Locale::USER_ENV == origin)
		return false;

	assert (Locale::BOTH == origin);
	assert ((0 <= pass) && (2 > pass));
	return 0 == pass ? true : false;
}	// useSetLocale


END_NAMESPACE_UTIL

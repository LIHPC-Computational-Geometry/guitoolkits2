#include "TkUtil/NumericConversions.h"
#include "TkUtil/NumericServices.h"
#include "TkUtil/Exception.h"
#include "TkUtil/UnicodeString.h"

#include <assert.h>
#include <cstdlib>
#ifndef USE_OLD_STRING_STREAM
#include <sstream>
#else	// USE_OLD_STRING_STREAM
#include <strstream>
#endif	// USE_OLD_STRING_STREAM
#include <iomanip>


USING_STD


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


bool hasLeadingMinus (const string& str)
{
	// On ne teste pas ici la validité numérique de la chaine, mais juste si
	// elle commence par le caractère '-' avant les chiffres.
	const char*	ptr	= str.c_str ( );

	while ('\0' != *ptr)
	{
		if (0 != isdigit (*ptr))
			return false;
		else if ('-' == *ptr)
			return true;

		ptr++;
	}	// while ('\0' != *ptr)

	return false;
}	// hasLeadingMinus


#ifdef TOSTR_USES_CPP_IMPLEMENTATION

/**
 * Code suivant : utilise les flux C++.
 * Problème lors du portage Cent OS 7 / gcc 6.4 -D_GLIBCXX_USE_CXX11_ABI=0,
 * ça jardine dans la libstdc++ fournie avec g++ ...
 * Bogue de cette bibliothèque ??? D'après valgrind 3.13.0 --tool=memcheck la
 * première erreur rencontrée est au sein de la lib stdc++.so.6
 */
template <class T> string numberToString (T t)
{
#ifndef USE_OLD_STRING_STREAM
	ostringstream	stream;
	stream << t;
	static	string	str	= stream.str ( );
	return stream.str ( );
#else	// USE_OLD_STRING_STREAM
	ostrstream	stream;
	stream << t << ends;
	return stream.str ( );
#endif	// USE_OLD_STRING_STREAM
}	// numberToString (T t)


template <class T> string numberToString (T t, int width)
{
#ifndef USE_OLD_STRING_STREAM
	ostringstream	stream;
	stream << setfill ('0') << IN_STD setw (width) << t;
	return stream.str ( );
#else	// USE_OLD_STRING_STREAM
	ostrstream	stream;
	stream << setfill ('0') << IN_STD setw (width) << t << ends;
	return stream.str ( );
#endif	// USE_OLD_STRING_STREAM
}	// numberToString (T t, int width)


template <class T> string numberToString (
								T t, int precision, ios_base::fmtflags sc)
{
#ifndef USE_OLD_STRING_STREAM
	ostringstream	stream;
	stream.setf (sc, ios::floatfield);
	stream << IN_STD setprecision (precision) << t;
#else	// USE_OLD_STRING_STREAM
	ostrstream	stream;
	stream.setf (sc, ios::floatfield);
	stream << IN_STD setprecision (precision) << t << ends;
#endif	// USE_OLD_STRING_STREAM

	return stream.str ( );
}	// numberToString (T t, int precision, fmtflags sc)


template <class T> string numberToString (
						T t, int precision, int width, ios_base::fmtflags sc)
{
#ifndef USE_OLD_STRING_STREAM
	ostringstream	stream;
	stream.setf (sc, ios::floatfield);
	stream << IN_STD setw (width) << IN_STD setfill (' ')
	       << IN_STD setprecision (precision) << t;
#else	// USE_OLD_STRING_STREAM
	ostrstream	stream;
	stream.setf (sc, ios::floatfield);
	stream << IN_STD setw (width) << IN_STD setfill (' ')
	       << IN_STD setprecision (precision) << t << ends;
#endif	// USE_OLD_STRING_STREAM

	return stream.str ( );
}	// numberToString (T t, int precision, int width, fmtflags sc)

#endif	// TOSTR_USES_CPP_IMPLEMENTATION


NumericConversions::NumericConversions ( )
{
	assert (0 && "NumericConversions::NumericConversions : class not instanciable.");
}	// NumericConversions::NumericConversions


NumericConversions::NumericConversions (const NumericConversions&)
{
	assert (0 && "NumericConversions::NumericConversions : class not instanciable.");
}	// NumericConversions::NumericConversions


NumericConversions& NumericConversions::operator = (const NumericConversions&)
{
	assert (0 && "NumericConversions::operator = : class not instanciable.");
	return *this;
}	// NumericConversions::operator =


NumericConversions::~NumericConversions ( )
{
	assert (0 && "NumericConversions::~NumericConversions : class not instanciable.");
}	// NumericConversions::~NumericConversions


long NumericConversions::strToLong (const string& str)
{
	if (0 == str.length ( ))
		throw Exception (UTF8String ("Conversion en entier long impossible : chaîne vide.", charset));

	char*	ptr		= (char*)str.c_str ( );
	char*	endptr	= ptr;
	long	val		= strtol (ptr, &endptr, 10);

	if ('\0' != *endptr)
	{
		string	error ("Conversion impossible de ");
		error	+= str + " en entier long.";
		throw Exception (UTF8String (error, charset));
	}	// if ('\0' != *endptr)

	return val;
}	// NumericConversions::strToLong


unsigned long NumericConversions::strToULong (const string& str)
{
	if (0 == str.length ( ))
		throw Exception (UTF8String ("Conversion en entier long non signé impossible : chaîne vide.", charset));

	// Version 2.11.1 : on teste nous même le signe car la fonction strtoul
	// ne le fait pas sous Linux ou Sun :
	// If  there  was  a leading minus sign, the negation of the result of the
	// conversion represented as an unsigned value.
	char*			ptr		= (char*)str.c_str ( );
	char*			endptr	= ptr;
	unsigned long	val		= strtoul (ptr, &endptr, 10);

	if (('\0' != *endptr) || (true == hasLeadingMinus (str)))
	{
		string	error ("Conversion impossible de ");
		error	+= str + " en entier long non signé.";
		throw Exception (UTF8String (error, charset));
	}	// if (('\0' != *endptr) || (true == hasLeadingMinus (str)))

	return val;
}	// NumericConversions::strToULong


long long NumericConversions::strToLongLong (const string& str)
{
	if (0 == str.length ( ))
		throw Exception (UTF8String ("Conversion en entier long long impossible : chaîne vide.", charset));

	char*		ptr		= (char*)str.c_str ( );
	char*		endptr	= ptr;
	long long	val		= strtoll (ptr, &endptr, 10);

	if ('\0' != *endptr)
	{
		string	error ("Conversion impossible de ");
		error	+= str + " en entier long long.";
		throw Exception (UTF8String (error, charset));
	}	// if ('\0' != *endptr)

	return val;
}	// NumericConversions::strToLongLong


unsigned long long NumericConversions::strToULongLong (const string& str)
{
	if (0 == str.length ( ))
		throw Exception (UTF8String ("Conversion en entier long long non signé impossible : chaîne vide.", charset));

	// Version 2.11.1 : on teste nous même le signe car la fonction strtoull
	// ne le fait pas sous Linux ou Sun :
	// If  there  was  a leading minus sign, the negation of the result of the
	// conversion represented as an unsigned value.
	char*				ptr		= (char*)str.c_str ( );
	char*				endptr	= ptr;
	unsigned long long	val		= strtoull (ptr, &endptr, 10);

	if (('\0' != *endptr) || (true == hasLeadingMinus (str)))
	{
		string	error ("Conversion impossible de ");
		error	+= str + " en entier long long non signé.";
		throw Exception (UTF8String (error, charset));
	}	// if (('\0' != *endptr) || (true == hasLeadingMinus (str)))

	return val;
}	// NumericConversions::strToULongLong


double NumericConversions::strToDouble (const string& str)
{
	if (0 == str.length ( ))
		throw Exception (UTF8String ("Conversion en réel impossible : chaîne vide.", charset));

	char*	ptr		= (char*)str.c_str ( );
	char*	endptr	= ptr;
	double	val		= strtod (ptr, &endptr);

	if ('\0' != *endptr)
	{
		string	error ("Conversion impossible de ");
		error	+= str + " en réel double précision.";
		throw Exception (UTF8String (error, charset));
	}	// if ('\0' != *endptr)

	return val;
}	// NumericConversions::strToDouble


long double NumericConversions::strToLongDouble (const string& str)
{
	if (0 == str.length ( ))
		throw Exception (UTF8String ("Conversion en réel long impossible : chaîne vide.", charset));

	char*		ptr		= (char*)str.c_str ( );
	char*		endptr	= ptr;
	long double	val		= strtold (ptr, &endptr);

	if ('\0' != *endptr)
	{
		string	error ("Conversion impossible de ");
		error	+= str + " en réel long.";
		throw Exception (UTF8String (error, charset));
	}	// if ('\0' != *endptr)

	return val;
}	// NumericConversions::strToDouble


string NumericConversions::toStr (short s)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<short>(s);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	str [1025] = { '\0' };
	sprintf (str, "%hd", s);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (short)


string NumericConversions::toStr (short s, int width)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<short>(s, width);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s#0%d%s", "%", width, "hd");
	sprintf (str, format, s);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (short, int)


string NumericConversions::toStr (unsigned short us)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<unsigned short>(us);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	str [1025] = { '\0' };
	sprintf (str, "%hu", us);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (unsigned short)


string NumericConversions::toStr (unsigned short us, int width)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<unsigned short>(us, width);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s#0%d%s", "%", width, "hu");
	sprintf (str, format, us);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (unsigned short, int)


string NumericConversions::toStr (long l)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<long>(l);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	str [1025] = { '\0' };
	sprintf (str, "%ld", l);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (long)


string NumericConversions::toStr (long l, int width)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<long>(l, width);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s#0%d%s", "%", width, "ld");
	sprintf (str, format, l);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (long, int)


string NumericConversions::toStr (unsigned long ul)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<unsigned long>(ul);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	str [1025] = { '\0' };
	sprintf (str, "%lu", ul);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (unsigned long)


string NumericConversions::toStr (unsigned long ul, int width)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<unsigned long>(ul, width);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s#0%d%s", "%", width, "lu");
	sprintf (str, format, ul);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (unsigned long, int)


string NumericConversions::toStr (long long l)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<long long>(l);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	str [1025] = { '\0' };
	sprintf (str, "%lld", l);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (long long)


string NumericConversions::toStr (long long l, int width)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<long long>(l, width);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s#0%d%s", "%", width, "lld");
	sprintf (str, format, l);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (long long, int)


string NumericConversions::toStr (unsigned long long ul)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<unsigned long long>(ul);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	str [1025] = { '\0' };
	sprintf (str, "%llu", ul);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (unsigned long long)


string NumericConversions::toStr (unsigned long long ul, int width)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<unsigned long long>(ul, width);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s#0%d%s", "%", width, "llu");
	sprintf (str, format, ul);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (unsigned long long, int)


string NumericConversions::toStr (float f)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<float>(f);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	str [1025] = { '\0' };
	sprintf (str, "%g", (double)f);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (float)


string NumericConversions::toStr (float f, int precision, bool scientific)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<float>(f, precision, 
				true == scientific ? ios_base::scientific : ios_base::fixed);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s.%d%s", "%", (precision+1), true == scientific ? "g" : "f");
	sprintf (str, format, (double)f);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (float, int, bool)


string NumericConversions::toStr (
							float f, int precision, int width, bool scientific)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<float>(f, precision, width,
				true == scientific ? ios_base::scientific : ios_base::fixed);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s#0%d.%d%s", "%", width, (precision+1), true == scientific ? "g" : "f");
	sprintf (str, format, (double)f);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (float, int, int, bool)


string NumericConversions::toStr (double d)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<double>(d);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	str [1025] = { '\0' };
	sprintf (str, "%g", d);	// v 4.4.1, previously %Lg
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (double)


string NumericConversions::toStr (double d, int precision, bool scientific)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<double>(d, precision, 
					true== scientific ? ios_base::scientific : ios_base::fixed);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s.%d%s", "%", (precision+1), true == scientific ? "g" : "f");
	sprintf (str, format, d);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (double, int, bool)



string NumericConversions::toStr (
							double d, int precision, int width, bool scientific)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<double>(d, precision, width,
					true== scientific ? ios_base::scientific : ios_base::fixed);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s#0%d.%d%s", "%", width, (precision+1), true == scientific ? "g" : "f");
	sprintf (str, format, d);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (double, int, int, bool)


string NumericConversions::toStr (long double d)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<long double>(d);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	str [1025] = { '\0' };
	sprintf (str, "%Lg", d);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (long double)


string NumericConversions::toStr (long double d, int precision, bool scientific)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<long double>(d, precision, 
					true== scientific ? ios_base::scientific : ios_base::fixed);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s.%d%s", "%", (precision+1), true == scientific ? "Lg" : "Lf");
	sprintf (str, format, d);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (long double, int, bool)


string NumericConversions::toStr (
					long double d, int precision, int width, bool scientific)
{
#ifdef TOSTR_USES_CPP_IMPLEMENTATION
	return numberToString<long double>(d, precision, width,
					true== scientific ? ios_base::scientific : ios_base::fixed);
#else	// TOSTR_USES_CPP_IMPLEMENTATION
	char	format [20] = { '\0' };
	char	str [1025] = { '\0' };
	sprintf (format, "%s#0%d.%d%s", "%", width, (precision+1), true == scientific ? "Lg" : "Lf");
	sprintf (str, format, d);
	return string (str);
#endif	// TOSTR_USES_CPP_IMPLEMENTATION
}	// NumericConversions::toStr (long double, int, int, bool)


static string withoutDot (const string& number)
{
	UnicodeString		integerPart, decimalPart, exponantPart;
	bool				ip	= true;
	string::size_type	i	= 0;
	for (i = 0; i < number.length ( ); i++)
	{
		if (0 != isdigit (number [i]))
		{
			if (true == ip)
				integerPart << number [i];
			else
				decimalPart << number [i];
		}
		else if ('.' == number [i])
		{
			if (true == ip)
				ip	= false;
			else
			{
				UTF8String	error (charset);
				error << "Syntaxe du nombre décimal " << number << " invalide.";
				throw Exception (error);
			}
		}
		else if (('-' == number [i]) && (0 == i))
			integerPart << number [i];
		else if (('+' == number [i]) && (0 == i))
			;
		else if (('e' == number [i]) || ('E' == number [i]))
		{
			exponantPart	= withoutDot (number.substr (i + 1));
			break;
		}	// else if (('e' == number [i]) || ('E' == number [i]))
		else
		{
			UTF8String	error (charset);
			error << "Syntaxe du nombre décimal " << number << " invalide.";
			throw Exception (error);
		}
	}	// for (i = 0; i < number.length ( ); i++)

	UnicodeString	notDoted;
	notDoted << integerPart << decimalPart;
	long	exp	= 0;
	if (0 != exponantPart.length ( ))
		exp	= NumericConversions::strToLong (exponantPart);
	if (0 != decimalPart.length ( ))
		exp	-= decimalPart.length ( );
	if (0 != exp)
		notDoted << "e" << exp;

	// On veille à ne pas retourner de 0 en tête :
	const string	str	= notDoted.iso ( );
	if ((1 < str.length ( )) && ('0' == str [0]))
	{
		for (i = 0; (i < str.length ( )) && ('0' == str [i]); i++);

		notDoted.clear ( );
		notDoted << str.substr (i);
	}	// if ((1 < str.length ( )) && ('0' == str [0])))

	return notDoted.iso ( );
}	// withoutDot


string NumericConversions::userRepresentation (
			double d, unsigned short precision, unsigned short integerPartNum)
{
/* Marche pas trop mal en dehors de la bascule
 * scientifique --integerPartNum--> fixed :
#ifndef USE_OLD_STRING_STREAM
ostringstream  s;
s << std::setprecision (precision)
  << std::setw (integerPartNum) << d;
#else	// USE_OLD_STRING_STREAM
ostrstream  s;
s << std::setprecision (precision)
  << std::setw (integerPartNum) << d << ends;
#endif	// USE_OLD_STRING_STREAM
return s.str ( );
*/

	// On veut au moins 1 chiffre en partie entière :
	if (0 == integerPartNum)
		integerPartNum	= 1;

// Pb : si on modifie maintenant la précision ou la notation (-> scientifique)
// on aura dans certains cas des .999999999999
// => On effectuera ces modifications nous même à la fin, si nécessaire.
//	stream.setf (ios_base::scientific, ios::floatfield);
// ============================================================================
//	std::setprecision (NumericServices::doubleDigitsMax) << std::setw (1) << d << ends
//	n'altère pas la représentation du double
// ============================================================================
#ifndef USE_OLD_STRING_STREAM
	ostringstream  stream;
	stream << std::setprecision (NumericServices::doubleDigitsMax)
	       << std::setw (1) << d;
#else	// USE_OLD_STRING_STREAM
	ostrstream  stream;
	stream << std::setprecision (NumericServices::doubleDigitsMax)
	       << std::setw (1) << d << ends;
#endif	// USE_OLD_STRING_STREAM

	string	str	= stream.str ( );
	// On transforme la chaine sous la forme : IIIII[eEEE]
	// où I sont des chiffres de la partie entière et E de l'exposant.
	// => simplification de la représentation.
	str			= withoutDot (str);

	// Y a-t-il un signe - ?
	const bool	minus	= '-' == str [0] ? true : false;
	if (true == minus)
		str	= str.substr (1);

	string	intPart, exp;
	string::size_type	expPos	= str.find ('e');
	if (string::npos == expPos)
		expPos	= str.find ('E');
	if (string::npos != expPos)
		intPart	= str.substr (0, expPos);
	else
		intPart	= str;
	if (string::npos != expPos)
		exp	= str.substr (expPos);
	if (1 < exp.length ( ))
		exp	= exp.substr (1);	// On élimine le e/E => nombre
	long	e	= 0 == exp.length ( ) ?  0 : NumericConversions::strToLong(exp);

	// precision est le nombre de chiffres significatifs, à savoir derrière la
	// virgule.
	// Ici on raisonne en nombre de chiffres. En cas de notation scientifique
	// c'est 1 avant virgule, le reste apres => incrémenter.
// coverity : risque d'overflow => passage aux valeurs absolues
//	if ((e*e) > (integerPartNum*integerPartNum))
	if (labs (e) > labs (integerPartNum))
		precision	+= 1;

	// On veut precision chiffres significatifs => On remplace par 0 les autres.
	// intPart [precision] est incrémenté si intPart [precision+1] >= 5.
	if (intPart.length ( ) > precision)
	{
		string			kept	= intPart.substr (0, precision);
		string			rejected= intPart.substr (precision);
		unsigned long	ul		= NumericConversions::strToULong (kept);
		if ('5' <= rejected [0])
			ul++;
		UnicodeString	us (NumericConversions::toStr (ul));
		for (string::size_type i = 0; i < rejected.length ( ); i++)
			us << '0';
		intPart	= us.iso ( );
	}	// if (intPart.length ( ) > precision)

	UnicodeString	numbers;
	numbers << intPart;
	long	l	= NumericConversions::strToLong (numbers.iso ( ));

	// Si on passe en notation scientifique la partie entière est de taille 1 :
// coverity : risque d'overflow => passage aux valeurs absolues
//	if ((e*e) > (integerPartNum*integerPartNum))
	if (labs (e) > labs (integerPartNum))
		integerPartNum	= 1;

	// On a le nombre de départ, représenté ici sans partie décimale : lEe
	// On travaille sur la partie entière, élimination des 0 inutiles :	
	intPart	= NumericConversions::toStr (l);

	// On exprime maintenant le nombre avec une virgule après integerPartNum
	// chiffres :
	UnicodeString	userRep;
	if (true == minus)
		userRep << '-';
	string::size_type	pos	= 0;
// coverity : risque d'overflow => passage aux valeurs absolues
//	if ((e < 0) && ((e*e) <= (integerPartNum*integerPartNum)))
	if ((e < 0) && (labs (e) <= labs (integerPartNum)))
	{
		// Un pré-remplissage avec des 0 est il nécessaire ? ex : 0.0012
		if (intPart.length ( ) < -e)
		{
			userRep << "0.";
			string::size_type	fill	= -e - intPart.length ( );
			for (pos = 0; pos < fill; pos++)
				 userRep << '0';
			for (pos = 0; pos < intPart.length ( ); pos++)
				userRep << intPart [pos];
		}	// if (intPart.length ( ) < -e)
		else
		{
			for (pos = 0; pos < intPart.length ( ) + e; pos++)
				userRep << intPart [pos];
			if (pos < intPart.length ( ))
				userRep << '.';
			for (; pos < intPart.length ( ); pos++)
				userRep << intPart [pos];
		}	// else if (intPart.length ( ) < -e)
	}
	else
	{
		for (pos = 0;
				(pos < integerPartNum) && (pos < intPart.length ( )); pos++)
			userRep << intPart [pos];
		e	+= (intPart.length ( ) - pos);
		if (pos < intPart.length ( ))
			userRep << '.';
		for (; pos < intPart.length ( ); pos++)
			userRep << intPart [pos];
		if (0 != e)
			userRep << 'e' << e;
	}

	return userRep.iso ( );
}	// NumericConversions::userRepresentation


string NumericConversions::shortestRepresentation (
			double d, unsigned short precision, unsigned short integerPartNum)
{ // Idem userRepresentation mais avec un minimum de 0 inutiles.
	// Mémorisation des attentes du demandeur :
	const unsigned short	requestedPrecision	= precision;
	const unsigned short	requestedIntPartNum	= integerPartNum;

	// On veut au moins 1 chiffre en partie entière :
	if (0 == integerPartNum)
		integerPartNum	= 1;

// Pb : si on modifie maintenant la précision ou la notation (-> scientifique)
// on aura dans certains cas des .999999999999
// => On effectuera ces modifications nous même à la fin, si nécessaire.
//	stream.setf (ios_base::scientific, ios::floatfield);
// ============================================================================
//	std::setprecision (NumericServices::doubleDigitsMax) << std::setw (1) << d << ends
//	n'altère pas la représentation du double
// ============================================================================
#ifndef USE_OLD_STRING_STREAM
	ostringstream  stream;
	stream << std::setprecision (NumericServices::doubleDigitsMax)
	       << std::setw (1) << d;
#else	// USE_OLD_STRING_STREAM
	ostrstream  stream;
	stream << std::setprecision (NumericServices::doubleDigitsMax)
	       << std::setw (1) << d << ends;
#endif	// USE_OLD_STRING_STREAM

	string	str	= stream.str ( );
	// On transforme la chaine sous la forme : IIIII[eEEE]
	// où I sont des chiffres de la partie entière et E de l'exposant.
	// => simplification de la représentation.
	str			= withoutDot (str);

	// Y a-t-il un signe - ?
	const bool	minus	= '-' == str [0] ? true : false;
	if (true == minus)
		str	= str.substr (1);

	string	intPart, exp;
	string::size_type	expPos	= str.find ('e');
	if (string::npos == expPos)
		expPos	= str.find ('E');
	if (string::npos != expPos)
		intPart	= str.substr (0, expPos);
	else
		intPart	= str;
	if (string::npos != expPos)
		exp	= str.substr (expPos);
	if (1 < exp.length ( ))
		exp	= exp.substr (1);	// On élimine le e/E => nombre
	long	e	= 0 == exp.length ( ) ?  0 : NumericConversions::strToLong(exp);

	// precision est le nombre de chiffres significatifs, à savoir derrière la
	// virgule.
	// Ici on raisonne en nombre de chiffres. En cas de notation scientifique
	// c'est 1 avant virgule, le reste apres => incrémenter.
// coverity : risque d'overflow => passage aux valeurs absolues
//	if ((e*e) > (integerPartNum*integerPartNum))
	if (labs (e) > labs (integerPartNum))
		precision	+= 1;

	// On veut au max precision chiffres significatifs => On supprime les autres
	// intPart [precision] est incrémenté si intPart [precision+1] >= 5.
	if (intPart.length ( ) > precision)
	{
		string			kept	= intPart.substr (0, precision);
		string			rejected= intPart.substr (precision);
		e	+= rejected.length ( );	// v 2.43.0
		unsigned long	ul		= NumericConversions::strToULong (kept);
		if ('5' <= rejected [0])
			ul++;
		UnicodeString	us (NumericConversions::toStr (ul));
//		for (string::size_type i = 0; i < rejected.length ( ); i++)
//			us << '0';
		intPart	= us.iso ( );
	}	// if (intPart.length ( ) > precision)

	UnicodeString	numbers;
	numbers << intPart;
	long	l	= NumericConversions::strToLong (numbers.iso ( ));

	// Si on passe en notation scientifique la partie entière est de taille 1 :
// coverity : risque d'overflow => passage aux valeurs absolues
//	if ((e*e) > (integerPartNum*integerPartNum))
//	if (labs (e) > labs (integerPartNum))	// v 3.0.0
//		integerPartNum	= 1;	// v 3.0.0
// v 3.0.0 : shortestRepresentation (98.00085, 6, 3) -> 98.00085 et non
// plus 9.800085e-1)
	if (labs (e) > (long int)integerPartNum)			// v 3.0.0
		integerPartNum	= labs (e) - integerPartNum;	// v 3.0.0

	// On a le nombre de départ, représenté ici sans partie décimale : lEe
	// On travaille sur la partie entière, élimination des 0 inutiles :	
	intPart	= NumericConversions::toStr (l);

	// On exprime maintenant le nombre avec une virgule après integerPartNum
	// chiffres :
	UnicodeString	userRep;
	if (true == minus)
		userRep << '-';
	string::size_type	pos	= 0;
// coverity : risque d'overflow => passage aux valeurs absolues
//	if ((e < 0) && ((e*e) <= (integerPartNum*integerPartNum)))
	if ((e < 0) && (labs (e) <= labs (integerPartNum)))
	{
		// Un pré-remplissage avec des 0 est il nécessaire ? ex : 0.0012
		if (intPart.length ( ) < -e)
		{
			userRep << "0.";
			string::size_type	fill	= -e - intPart.length ( );
			for (pos = 0; pos < fill; pos++)
				 userRep << '0';
			for (pos = 0; pos < intPart.length ( ); pos++)
				userRep << intPart [pos];
		}	// if (intPart.length ( ) < -e)
		else
		{
			for (pos = 0; pos < intPart.length ( ) + e; pos++)
				userRep << intPart [pos];
			if (pos < intPart.length ( ))
				userRep << '.';
			for (; pos < intPart.length ( ); pos++)
				userRep << intPart [pos];
		}	// else if (intPart.length ( ) < -e)
	}
	else
	{
		for (pos = 0;
				(pos < integerPartNum) && (pos < intPart.length ( )); pos++)
			userRep << intPart [pos];
		e	+= (intPart.length ( ) - pos);
		if (pos < intPart.length ( ))
			userRep << '.';
		for (; pos < intPart.length ( ); pos++)
			userRep << intPart [pos];
		if (0 != e)
			userRep << 'e' << e;
	}

	// On a une représentation épurée, mais dans certains cas ne correspondant
	// pas exactement à la demande.	// v 3.0.0
	d				= NumericConversions::strToDouble (userRep.iso ( ));
	// Conversion au format fixe : la partie entière est elle de taille ok ?
	string				eval	=
							NumericConversions::toStr (d, precision, false);
	string::size_type	dotPos	= eval.find ('.');
	if ((false == minus) && (string::npos != dotPos) &&
	    (dotPos <= requestedIntPartNum))
	{
		string::size_type	p	= requestedPrecision + 1 > eval.length ( ) ?
								  eval.length ( ) - 1 : requestedPrecision;
		while ((p > 0) && (eval [p] == '0'))
			p--;
		if (eval [p] == '.')
			p--;
		return eval.substr (0, p + 1);
	}
	if ((true == minus) && (string::npos != dotPos) &&
	    (dotPos <= requestedIntPartNum + 1))
	{
		string::size_type	p	= requestedPrecision + 2 > eval.length ( ) ?
								  eval.length ( ) - 1 : requestedPrecision + 1;
		while ((p > 0) && (eval [p] == '0'))
			p--;
		if (eval [p] == '.')
			p--;
		return eval.substr (0, p + 1);
	}

	return userRep.iso ( );
}	// NumericConversions::shortestRepresentation


END_NAMESPACE_UTIL

#include "TkUtil/UTF8String.h"
#include "TkUtil/AutoArrayPtr.h"
#include "TkUtil/MemoryError.h"
#include "TkUtil/NumericConversions.h"
#include "TkUtil/Exception.h"
#include "TkUtil/UnicodeString.h"

#include <string.h>		// memcpy, strstr, ...
#include <strings.h>	// strcasecmp, ...
#include <assert.h>
#include <ctype.h>		// isascii
#include <sstream>		// ostringstream


USING_STD


BEGIN_NAMESPACE_UTIL

static const  Charset	charset ("àéèùô");


//
// Mémo :
//
// ASCII :
// -------
//
// De 0 à 127
//
// ISO 8859 :
// ----------
//
// - Codé sur des unsigned char, de 0 à 255
// - Les 127 premiers caractères sont ceux du jeu ASCII
// - Les suivants varient selon 16 jeux :
//		- Latin 1 : Europe de l'Ouest, dont la France, le Royaume-Uni,
//		l'Allemagne, l'Italie, l'Espagne, ...
//		- Latin 2 : Europe centrale
//		- Latin 3 : Europe du sud
//		- Latin 4 : Europe du Nord
//		- Cyrillic : Pays slaves
//		...
//
// Unicode : (UTF-16)
// ---------
//
// - Codé de 1 un 4 octets
// - Les 256 premiers caractères unicode sont les mêmes que ceux du jeu 
// ISO 8859-1.
//

// Source : Xerces, XMLUniDefs.hpp
const unsigned char	UTF8String::maxASCIIChar		= 0x7F;		// 127
const unsigned char	UTF8String::maxISOChar			= 255;
Charset				UTF8String::defaultCharset ("àéèùô");


static const int	defaultDecimalNumber	= ostringstream ( ).precision ( );


// ============================================================================
//                              LA CLASSE Charset
// ============================================================================

Charset::Charset (const char* refString)
	: _charset (Charset::UNKNOWN)
{
	const size_t	number	= strlen (refString);
	size_t			value	= 0;
	for (size_t i = 0; i < number; i++)
		value	+= (unsigned char)refString [i];

	switch (value)
	{
		case 1182	: /* 0x000000000000049e */
			_charset	= ISO_8859;	break;
		case 1837	: /* 0x000000000000072d */
			_charset	= UTF_8;	break;
	}	// switch (value)
}	// Charset::Charset


string Charset::name ( ) const
{
	switch (_charset)
	{
		case ASCII		: return "ASCII";
		case ISO_8859	: return "ISO-8859";
		case UTF_8		: return "UTF-8";
		case UTF_16		: return "UTF-16";
	}	// switch (_charset)

	return "INCONNU";
}	// Charset::name


Charset::CHARSET Charset::str2charset (const char* str)	// v 4.2.0
{
	Charset::CHARSET	charset	= Charset::UNKNOWN;
	if (0 != str)
	{
		if ((0 == strcasecmp (str, "utf8")) || (0 == strcasecmp (str, "utf-8")))
			charset	= Charset::UTF_8;
		else if ((0 == strncasecmp (str, "iso8859", 7)) ||
		         (0 == strncasecmp (str, "iso-8859", 8)))
			charset	= Charset::ISO_8859;
		else if (0 == strcasecmp (str, "ascii"))
			charset	= Charset::ASCII;
		else if ((0 == strcasecmp (str, "utf16")) ||
		         (0 == strcasecmp (str, "utf-16")))
			charset	= Charset::UTF_16;
	}	// if (0 != str)

	return charset;
}	// Charset::str2charset


const char* Charset::charset2str (Charset::CHARSET encoding)  // v 4.2.0
{
	switch (encoding)
	{
		case Charset::ISO_8859	: return "ISO8859-1";
		case Charset::UTF_8		: return "UTF-8";
		case Charset::UTF_16	: return "UTF-16";
	}   // switch (encoding)

	return "ASCII";
}   // Charset::charset2str


// ============================================================================
//                              LA CLASSE UTF8String
// ============================================================================


#ifndef WITHOUT_THREAD_LOCALE
thread_local map< pair<Charset::CHARSET, Charset::CHARSET>, iconv_t >*
	iconvDescriptors =
			new map< pair<Charset::CHARSET, Charset::CHARSET>, iconv_t > ( );
#else	// Anciens systèmes : on ne peut avoir une map par thread
std::mutex	iconvDescriptorsMutex;
map< pair<Charset::CHARSET, Charset::CHARSET>, iconv_t >* iconvDescriptors =
			new map< pair<Charset::CHARSET, Charset::CHARSET>, iconv_t > ( );
#endif	// WITHOUT_THREAD_LOCALE

/*
 * La largeur des champs numériques par défaut.
 */
#define DEF_NFW (0)

UTF8String::UTF8String ( )
	: _utf8String ( ), _localCharset (defaultCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
}	// UTF8String::UTF8String


UTF8String::UTF8String (const UTF8String& str)
	: _utf8String ( ), _localCharset (str._localCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	copy (str);
}	// UTF8String::UTF8String


UTF8String::UTF8String (const Charset& cs)
	: _utf8String ( ), _localCharset (cs),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
}	// UTF8String::UTF8String


UTF8String::UTF8String (Charset::CHARSET cs)
	: _utf8String ( ), _localCharset (cs),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
}	// UTF8String::UTF8String


UTF8String::UTF8String (const UnicodeString& us)
	: _utf8String ( ), _localCharset (defaultCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	copy (us.unicode ( ));
}	// UTF8String::UTF8String


UTF8String::UTF8String (const string& str)
	: _utf8String ( ), _localCharset (Charset::ASCII),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	asciiCopy (str.c_str ( ));
}	// UTF8String::UTF8String


UTF8String::UTF8String (const string& str, const Charset& cs)
	: _utf8String ( ), _localCharset (cs),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	copy (str.c_str ( ), cs);
}	// UTF8String::UTF8String


UTF8String::UTF8String (const string& str, Charset::CHARSET cs)
	: _utf8String ( ), _localCharset (cs),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	copy (str.c_str ( ), cs);
}	// UTF8String::UTF8String


UTF8String::UTF8String (const char* str, const Charset& cs)
	: _utf8String ( ), _localCharset (cs),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
if (cs.charset ( ) != Charset::UTF_16)
	copy (str, cs);
else
	copy ((const WChar_t*)str);
}	// UTF8String::UTF8String


UTF8String::UTF8String (const char* str)
	: _utf8String ( ), _localCharset (Charset::ASCII),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	asciiCopy (str);
}	// UTF8String::UTF8String


UTF8String::UTF8String (const char* str, Charset::CHARSET cs)
	: _utf8String ( ), _localCharset (cs),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
if (cs != Charset::UTF_16)
	copy (str, cs);
else
	copy ((const WChar_t*)str);
}	// UTF8String::UTF8String


UTF8String::UTF8String (char c)
	: _utf8String ( ), _localCharset (defaultCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{	char	buffer [2]	= { c, '\0' };
	copy (buffer, defaultCharset);
}	// UTF8String::UTF8String (char)


UTF8String::UTF8String (short s)
	: _utf8String ( ), _localCharset (defaultCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	_utf8String	= NumericConversions::toStr (s);
}	// UTF8String::UTF8String (short)


UTF8String::UTF8String (long l)
	: _utf8String ( ), _localCharset (defaultCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	_utf8String	= NumericConversions::toStr (l);
}	// UTF8String::UTF8String (long)


UTF8String::UTF8String (unsigned long ul)
	: _utf8String ( ), _localCharset (defaultCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	_utf8String	= NumericConversions::toStr (ul);
}	// UTF8String::UTF8String (unsigned long)


UTF8String::UTF8String (long long l)
	: _utf8String ( ), _localCharset (defaultCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	_utf8String	= NumericConversions::toStr (l);
}	// UTF8String::UTF8String (long long)


UTF8String::UTF8String (unsigned long long ul)
	: _utf8String ( ), _localCharset (defaultCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	_utf8String	= NumericConversions::toStr (ul);
}	// UTF8String::UTF8String (unsigned long long)


UTF8String::UTF8String (float f)
	: _utf8String ( ), _localCharset (defaultCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	_utf8String	= NumericConversions::toStr (f);
}	// UTF8String::UTF8String (float)


UTF8String::UTF8String (double d)
	: _utf8String ( ), _localCharset (defaultCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	_utf8String	= NumericConversions::toStr (d);
}	// UTF8String::UTF8String (double)


UTF8String::UTF8String (long double ld)
	: _utf8String ( ), _localCharset (defaultCharset),
	  _numericFieldWidth (DEF_NFW), _decimalNumber (defaultDecimalNumber), 
	  _scientific (true)
{
	_utf8String	= NumericConversions::toStr (ld);
}	// UTF8String::UTF8String (long double)


UTF8String& UTF8String::operator = (const UTF8String& str)
{
	if (&str != this)
		copy (str);

	return *this;
}	// UTF8String::operator =


UTF8String& UTF8String::operator = (const string& str)
{
	_utf8String	= str;

	return *this;
}	// UTF8String::operator =


UTF8String& UTF8String::operator = (const char* str)
{
	_utf8String	= str;

	return *this;
}	// UTF8String::operator =


UTF8String& UTF8String::operator = (char c)
{
	char	buffer [2]	= { c, '\0' };
	copy (buffer, defaultCharset);

	return *this;
}	// UTF8String::operator =


UTF8String& UTF8String::operator = (short s)
{
	_utf8String	= NumericConversions::toStr (s);

	return *this;
}	// UTF8String::operator =


UTF8String& UTF8String::operator = (long l)
{
	_utf8String	= NumericConversions::toStr (l);

	return *this;
}	// UTF8String::operator =


UTF8String& UTF8String::operator = (unsigned long ul)
{
	_utf8String	= NumericConversions::toStr (ul);

	return *this;
}	// UTF8String::operator =


UTF8String& UTF8String::operator = (long long ll)
{
	_utf8String	= NumericConversions::toStr (ll);

	return *this;
}	// UTF8String::operator =


UTF8String& UTF8String::operator = (unsigned long long ull)
{
	_utf8String	= NumericConversions::toStr (ull);

	return *this;
}	// UTF8String::operator =


UTF8String& UTF8String::operator = (float f)
{
	_utf8String	= NumericConversions::toStr (f);

	return *this;
}	// UTF8String::operator =


UTF8String& UTF8String::operator = (double d)
{
	_utf8String	= NumericConversions::toStr (d);

	return *this;
}	// UTF8String::operator =


UTF8String& UTF8String::operator = (long double d)
{
	_utf8String	= NumericConversions::toStr (d);

	return *this;
}	// UTF8String::operator =


UTF8String::~UTF8String ( )
{
	clear ( );
}	// UTF8String::~UTF8String


void UTF8String::clear ( )
{
	_utf8String.clear ( );
}	// UTF8String::clear


size_t UTF8String::length ( ) const
{
	return _utf8String.length ( );
}	// UTF8String::length


size_t UTF8String::charNum ( ) const
{
	size_t	num	= 0;

	const unsigned char*	ptr		= (const unsigned char*)_utf8String.c_str ( );
	const unsigned char*	last	= ptr + _utf8String.length ( );
	while (ptr < last)
	{
		unsigned char	sz	= utf8CharSize (ptr);
		num++;
		ptr	+= sz;
	}	// while (ptr < last)

	return num;
}	// UTF8String::charNum


const unsigned char* UTF8String::charAt (size_t i) const
{
	size_t	num	= 0;

	const unsigned char*	ptr		= (const unsigned char*)_utf8String.c_str ( );
	const unsigned char*	last	= ptr + _utf8String.length ( );
	while (ptr < last)
	{
		if (i == num)
			return (const unsigned char*)ptr;

		unsigned char	sz	= utf8CharSize (ptr);
		num++;
		ptr	+= sz;
	}	// while (ptr < last)

	return 0;
}	// UTF8String::charAt


bool UTF8String::empty ( ) const
{
	return 0 == length ( ) ? true : false;
}	// UTF8String::empty


bool UTF8String::isAscii ( ) const
{
	try
	{
		translate (Charset::ASCII, utf8 ( ).c_str ( ), Charset::UTF_8);
	}
	catch (...)
	{
		return false;
	}

	return true;
}	// UTF8String::isAscii


bool UTF8String::isIso ( ) const
{
	try
	{
		translate (Charset::ISO_8859, utf8 ( ).c_str ( ), Charset::UTF_8);
	}
	catch (...)
	{
		return false;
	}

	return true;
}	// UTF8String::isIso


UTF16String UTF8String::utf16 ( ) const
{
	const string	str				=
			translate (Charset::UTF_16, utf8 ( ).c_str ( ), Charset::UTF_8);
	const size_t	len			= str.length ( );
	const size_t	bytesNum	= len / sizeof (WChar_t);
	WChar_t*		bytes	= new WChar_t [bytesNum + 1];
	memcpy (bytes, str.c_str ( ), len);
	bytes [bytesNum]	= 0;

	return UTF16String (bytesNum, bytes);	// bytes adopted
}	// UTF8String::utf16


const string& UTF8String::utf8 ( ) const
{
	return _utf8String;
}	// UTF8String::utf8


string UTF8String::ascii ( ) const
{
	const string	iso				= translate (Charset::ISO_8859, _utf8String.c_str ( ), Charset::UTF_8);
	string			str;
	const string::size_type	len		= iso.length ( );
	str.reserve (len + 1);

	for (size_t i = 0; i < len; i++)
	{
		if ((unsigned char)iso [i] <= (unsigned char)maxASCIIChar)
			str	+= iso [i];
	}	// for (string::size_type i = 0; i < len; i++)

	return str;
}	// UTF8String::ascii


string UTF8String::convertedToAscii ( ) const
{
	return UTF8String::replaceSpecialCharacters (utf8 ( ));
}	// UTF8String::convertedToAscii


string UTF8String::iso ( ) const
{
	const string	str				= translate (Charset::ISO_8859, _utf8String.c_str ( ), Charset::UTF_8);

	return str;
}	// UTF8String::iso


string UTF8String::asCharset ( ) const
{
	switch (_localCharset.charset ( ))
	{
		case Charset::ASCII			: return ascii ( );
		case Charset::UTF_8			: return utf8 ( );
		case Charset::ISO_8859		: return iso ( );
	}	// switch (_localCharset.charset ( ))

	return ascii ( );
}	// UTF8String::asCharset


bool UTF8String::hasWhiteSpace ( ) const
{
	const string::size_type	len		= _utf8String.length ( );
	
	for (string::size_type i = 0; i < len; i++)
		if (' ' == _utf8String [i])
			return true;

	return false;
}	// UTF8String::hasWhiteSpace


bool UTF8String::operator == (const UTF8String& toCompare) const
{
	return toCompare._utf8String == _utf8String ? true : false;
}	// UTF8String::operator ==


bool UTF8String::operator != (const UTF8String& toCompare) const
{
	return !(*this == toCompare);
}	// UTF8String::operator ==


UTF8String::operator string ( ) const
{
//	return iso ( );
	return _utf8String;
}	// UTF8String::operator


const char* UTF8String::operator [] (size_t pos) const
{
	if (pos >= length ( ))
		throw Exception ("UTF8String::operator [] : position invalide.");

	return (const char*)_utf8String.c_str ( ) + pos;
}	// UTF8String::operator []


size_t UTF8String::find (const UTF8String& str, size_t pos) const
{	// v 5.11.4 :
	const size_t	len	= length ( );
	if (0 == len)
		return (size_t)-1;

	if (pos >= len)
		throw Exception ("UTF8String::find : position initiale invalide.");
	if ((true == str.empty ( )) || (len < str.length ( )))
		return (size_t)-1;

	const char*		toFind	= str.utf8 ( ).c_str ( );
	const size_t	size	= str.length ( );
	const size_t	stop	= len - str.length ( );
	for ( ; pos <= stop; pos++)
		if (0 == memcmp (utf8 ( ).c_str ( ) + pos, toFind, size))
			return pos;
/*
	if (pos >= length ( ))
		throw Exception ("UTF8String::find : position initiale invalide.");
	if ((true == str.empty ( )) || (length ( ) < str.length ( )))
		return (size_t)-1;

	const char*		toFind	= str.utf8 ( ).c_str ( );
	const size_t	size	= str.length ( );
	const size_t	stop	= length ( ) - str.length ( );
	for ( ; pos <= stop; pos++)
		if (0 == memcmp (utf8 ( ).c_str ( ) + pos, toFind, size))
			return pos;
*/

	return (size_t)-1;
}	// UTF8String::find (const UTF8String& str, size_t pos)


size_t UTF8String::find (const string& str, size_t pos) const
{
	return find (UTF8String (str, UTF8String::defaultCharset), pos);
}	// UTF8String::find (const string& str, size_t pos)


size_t UTF8String::find (const char* str, size_t pos) const
{
	return find (UTF8String (str, UTF8String::defaultCharset), pos);
}	// UTF8String::find (const char* str, size_t pos)


size_t UTF8String::find (char c, size_t pos) const
{
	return find (UTF8String (c), pos);
}	// UTF8String::find (char c, size_t pos)


size_t UTF8String::rfind (const UTF8String& str, size_t pos) const
{
	const size_t	strLength	= str.length ( );
	const size_t	len			= length ( );
	if ((size_t)-1 == pos)
		pos	= len - 1;

	if (pos >= len)
		throw Exception ("UTF8String::rfind : position initiale invalide.");
	if ((0 == strLength) || (len < strLength))
		return (size_t)-1;

	// Eviter les comparaisons au delà de la chaîne :
	if (pos + strLength >= len)
		pos	= len - 1 - strLength;
	const char*		toFind	= str.utf8 ( ).c_str ( );
	const size_t	size	= strLength;
	// coverity : pos >= 0 toujours vrai
	for ( ; (size_t)-1 != pos; pos--)
		if (0 == memcmp (utf8 ( ).c_str ( ) + pos, toFind, size))
			return pos;

	return (size_t)-1;
}	// rfind (const UTF8String&, size_t)


size_t UTF8String::rfind (const string& str, size_t pos) const
{
	return rfind (UTF8String (str, UTF8String::defaultCharset), pos);
}	// rfind (const string&, size_t)


size_t UTF8String::rfind (const char* str, size_t pos) const
{
	return rfind (UTF8String (str, UTF8String::defaultCharset), pos);
}	// rfind (const char*, size_t)


size_t UTF8String::rfind (char c, size_t pos) const
{
	return rfind (UTF8String (c), pos);
}	// UTF8String::rfind (char, size_t)


size_t UTF8String::replace (
					const string& pattern1, const string& pattern2, bool all)
{
	if (0 == pattern1.length ( ))
		throw Exception (UTF8String ("UTF8String::replace : absence de motif à remplacer.", charset));

	size_t			count	= 0;
	const char*		ptr		= _utf8String.c_str ( );
	const char*		pat1	= pattern1.c_str ( );
	const size_t	pat1len	= strlen (pat1);
	const char*		pos		= strstr (ptr, pat1);
	ostringstream	stream;

	while (NULL != pos)
	{
		while (ptr != pos)
			stream << *ptr++;
		ptr += pat1len;
		stream << pattern2;
		
		pos	= strstr (ptr, pat1);
		if (false == all)
			break;
	}	// while (NULL != pos)

	while (*ptr != '\0')
		stream << *ptr++;
	_utf8String	= stream.str ( );

	return count;
}	// UTF8String::replace


size_t UTF8String::replace (
			const UTF8String& pattern1, const UTF8String& pattern2, bool all)
{
	return replace (pattern1.utf8 ( ), pattern2.utf8 ( ), all);
}	// UTF8String::replace


UTF8String UTF8String::substring (size_t first, size_t last) const
{
	const string::size_type	len		= _utf8String.length ( );
	if (len < first)
		throw Exception ("UTF8String::substring. first hors domaine.");
	if ((size_t)-1 == last)
		last	= len;
	if (len < last)
		throw Exception ("UTF8String::substring. last hors domaine.");
	if (last < first)
		throw Exception ("UTF8String::substring. Domaine invalide.");
	const size_t	length	= last - first + 1;

	UTF8String	substr (_localCharset);
	substr._utf8String	= _utf8String.substr (first, length);

	return substr;
}	// UTF8String::substring


UTF8String& UTF8String::operator += (const UTF8String& str)
{
	const string	dest	= _utf8String + str._utf8String;
	_utf8String	= dest;

	return *this;
}	// UTF8String::operator += (const UTF8String&)


UTF8String& UTF8String::operator += (const string& str)
{
	return operator += (UTF8String (str, _localCharset));
}	// UTF8String::operator += (const string&)


UTF8String& UTF8String::operator += (const char* str)
{
	return operator += (UTF8String (str, _localCharset));
}	// UTF8String::operator += (const char*)


UTF8String& UTF8String::operator += (char c)
{
	return operator += (UTF8String (c));
}	// UTF8String::operator += (char)


UTF8String& UTF8String::operator += (short s)
{
	return operator += (UTF8String (s));
}	// UTF8String::operator += (short)


UTF8String& UTF8String::operator += (long l)
{
	return operator += (UTF8String (l));
}	// UTF8String::operator += (long)


UTF8String& UTF8String::operator += (unsigned long ul)
{
	return operator += (UTF8String (ul));
}	// UTF8String::operator += (unsigned long)


UTF8String& UTF8String::operator += (long long l)
{
	return operator += (UTF8String (l));
}	// UTF8String::operator += (long long)


UTF8String& UTF8String::operator += (unsigned long long ul)
{
	return operator += (UTF8String (ul));
}	// UTF8String::operator += (unsigned long long)


UTF8String& UTF8String::operator += (float f)
{
	return operator += (UTF8String (f));
}	// UTF8String::operator += (float f)


UTF8String& UTF8String::operator += (double d)
{
	return operator += (UTF8String (d));
}	// UTF8String::operator += (double d)


UTF8String& UTF8String::operator += (long double d)
{
	return operator += (UTF8String (d));
}	// UTF8String::operator += (long double d)


UTF8String& UTF8String::operator << (const UTF8String& str)
{
	return *this += str;
}	// UTF8String::operator << (const UTF8String& str)


UTF8String& UTF8String::operator << (const string& str)
{
	return *this += str;
}	// UTF8String::operator << (const string& str)


UTF8String& UTF8String::operator << (const char* str)
{
	return *this += str;
}	// UTF8String::operator << (const char*)


UTF8String& UTF8String::operator << (char c)
{
	return *this += c;
}	// UTF8String::operator << (char)


UTF8String& UTF8String::operator << (short s)
{
	return *this += NumericConversions::toStr (s, _numericFieldWidth);
}	// UTF8String::operator << (short)


UTF8String& UTF8String::operator << (long l)
{
	return *this += NumericConversions::toStr (l, _numericFieldWidth);
}	// UTF8String::operator << (long)


UTF8String& UTF8String::operator << (unsigned long ul)
{
	return *this += NumericConversions::toStr (ul, _numericFieldWidth);
}	// UTF8String::operator << (unsigned long)


UTF8String& UTF8String::operator << (long long l)
{
	return *this += NumericConversions::toStr (l, _numericFieldWidth);
}	// UTF8String::operator << (long)


UTF8String& UTF8String::operator << (unsigned long long ul)
{
	return *this += NumericConversions::toStr (ul, _numericFieldWidth);
}	// UTF8String::operator << (unsigned long)


UTF8String& UTF8String::operator << (float f)
{
	return *this += NumericConversions::toStr (
						f, _decimalNumber, _numericFieldWidth, _scientific);
}	// UTF8String::operator << (float)


UTF8String& UTF8String::operator << (double d)
{
	return *this += NumericConversions::toStr (
							d, _decimalNumber, _numericFieldWidth, _scientific);
}	// UTF8String::operator << (double)


UTF8String& UTF8String::operator << (long double d)
{
	return *this += NumericConversions::toStr (
							d, _decimalNumber, _numericFieldWidth, _scientific);
}	// UTF8String::operator << (long double)


UTF8String UTF8String::format (size_t columnNum) const
{
	if (0 == columnNum)
		throw Exception ("UTF8String::format : nombre de colonnes nul.");
	if (columnNum >= length ( ))
		return UTF8String (*this);

	UTF8String				formated;
	const char*				str		= utf8 ( ).c_str ( );
	const char*				ptr		= str;
	size_t					lastWs	= 0, pos = 0, lastPos = 0, size = 0;
	bool					hasWs	= false;
	const string::size_type	l		= _utf8String.length ( );

	while (pos < l)
	{
		if (' ' == *ptr)
		{
			lastWs	= pos;
			hasWs	= true;
		}	// if (' ' == *ptr)
		if ('\n' == *ptr)
		{
			formated	+= substring (lastPos, pos);
			lastPos		= pos + 1;
			size		= 1;
			hasWs		= false;
			pos++;	ptr++;
			continue;
		}	// if ('\n' == *ptr)

		if (size + 1 >= columnNum)	// => necessité de couper
		{
			if (false == hasWs)
				lastWs	= lastPos + columnNum - 1;
			formated	+= substring (lastPos, lastWs);
			formated	+= '\n';
			lastPos		= lastWs + 1;
			size		= pos - lastWs;
			hasWs		= false;
		}	// if (size + 1 == columnNum)

		if (lastPos + columnNum >= l)
		{
			formated	+= substring (lastPos);
			break;
		}	// if (lastPos + columnNum == l)

		pos++;	ptr++; size++;
	}	// while (lastWs < l)

	return formated;
}	// UTF8String::format


UTF8String UTF8String::trim (bool removeTabs) const
{
	const char*				str		= utf8 ( ).c_str ( );
	const char*				ptr		= str;
	bool					hasWs	= false;
	const string::size_type	l		= _utf8String.length ( );
	size_t					pos = 0, first = 0, last = l - 1;

	for (pos = 0, ptr = str; pos < l; pos++, ptr++)
	{
		if (true == removeTabs)
		{
			if ((' ' == *ptr) ||
			    ('\n'   == *ptr) ||
			    ('\t' == *ptr))
			{
				hasWs	= true;
				continue;
			}
		}
		else if ((' ' == *ptr) || ('\n'   == *ptr))
		{
			hasWs	= true;
			continue;
		}

		first	= pos;
		break;
	}	// for (pos = 0, ptr = str; pos < l; pos++, ptr++)
	if (l == pos)		// Que des séparateurs
		return UTF8String (string (""), _localCharset);

	for (pos = l - 1, ptr = str + l - 1; pos != (size_t)-1; pos--, ptr--)
	{
		if (true == removeTabs)
		{
			if ((' ' == *ptr) ||
			    ('\n'   == *ptr) ||
			    ('\t' == *ptr))
			{
				hasWs	= true;
				continue;
			}
		}
		else if ((' ' == *ptr) || ('\n'   == *ptr))
		{
			hasWs	= true;
			continue;
		}

		last	= pos;
		break;
	}	// for (pos = l - 1, ptr = str + l - 1; pos != (size_t)-1; pos--, ptr--)

	if (false == hasWs)
		return *this;

	if (first <= last)
		return substring (first, last);

	return UTF8String (string (""), _localCharset);
}	// UTF8String::trim


unsigned char UTF8String::utf8CharSize (const unsigned char* c)
{	// Les bits du 1er octet donnent le nombre d'octets encodant le caractère :
	// 0xxxxxxx : 1 octet codant de 1 à 7 bits
	// 110xxxxx : 2 octets codants de 8 à 11 bits
	// 1110xxxx : 3 octets codants de 12 à 16 bits
	// 11110xxx : 4 octets codants de 17 à 21 bits
	if (0 == c)
		return 0;

	const unsigned char	two		= 192;	// 2^7 + 2^6
	const unsigned char	three	= 224;	// 2^7 + 2^6 + 2^5
	const unsigned char	four	= 240;	// 2^7 + 2^6 + 2^5 + 2^4
	const unsigned char	uc		= *c;
	unsigned char	test	= uc & four;
	if (test == four)
		return 4;
	test	= uc & three;
	if (test == three)
		return 3;
	test	= uc & two;
	if (test == two)
		return 2;

	return 1;
}	// UTF8String::utf8CharSize


string UTF8String::replaceUtf8Char (
					const string& utf8str, const unsigned char* r, char c)
{
	if (0 == r)
		return utf8str;

	const unsigned char		sz	= utf8CharSize (r);
	const unsigned char*	ptr	= (const unsigned char*)utf8str.c_str ( );
	const unsigned char*	end	= ptr + utf8str.length ( );
	string	newstr;

	while (ptr < end)
	{
		const unsigned char	step	= utf8CharSize (ptr);
		if (0 != step)
		{
			if (0 == memcmp (ptr, r, sz * sizeof (char)))
				newstr	+= c;
			else
				for (size_t j = 0; j < step; j++)
					newstr	+= (char)ptr [j];

			ptr	+= step;
		}	// if (0 != step)
		else
			ptr++;		// Oups !!!
	}	//  while (ptr < end)

	return newstr;
}	// replaceUtf8Char


string UTF8String::replaceSpecialCharacters (const string& utf8str)
{
	static const UTF8String	a ("àáâä", charset);
	static const UTF8String	e ("éèêë", charset);
	static const UTF8String	o ("óòôö", charset);
	static const UTF8String	i ("ìíîï", charset);
	static const UTF8String	u ("ùúûü", charset);
	static const UTF8String	c ("ç", charset);
	static const UTF8String	A ("ÀÁÂÄ", charset);
	static const UTF8String	E ("ÈÉÊË", charset);
	static const UTF8String	I ("ÌÍÎÏ", charset);
	static const UTF8String	O ("ÒÓÔÖ", charset);
	static const UTF8String	U ("ÙÚÛÜ", charset);
	string		converted	= utf8str;

	size_t	num	= a.charNum ( );
	for (size_t cpt = 0; cpt < num; cpt++)
		converted	= replaceUtf8Char (converted, a.charAt (cpt), 'a');
	num	= e.charNum ( );
	for (size_t cpt = 0; cpt < num; cpt++)
		converted	= replaceUtf8Char (converted, e.charAt (cpt), 'e');
	num	= i.charNum ( );
	for (size_t cpt = 0; cpt < num; cpt++)
		converted	= replaceUtf8Char (converted, i.charAt (cpt), 'i');
	num	= o.charNum ( );
	for (size_t cpt = 0; cpt < num; cpt++)
		converted	= replaceUtf8Char (converted, o.charAt (cpt), 'o');
	num	= u.charNum ( );
	for (size_t cpt = 0; cpt < num; cpt++)
		converted	= replaceUtf8Char (converted, u.charAt (cpt), 'u');
	num	= c.charNum ( );
	for (size_t cpt = 0; cpt < num; cpt++)
		converted	= replaceUtf8Char (converted, c.charAt (cpt), 'c');
	num	= A.charNum ( );
	for (size_t cpt = 0; cpt < num; cpt++)
		converted	= replaceUtf8Char (converted, A.charAt (cpt), 'A');
	num	= E.charNum ( );
	for (size_t cpt = 0; cpt < num; cpt++)
		converted	= replaceUtf8Char (converted, E.charAt (cpt), 'E');
	num	= I.charNum ( );
	for (size_t cpt = 0; cpt < num; cpt++)
		converted	= replaceUtf8Char (converted, I.charAt (cpt), 'I');
	num	= O.charNum ( );
	for (size_t cpt = 0; cpt < num; cpt++)
		converted	= replaceUtf8Char (converted, O.charAt (cpt), 'O');
	num	= U.charNum ( );
	for (size_t cpt = 0; cpt < num; cpt++)
		converted	= replaceUtf8Char (converted, U.charAt (cpt), 'U');

	return converted;
}	// UTF8String::replaceSpecialCharacters


void UTF8String::copy (const UTF8String& str)
{
	if (&str == this)
		return;
	clear ( );
	_utf8String	= str._utf8String;
}	// UTF8String::copy


void UTF8String::copy (const char* str, const Charset& charset)
{
	clear ( );
	_utf8String	= translate (Charset::UTF_8, str, charset.charset ( ));
}	// UTF8String::copy


void UTF8String::asciiCopy (const char* str)
{
	clear ( );
	const size_t	length	= strlen (str);
	_utf8String.reserve (length + 1);
	const char*	src	= str;
	for (size_t i = 0; i < length; i++, src++)
		if (0 != isascii (*src))
			_utf8String.push_back (*src);
}	// UTF8String::asciiCopy


void UTF8String::copy (const WChar_t* str)
{
	clear ( );

	const size_t	length	= utf16StringLength (str);
	UTF16String		utf16 (str, length);

	// ATTENTION : pas de test big/little endian
	_utf8String	= translate (
			Charset::UTF_8, (char*)utf16.utf16 ( ), Charset::UTF_16);
}	// UTF8String::copy


static const char* charsetToName (Charset::CHARSET cs)
{
	switch (cs)
	{
		case Charset::ASCII		: return "ASCII";
		case Charset::ISO_8859	: return "ISO-8859-1";
		case Charset::UTF_8		: return "UTF-8";
		case Charset::UTF_16	: return "UTF-16";
	}	// switch (cs)

	ostringstream	error;
	error << "Jeu de caracteres inconnu (" << (unsigned long)cs
	      << ") dans charsetToName (Charset::CHARSET).";
	throw Exception (error.str ( ));
}	// charsetToName


string UTF8String::translate (Charset::CHARSET to, const char* str, Charset::CHARSET from)
{
#ifdef WITHOUT_THREAD_LOCALE
// Anciens systèmes : iconv modifie le descripteur => pas d'accès concurrents
// possibles.
	unique_lock<mutex>	iconvDescriptorsLock (iconvDescriptorsMutex);
#endif	// WITHOUT_THREAD_LOCALE
	if (to == from)
		return string (str);

	// Tentative avec des tableaux de taille raisonnable alloués dans le tas.
	// En cas d'échec on passera à des tableaux alloués dynamiquement.
	size_t			bufferSize	= 10000;
	size_t			length		= strlen (str);
	const	size_t	bufSize		= 10000;
	switch (from)
	{
		case Charset::UTF_16	:
			// REM : on pourrait s'assurer ici que les 2 premiers octets de str sont (char)0xFF (char)0xFE (little endian)
			// ou (char)0xFE (char)0xFF (big endian). Ces 2 premiers octets (BOM) ne sont pas utiles si le codec le renseigne
			// (UTF-16LE/UTF-16BE).
			length	= utf16StringLength ((const WChar_t*)str) * sizeof(WChar_t);
			break;
	}	// switch (from)

	if (length > bufferSize)
		return dynTranslate (to, str, from);

	char		input [10001];
	char		output [10001];
	memcpy (input, str, length + 1);
	memset (output, '\0', bufferSize + 1);
	iconv_t	descriptor	= getIconvDescriptor (from, to);

	errno	= 0;
	char*	inptr	= &input [0];
	char*	outptr	= &output [0];

	const size_t res	= iconv (
			descriptor, &inptr, (size_t*)&length, &outptr, &bufferSize);
	if ((size_t)-1 == res)
	{
		if (E2BIG == errno)	// bufferSize too small
			return dynTranslate (to, str, from);
			
		ostringstream	error;
cout << __FILE__ << ' ' << __LINE__ << " ERREUR CONVERSION ICI : " << charsetToName (from) << ") vers le jeu de caracteres ("
		      << charsetToName (to) << "). CHAINE=" << str << endl;
		error << "Conversion impossible du jeu de caracteres ("
		      << charsetToName (from) << ") vers le jeu de caracteres ("
		      << charsetToName (to) << ") : " << strerror (errno) << ".";
		throw Exception (error.str ( ));
	}	// if ((size_t)-1 == res)

	return string (output, bufSize - bufferSize);
}	// UTF8String::translate


string UTF8String::dynTranslate (
                Charset::CHARSET to, const char* str, Charset::CHARSET from)
{	// Appelé par UTF8String::translate => pas plus de mutex
	size_t				length		= strlen (str);
	// 4 * length + 4 devrait répondre aux pires cas (UTF-16, ~UTF-8)
	size_t				bufferSize	= 4 * length + 4;
	const size_t		allocated	= bufferSize;
	char*				input		= new char [length + 1];
	char*				output		= new char [bufferSize + 1];
	AutoArrayPtr<char>	in (input);
	AutoArrayPtr<char>	out (output);
	memcpy (input, str, length+1);
	memset (output, '\0', bufferSize + 1);
	iconv_t	descriptor	= getIconvDescriptor (from, to);

	errno	= 0;
	char*	inptr	= &input [0];
	char*	outptr	= &output [0];

	const size_t res	= iconv (
			descriptor, &inptr, (size_t*)&length, &outptr, &bufferSize);
	if ((size_t)-1 == res)
	{
		ostringstream	error;
cout << __FILE__ << ' ' << __LINE__ << " ERREUR CONVERSION ICI : " << charsetToName (from) << ") vers le jeu de caracteres ("
		      << charsetToName (to) << "). CHAINE=" << str << endl;
		error << "Conversion impossible du jeu de caracteres ("
		      << charsetToName (from) << ") vers le jeu de caracteres ("
		      << charsetToName (to) << ") : " << strerror (errno) << ".";
		throw Exception (error.str ( ));
	}	// if ((size_t)-1 == res)

	return string (output, allocated - bufferSize);
}	// UTF8String::dynTranslate


size_t UTF8String::utf16StringLength (const WChar_t* str)
{
	size_t			length	= 0;
	const WChar_t*	ptr		= str;

	while (0 != *ptr)
	{
		ptr++;
		length++;
	}

	return length;
}	// UTF8String::utf16StringLength


iconv_t UTF8String::getIconvDescriptor (
									Charset::CHARSET from, Charset::CHARSET to)
{
	assert (0 != iconvDescriptors);
//	unique_lock<mutex>	iconvLock (iconvMapMutex);
	pair<Charset::CHARSET, Charset::CHARSET>	required (from, to);

	map< pair<Charset::CHARSET, Charset::CHARSET>, iconv_t >::iterator	it	=
											iconvDescriptors->find (required);
	if (iconvDescriptors->end ( ) == it)
	{
		errno			= 0;
		const char*		f	= charsetToName (from);
		const char*		t	= charsetToName (to);
		iconv_t	desc	= iconv_open (t, f);
		if ((iconv_t)-1 == desc)
		{
			ostringstream	error;
cout << __FILE__ << ' ' << __LINE__ << " ERREUR CONVERSION ICI : " << charsetToName (from) << ") vers le jeu de caracteres ("
		      << charsetToName (to) << ")." << endl;
			error << "Conversion impossible du jeu de caracteres ("
			      << (0 == f ? "NULL" : f) << " - "
			      << charsetToName (from) << ") vers le jeu de caracteres ("
			      << (0 == t ? "NULL" : t) << " - " << charsetToName (to)
			      << ") : " << strerror (errno) << ".";
			throw Exception (error.str ( ));
		}	// if ((iconv_t)-1 == desc)

		(*iconvDescriptors) [required]	= desc;
		return desc;
	}	// if (iconvDescriptors->end ( ) == it)

	return (*it).second;
}	// UTF8String::getIconvDescriptor


UTF8String operator + (const UTF8String& us1, const UTF8String& us2)
{
	const string	str	= us1.utf8 ( ) + us2.utf8 ( );
	UTF8String		us (us1._localCharset);
	us._utf8String	= str;

	return us;
}	// operator + (const UTF8String& us1, const UTF8String& us2)


UTF8String operator + (const UTF8String& us, const string& str)
{
	return us + UTF8String (str, us._localCharset);
}	// operator + (const UTF8String&, const string&)


UTF8String operator + (const UTF8String& us, const char* str)
{
	return us + UTF8String (str, us._localCharset);
}	// operator + (const UTF8String&, const char*)


UTF8String operator + (const UTF8String& us, char c)
{
	return us + UTF8String (c);
}	// operator + (const UTF8String&, char)


UTF8String operator + (const UTF8String& us, short s)
{
	return us + UTF8String (s);
}	// operator + (const UTF8String&, short)


UTF8String operator + (const UTF8String& us, long l)
{
	return us + UTF8String (l);
}	// operator + (const UTF8String&, long)


UTF8String operator + (const UTF8String& us, unsigned long ul)
{
	return us + UTF8String (ul);
}	// operator + (const UTF8String&, unsigned long)


UTF8String operator + (const UTF8String& us, long long l)
{
	return us + UTF8String (l);
}	// operator + (const UTF8String&, long)


UTF8String operator + (const UTF8String& us, unsigned long long ul)
{
	return us + UTF8String (ul);
}	// operator + (const UTF8String&, unsigned long)


UTF8String operator + (const UTF8String& us, float f)
{
	return us + UTF8String (f);
}	// operator + (const UTF8String&, float)


UTF8String operator + (const UTF8String& us, double d)
{
	return us + UTF8String (d);
}	// operator + (const UTF8String&, double)


UTF8String operator + (const UTF8String& us, long double d)
{
	return us + UTF8String (d);
}	// operator + (const UTF8String&, long double)


UTF8String& operator << (UTF8String& u8str, Charset::CHARSET charset)
{
	u8str._localCharset	= charset;
	return u8str;
}	// operator << (UTF8String&, CHARSET)


UTF8String& operator << (UTF8String& u8str, const Charset& charset)
{
	u8str._localCharset	= charset.charset ( );
	return u8str;
}	// operator << (UTF8String&, Charset)


UTF8String& operator << (UTF8String& u8str, setw width)
{
	u8str._numericFieldWidth	= width.width ( );
	return u8str;
}	// operator << (UTF8String&, setw)


UTF8String& operator << (UTF8String& u8str, setprecision prec)
{
	u8str._decimalNumber	= prec.precision ( );
	return u8str;
}	// operator << (UTF8String&, setprecision)


UTF8String& operator << (UTF8String& u8str, ios_base::fmtflags flags)
{
	if (flags == ios_base::scientific)
		u8str._scientific	= true;
	else if (flags == ios_base::fixed)
		u8str._scientific	= false;

	return u8str;
}	// operator << (UTF8String&, ios_base::fmtflags)


ostream& operator << (ostream& os, const UTF8String& str)
{
	if (0 != str.length ( ))
//		os << str.iso ( );	// v 4.1.0
		os << str.utf8 ( );	// v 4.1.0

	return os;
}	// operator << (ostream& os, const UTF8String& str)


// ============================================================================
//                              LA CLASSE UTF16String
// ============================================================================

UTF16String::UTF16String (size_t length, WChar_t* bytes)
	: _length (length), _bytes (bytes)
{
}	// UTF16String::UTF16String


UTF16String::UTF16String (const WChar_t* bytes, size_t length)
	: _length (length), _bytes (0)
{
	_bytes	= new WChar_t [length + 1];
	memcpy (_bytes, bytes, length * sizeof (WChar_t));
	_bytes [length]	= 0;
}	// UTF16String::UTF16String


UTF16String::UTF16String (const UTF16String& u16)
	: _length (u16._length), _bytes (0)
{
	_bytes	= new WChar_t [_length + 1];
	memcpy (_bytes, u16._bytes, _length * sizeof (WChar_t));
	_bytes [_length]	= 0;
}	// UTF16String::UTF16String


UTF16String& UTF16String::operator = (const UTF16String& u16)
{
	if (&u16 != this)
	{
		delete [] _bytes;		_bytes	= 0;
		_length	= u16._length;
		_bytes	= new WChar_t [_length + 1];
		memcpy (_bytes, u16._bytes, _length * sizeof (WChar_t));
		_bytes [_length]	= 0;
	}	// if (&u16 != this)

	return *this;
}	// UTF16String::operator =


UTF16String::~UTF16String ( )
{
	delete [] _bytes;
	_bytes	= 0;
}	// UTF16String::~UTF16String


// =============================================================================
//                            LA CLASSE ConsoleOutput
// =============================================================================

ConsoleOutput*	ConsoleOutput::_cout	= 0;
ConsoleOutput*	ConsoleOutput::_cerr	= 0;

ConsoleOutput& ConsoleOutput::cout ( )
{
	if (0 == _cout)
		_cout	= new ConsoleOutput (std::cout);

	return *_cout;
}	// ConsoleOutput::cout


ConsoleOutput& ConsoleOutput::cerr ( )
{
	if (0 == _cerr)
		_cerr	= new ConsoleOutput (std::cerr);

	return *_cerr;
}	// ConsoleOutput::cerr


ConsoleOutput& ConsoleOutput::operator << (const UTF8String& utf)
{
	try
	{

	string	str;

	switch (getCharset ( ).charset ( ))
	{
		case Charset::ISO_8859	: str	= utf.iso ( );	break;
		case Charset::UTF_8		: str	= utf.utf8 ( );	break;
		default					: str	= utf.convertedToAscii ( );	break;
	}	// switch (getCharset ( ).charset ( ))

	_stream << str;

	}
	catch (...)
	{
		_stream << utf.utf8 ( );
	}

	return *this;
}	// operator << (ConsoleOutput&, const UTF8String&)


ConsoleOutput& ConsoleOutput::operator << (unsigned long ul)
{
	_stream << ul;

	return *this;
}	// ConsoleOutput::operator << (unsigned long)


ConsoleOutput& ConsoleOutput::operator << (double d)
{
	_stream << d;

	return *this;
}	// ConsoleOutput::operator << (double)


ConsoleOutput& ConsoleOutput::operator << (const string& str)
{
	_stream << str;

	return *this;
}	// ConsoleOutput::operator << (const string&)


ConsoleOutput& ConsoleOutput::operator << (const char* str)
{
	_stream << str;

	return *this;
}	// ConsoleOutput::operator << (const char*)


ConsoleOutput& ConsoleOutput::operator << (const char c)
{
	_stream << c;

	return *this;
}	// ConsoleOutput::operator << (char)


ConsoleOutput& ConsoleOutput::operator << (ios_base::fmtflags flags)
{
	_stream << flags;

	return *this;
}	// ConsoleOutput::operator << (ios_base::fmtflags flags)


ConsoleOutput& operator << (ConsoleOutput& co, _Setprecision prec)
{
	co._stream << prec;

	return co;
}	// operator << (_Setprecision flags)


ConsoleOutput& operator << (ConsoleOutput& co, _Setw w)
{
	co._stream << w;

	return co;
}	// operator << (_Setw w)


ConsoleOutput& co_endl (ConsoleOutput& co)
{
	co._stream << std::endl;

	return co;
}	// co_endl (ConsoleOutput&)


END_NAMESPACE_UTIL

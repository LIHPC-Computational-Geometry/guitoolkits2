#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/MemoryError.h>

#include <iostream>
#include <string.h>


USING_UTIL
USING_STD


#define BUFFER_SIZE		2048


QString QtUnicodeHelper::toQString (Charset::CHARSET charset, const char* str)
{
	switch (charset)
	{
		case Charset::ASCII		: return QString::fromLatin1 (str);
		case Charset::ISO_8859	: return QString::fromLatin1 (str);
		case Charset::UTF_8		: return QString::fromUtf8 (str);
	}	// switch (charset)

	UTF8String	mess (charset);
	mess << "La chaine \"" << str
	     << "\" n'a pas pu etre convertie dans le bon codec, codec ("
	     << (unsigned long)charset << " non reconnu.";
	ConsoleOutput::cerr ( ) << mess << co_endl;

	return QString (str);
}	// QtUnicodeHelper::toQString


UnicodeString QtUnicodeHelper::qstringToUnicodeString (const QString& qstr)
{
	const size_t	length	= qstr.length ( );
	static WChar_t	buffer [BUFFER_SIZE];

	if (length < BUFFER_SIZE)
	{
		memcpy (buffer, qstr.unicode ( ), length * sizeof (WChar_t));
		buffer [length]	= 0;

		return UnicodeString (buffer);
	}
	else
	{
		WChar_t*	tmpBuffer	= new WChar_t [length + 1];
		CHECK_NULL_PTR_ERROR (tmpBuffer)
		memcpy (tmpBuffer, qstr.unicode ( ), length * sizeof (WChar_t));
		tmpBuffer [length]	= 0;

		UnicodeString	uStr (tmpBuffer);
		delete [] tmpBuffer;

		return uStr;
	}
}	// QtUnicodeHelper::qstringToUnicodeString


UTF8String QtUnicodeHelper::qstringToUTF8String (const QString& qstr)
{
	UTF8String	str (qstr.toUtf8 ( ), Charset::UTF_8);

	return str;
}	// QtUnicodeHelper::qstringToUTF8String


QString QtUnicodeHelper::unicodeStringToQString (const UnicodeString& ustr)
{
	return QString ((const QChar*)ustr.unicode ( ), ustr.length ( ));
}	// QtUnicodeHelper::unicodeStringToQString




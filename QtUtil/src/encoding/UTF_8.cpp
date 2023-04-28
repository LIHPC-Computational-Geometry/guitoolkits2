#include "UTF_8.h"

#include "QtUtil/QtUnicodeHelper.h"


static const TkUtil::Charset	charset ("àéèùô");
static const char*		text	=
	"Chaîne de caractères issu d'un fichier encodé en UTF-8";

USE_ENCODING_AUTODETECTION
	

std::string utf8Str ( )
{
	return STDSTR (text);
}	// utf8Str


TkUtil::UTF8String utfStr ( )
{
	TkUtil::UTF8String	s (text, charset);

	return s;
}	// utf8Str


QString fromUTF8 ( )
{
	return QSTR (text);
}	// fromUTF8

#include "Iso8859_1.h"

#include "QtUtil/QtUnicodeHelper.h"

static const TkUtil::Charset	charset ("àéèùô");
static const char*				text	=
			"Chaîne de caractères issue d'un fichier encodé en ISO8859-1";

USE_ENCODING_AUTODETECTION


std::string iso8859_1Str ( )
{
	return STDSTR (text);
}	// iso8859_1Str


TkUtil::UTF8String isoStr ( )
{
	TkUtil::UTF8String	s (text, charset);

	return s;
}	// isoStr


QString fromIso8859_1 ( )
{
	return QSTR ("Chaîne de caractères issue d'un fichier encodé en ISO8859-1");
}	// fromIso8859_1

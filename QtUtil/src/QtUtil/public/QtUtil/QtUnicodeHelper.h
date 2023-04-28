#ifndef QT_UNICODE_HELPER_H
#define QT_UNICODE_HELPER_H


#include <TkUtil/UnicodeString.h>
#include <TkUtil/UTF8String.h>

#include <QString>
#include <iostream>


#ifndef USE_ENCODING_AUTODETECTION

/**
 * <P>
 * Macros visant à instancier "convenablement" une QString ou une std::string à
 * partir d'une chaîne de caractères (avec par exemple des caractères accentués)
 * dont on ne sait pas a priori quel est le jeu de caractères (latin/UTF) 
 * En cas de mauvais choix des caractères bizarres peuvent apparaître (ex :
 * ChaÃ®ne à la place de Chaîne).
 * </P>
 * <P>
 * Pour pouvoir utiliser ces macro <B>QSTR</B> et <B>STDSTR</B> il convient, en
 * début de fichier, de mettre les 3 lignes suivantes :
 * </P>
 * <CODE>
 * #include <QtUtil/QtUnicodeHelper.h>
 * static const TkUtil::Charset    charset ("àéèùô");
 * USE_ENCODING_AUTODETECTION
 * </CODE>
 */
#define USE_ENCODING_AUTODETECTION                                          \
static QString QSTR (const char* str)                                       \
{                                                                           \
	return QtUnicodeHelper::toQString (charset.charset ( ), str);           \
}                                                                           \
static QString QSTR (const std::string& str)                                \
{                                                                           \
	return QtUnicodeHelper::toQString (                                     \
	                         charset.charset ( ), str.c_str ( ));           \
}                                                                           \
static std::string STDSTR (const char* str)                                 \
{                                                                           \
	return std::string (QSTR (str).toLatin1 ( ));                           \
}                                                                           \
static TkUtil::UTF8String UTF8STR (const char* str)                         \
{                                                                           \
	return TkUtil::UTF8String (str, charset);                               \
}

#endif	// USE_ENCODING_AUTODETECTION

/*
 * @return	Une <I>QString</I> correspondant à la chaîne <I>UTF8String</I>
 *			transmise en argument.
 */
#define UTF8TOQSTRING(str)                                                  \
	QString::fromUtf8(str.utf8 ( ).c_str ( ))

class QtUnicodeHelper
{
	public :

	static IN_UTIL UnicodeString qstringToUnicodeString (const QString& qstr);
	static IN_UTIL UTF8String qstringToUTF8String (const QString& qstr);
	static QString unicodeStringToQString (const IN_UTIL UnicodeString& ustr);
	static QString toQString (
					TkUtil::Charset::CHARSET charset, const char* str);
};	// class QtUnicodeHelper

#endif	// QT_UNICODE_HELPER_H

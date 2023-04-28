#include "QtUtil/QtStringNumberFormatComboBox.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/Exception.h>
#include <TkUtil/InternalError.h>

#include <assert.h>


USING_UTIL

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


struct StringNumberFormatMap
{
	const char	format;
	const char*	label;
};


#define FORMAT_NUM	(5)

static const	size_t				formatsNum	= FORMAT_NUM;
static const StringNumberFormatMap	formats [FORMAT_NUM]	=
{
	{ 'e', "[-]9.9e[+|-]999"},
	{ 'E', "[-]9.9E[+|-]999"},
	{ 'f', "[-]9.9"},
	{ 'g', "Le plus concis de 'e' et 'f'"},
	{ 'G', "Le plus concis de 'E' et 'f'"}
};	// formats


QtStringNumberFormatComboBox::QtStringNumberFormatComboBox (
												QWidget* parent, char format)
	: QComboBox (parent)
{
	for (size_t i = 0; i < formatsNum; i++)
	{
		UTF8String	label;
		label << formats [i].format << " - " << formats [i].label;
		addItem (UTF8TOQSTRING(label), formats [i].format);
	}	// for (size_t i = 0; i < formatsNum; i++)
}	// QtStringNumberFormatComboBox::QtStringNumberFormatComboBox


QtStringNumberFormatComboBox::QtStringNumberFormatComboBox (
										const QtStringNumberFormatComboBox&)
	: QComboBox ((QWidget*)0)
{
	assert (0 && "QtStringNumberFormatComboBox copy constructor is not allowed.");
}	// QtStringNumberFormatComboBox::QtStringNumberFormatComboBox (const QtStringNumberFormatComboBox& view)


QtStringNumberFormatComboBox& QtStringNumberFormatComboBox::operator = (
										const QtStringNumberFormatComboBox&)
{
	assert (0 && "QtStringNumberFormatComboBox::operator = is not allowed.");
	return *this;
}	// QtStringNumberFormatComboBox::operator =


QtStringNumberFormatComboBox::~QtStringNumberFormatComboBox ( )
{
}	// QtStringNumberFormatComboBox::~QtStringNumberFormatComboBox


char QtStringNumberFormatComboBox::getFormat ( ) const
{
	int	index	= currentIndex ( );
	if (index >= formatsNum)
	{
		INTERNAL_ERROR (exc, "Erreur de gestion de format.",
		                     "QtStringNumberFormatComboBox::getFormat")
		throw exc;
	}	// if (index >= formatsNum)

	return formats [index].format;
}	// QtStringNumberFormatComboBox::getFormat


void QtStringNumberFormatComboBox::setFormat (char format)
{
	for (size_t i = 0; i < formatsNum; i++)
	{
		if (formats [i].format == format)
		{
			setCurrentIndex (i);
			return;
		}	// if (formats [i].format == format)
	}	// for (size_t i = 0; i < formatsNum; i++)

	UTF8String	msg (charset);
	msg << "Format invalide : " << format << ".";
	throw Exception (msg);
}	// QtStringNumberFormatComboBox::setFormat



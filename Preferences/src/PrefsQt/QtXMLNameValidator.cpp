#include "PrefsQt/QtXMLNameValidator.h"
#include <QtUtil/QtUnicodeHelper.h>
#include "PrefsXerces/XercesUtils.h"
#include <xercesc/util/XMLChar.hpp>
#include <assert.h>


QtXMLNameValidator::QtXMLNameValidator (QObject* parent, const char* name)
	: QValidator (parent)
{
}	// QtXMLNameValidator::QtXMLNameValidator


QtXMLNameValidator::QtXMLNameValidator (const QtXMLNameValidator&)
	: QValidator (0)
{
	assert (0 && "QtXMLNameValidator copy constructor is not allowed.");
}	// QtXMLNameValidator::QtXMLNameValidator


QtXMLNameValidator& QtXMLNameValidator::operator = (const QtXMLNameValidator&)
{
	assert (0 && "QtXMLNameValidator operator = is not allowed.");
	return *this;
}	// QtXMLNameValidator::QtXMLNameValidator


QtXMLNameValidator::~QtXMLNameValidator ( )
{
}


QValidator::State QtXMLNameValidator::validate (QString& input, int& pos) const
{
	IN_UTIL UTF8String		ustr	= QtUnicodeHelper::qstringToUTF8String (input);
	const IN_UTIL	UTF16String		u16		= ustr.utf16 ( );
	return true == XERCES_CPP_NAMESPACE_QUALIFIER
				XMLChar1_1::isValidName ((const XMLCh*)u16.utf16 ( ), u16.length ( )) ?
				QValidator::Acceptable : QValidator::Invalid;
/*	IN_UTIL UnicodeString	ustr	= 
				QtUnicodeHelper::qstringToUnicodeString (input);
	return true == XERCES_CPP_NAMESPACE_QUALIFIER 
				XMLChar1_1::isValidName (ustr.unicode ( ), ustr.length ( )) ? 
	       QValidator::Acceptable : QValidator::Invalid; */
}	// QtXMLNameValidator::validate



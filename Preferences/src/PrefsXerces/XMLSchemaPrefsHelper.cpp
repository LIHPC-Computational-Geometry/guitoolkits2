#include "PrefsXerces/XMLSchemaPrefsHelper.h"
#include <TkUtil/Exception.h>
#include <TkUtil/UTF8String.h>
#include "PrefsCore/Section.h"
#include "PrefsCore/DoubleTripletNamedValue.h"
#include "PrefsCore/ColorNamedValue.h"

#include <assert.h>


USING_STD
USING_UTIL


BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");


XMLSchemaPrefsHelper::XMLSchemaPrefsHelper ( )
{
	assert (0 && "XMLSchemaPrefsHelper::XMLSchemaPrefsHelper is not allowed.");
}	// XMLSchemaPrefsHelper::XMLSchemaPrefsHelper


XMLSchemaPrefsHelper::XMLSchemaPrefsHelper (const XMLSchemaPrefsHelper&)
{
	assert (0 && "XMLSchemaPrefsHelper copy constructor is not allowed.");
}	// XMLSchemaPrefsHelper::XMLSchemaPrefsHelper


XMLSchemaPrefsHelper& XMLSchemaPrefsHelper::operator = (const XMLSchemaPrefsHelper&)
{
	assert (0 && "XMLSchemaPrefsHelper operator = is not allowed.");
	return *this;
}	// XMLSchemaPrefsHelper::operator =


XMLSchemaPrefsHelper::~XMLSchemaPrefsHelper  ( )
{
	assert (0 && "XMLSchemaPrefsHelper::~XMLSchemaPrefsHelper is not allowed.");
}	// XMLSchemaPrefsHelper::~XMLSchemaPrefsHelper


string XMLSchemaPrefsHelper::elementToXMLschemaType (const Element& element)
{
	// Types atomiques :
	if (element.getType ( ) == "string")
		return "string";
	if (element.getType ( ) == "bool")
		return "boolean";
	if (element.getType ( ) == "long")
		return "long";
	if (element.getType ( ) == "unsigned long")
		return "unsignedLong";
	if (element.getType ( ) == "double")
		return "double";

	// Types complexes du module "Préférences" :
	if (element.getType ( ) == "section")
		return "container";
	if (element.getType ( ) == "doubleTriplet")
		return "doubleTriplet";
	if (element.getType ( ) == "color")
		return "color";

	UTF8String	errorMsg (charset);
	errorMsg << "Type " << element.getType ( ) 
	         << " inconnu. Conversion à la norme XML schema impossible.";
	throw Exception (errorMsg);
}	// XMLSchemaPrefsHelper::elementToXMLschemaType


UTF8String XMLSchemaPrefsHelper::elementToXMLschemaTypeName (const Element& element)
{
	if (0 != element.getAppType ( ).length ( ))
		return element.getAppType ( );

	const Section*	section	= dynamic_cast<const Section*>(&element);
	if (0 != section)
		return UTF8String ("Section", charset);

	return UTF8String ("element", charset);
}	// XMLSchemaPrefsHelper::elementToXMLschemaTypeName


END_NAMESPACE_PREFS

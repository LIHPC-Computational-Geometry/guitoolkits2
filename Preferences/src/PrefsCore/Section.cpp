#include "PrefsCore/Section.h"

#include <TkUtil/Exception.h>
#include <TkUtil/InternalError.h>
#include <TkUtil/UTF8String.h>

#include <assert.h>


USING_STD
USING_UTIL

BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");

const string Section::typeName ("section");


Section::Section (const UTF8String& name, const UTF8String& comment, bool overloadable, bool safeguardable)
	: Element (name, comment, overloadable, safeguardable), _sections ( ), _namedValues ( )
{
}	// Section::Section


Section::Section (const Section& section)
	: Element (section.getName ( )), _sections ( ), _namedValues ( )
{
	copy (section);
}	// Section::Section (const Section&)


Section& Section::operator = (const Section& section)
{
	copy (section);

	return *this;
}	// Section::operator =


Section::~Section ( )
{
	for (vector<Section*>::iterator its = _sections.begin ( );
	     _sections.end ( ) != its; its++)
		delete *its;
	_sections.clear ( );
	for (vector<NamedValue*>::iterator itnv = _namedValues.begin ( );
	     _namedValues.end ( ) != itnv; itnv++)
		delete *itnv;
	_namedValues.clear ( );
}	// Section::~Section


bool Section::isAscii ( ) const
{
	if (false == Element::isAscii ( ))
		return false;

	for (vector<Section*>::const_iterator its = _sections.begin ( );
	     _sections.end ( ) != its; its++)
		if (false == (*its)->isAscii ( ))
			return false;
	for (vector<NamedValue*>::const_iterator itnv = _namedValues.begin ( );
	     _namedValues.end ( ) != itnv; itnv++)
		if (false == (*itnv)->isAscii ( ))
			return false;

	return true;
}	// Section::isAscii


bool Section::isIso ( ) const
{
	if (false == Element::isIso ( ))
		return false;

	for (vector<Section*>::const_iterator its = _sections.begin ( );
	     _sections.end ( ) != its; its++)
		if (false == (*its)->isIso ( ))
			return false;
	for (vector<NamedValue*>::const_iterator itnv = _namedValues.begin ( );
	     _namedValues.end ( ) != itnv; itnv++)
		if (false == (*itnv)->isIso ( ))
			return false;

	return true;
}	// Section::isIso


Element* Section::clone ( ) const
{
	return new Section (*this);
}	// Section::clone


bool Section::checkForModification (bool raise)			// v 5.7.0
{
	if (true == isOverloadable ( ))
		return true;
		
	if (true == raise)
	{
		UTF8String	message (charset);
		message << "Section::checkForModification. La section " << getName ( ) << " n'est pas modifiable.";
		throw Exception (message);
	}	// if (true == raise)
	
	return false;
}	// Section::checkForModification


bool Section::checkForSafeguard (bool raise)			// v 5.7.0
{
	if (true == isSafeguardable ( ))
		return true;
		
	if (true == raise)
	{
		UTF8String	message (charset);
		message << "Section::checkForSafeguard. La section " << getName ( ) << " n'est pas enregistrable.";
		throw Exception (message);
	}	// if (true == raise)
	
	return false;
}	// Section::checkForSafeguard


bool Section::hasElement (const UTF8String& name) const
{
	if (true == hasSection (name))
		return true;

	return hasNamedValue (name);
}	// Section::hasElement


Element& Section::getElement (const UTF8String& name) const
{
	if (true == hasSection (name))
		return getSection (name);

	return getNamedValue (name);
}	// Section::getElement


bool Section::hasSection (const UTF8String& name) const
{
	for (vector<Section*>::const_iterator	its	= _sections.begin ( );
	     _sections.end ( ) != its; its++)
		if (name == (*its)->getName ( ))
			return true;

	return false;
}	// Section::hasSection


void Section::addSection (Section* section)
{
	checkForModification (true);	// v 5.7.0
	
	if (0 == section)
	{
		UTF8String	error (charset);
		error << "Tentative d'ajout d'une section nulle à la section " << getName ( );
		throw Exception (error);
	}	// if (0 == section)
	if (true == hasSection (section->getName ( )))
	{
		UTF8String	error (charset);
		error << "Echec de l'ajout de la section " << section->getName ( ) << " à la section " << getName ( )
		      << ". Cette section a déjà une sous-section de ce nom.";
		throw Exception (error);
	}	// if (true == hasSection (section->getName ( )))

	section->setParent (this);
	_sections.push_back (section);
}	// Section::addSection


void Section::removeSection (const UTF8String& name)
{
	checkForModification (true);	// v 5.7.0
	
	if (false == hasSection (name))
	{
		UTF8String	error (charset);
		error << "Echec de la suppression de la section " << name << " à la section " << getName ( )
		      << ". Cette section n'a pas de sous-section de ce nom.";
		throw Exception (error);
	}	// if (false == hasSection (name))

	for (vector<Section*>::iterator its = _sections.begin ( ); _sections.end ( ) != its; its++)
		if (name == (*its)->getName ( ))
		{
			delete *its;
			_sections.erase (its);
			return;
		}

	UTF8String	internalError (charset);
	internalError << "Echec de la suppression de la section " << name << " à la section " << getName ( )
		          << ". Cette section n'a pas de sous-section de ce nom.";
	INTERNAL_ERROR (exc, internalError, "Section::removeSection")
	throw exc;
}	// Section::removeSection


Section& Section::getSection (const UTF8String& name) const
{
	for (vector<Section*>::const_iterator its = _sections.begin ( ); _sections.end ( ) != its; its++)
		if (name == (*its)->getName ( ))
			return **its;

	UTF8String	error (charset);
	error << "Echec de la récupération de la section " << name << " depuis la section " << getName ( )
		  << ". Cette section n'a pas de sous-section de ce nom.";
	throw Exception (error);
}	// Section::getSection


vector<Section*> Section::getSections ( ) const
{
	return _sections;
}	// Section::getSections


bool Section::hasNamedValue (const UTF8String& name) const
{
	for (vector<NamedValue*>::const_iterator	itnv	= _namedValues.begin ( ); _namedValues.end ( ) != itnv; itnv++)
		if (name == (*itnv)->getName ( ))
			return true;

	return false;
}	// Section::hasNamedValue


void Section::addNamedValue (NamedValue* namedValue)
{
	checkForModification (true);	// v 5.7.0
	
	if (0 == namedValue)
	{
		UTF8String	error (charset);
		error << "Tentative d'ajout d'une valeur nommée nulle à la section " << getName ( );
		throw Exception (error);
	}	// if (0 == namedValue)
	if (true == hasNamedValue (namedValue->getName ( )))
	{
		UTF8String	error (charset);
		error << "Echec de l'ajout de la valeur nommée " << namedValue->getName ( ) << " à la section " << getName ( )
		      << ". Cette section a déjà une valeur nommée de ce nom.";
		throw Exception (error);
	}	// if (true == hasNamedValue (namedValue->getName ( )))

	namedValue->setParent (this);
	_namedValues.push_back (namedValue);
}	// Section::addNamedValue


void Section::removeNamedValue (const UTF8String& name)
{
	checkForModification (true);	// v 5.7.0
	
	if (false == hasNamedValue (name))
	{
		UTF8String	error (charset);
		error << "Echec de la suppression de la valeur nommée " << name << " à la section " << getName ( )
		      << ". Cette section n'a pas de valeur nommée de ce nom.";
		throw Exception (error);
	}	// if (false == hasNamedValue (name))

	for (vector<NamedValue*>::iterator itnv = _namedValues.begin ( ); _namedValues.end ( ) != itnv; itnv++)
		if (name == (*itnv)->getName ( ))
		{
			delete *itnv;
			_namedValues.erase (itnv);
			return;
		}

	UTF8String	internalError (charset);
	internalError << "Echec de la suppression de la valeur nommée " << name << " à la section " << getName ( )
		          << ". Cette section n'a pas de valeur nommée de ce nom.";
	INTERNAL_ERROR (exc, internalError, "Section::removeNamedValue")
	throw exc;
}	// Section::removeNamedValue


NamedValue& Section::getNamedValue (const UTF8String& name) const
{
	for (vector<NamedValue*>::const_iterator itnv = _namedValues.begin ( ); _namedValues.end ( ) != itnv; itnv++)
		if (name == (*itnv)->getName ( ))
			return **itnv;

	UTF8String	error (charset);
	error << "Echec de la récupération de la valeur nommée " << name << " depuis la section " << getName ( )
		  << ". Cette section n'a pas de valeur nommée de ce nom.";
	throw Exception (error);
}	// Section::getNamedValue


vector<NamedValue*> Section::getNamedValues ( ) const
{
	return _namedValues;
}	// Section::getNamedValues


void Section::renameElement (const UTF8String& name, const UTF8String& newName)
{
	checkForModification (true);	// v 5.7.0
	
	UTF8String	errorMsg (charset);
	errorMsg << "Impossible de renommer l'élément " << name << " en " << newName << " :\n";

	if (false == hasElement (name))
	{
		errorMsg << " la section " << getName ( ) << " n'a pas d'élément de ce nom.";
		throw Exception (errorMsg);
	}	// if (false == hasElement (name))
	if (name == newName)
		return;

	if (true == hasElement (newName))
	{
		errorMsg << " la section " << getName ( ) << " a déjà un élément du nom de " << newName << ".";
		throw Exception (errorMsg);
	}	// if (true == hasElement (newName))

	if (0 == newName.length ( ))
	{
		errorMsg << " nom ne contenant pas de caractères.";
		throw Exception (errorMsg);
	}	// if (0 == newName.length ( ))

	if (string::npos != newName.ascii ( ).find (' '))
	{
		errorMsg << " nom contenant des espaces.";
		throw Exception (errorMsg);
	}	// if (string::npos != newName.find (' '))

	getElement (name).setName (newName);
}	// Section::renameElement


void Section::rename (const UTF8String& name)
{
	checkForModification (true);	// v 5.7.0
	
	if (true == hasParent ( ))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Impossible de renommer la section " << getName ( ) << " par Section::rename : cette section a un parent.";
		throw Exception (errorMsg);
	}	// if (true == hasParent ( ))

	setName (name);
}	// Section::rename


void Section::print (ostream& stream, size_t indent) const
{
	size_t	i	= 0;
	for (i = 0; i < indent; i++) stream << '\t';
	stream << getName ( ) << endl;
	for (i = 0; i < indent; i++) stream << '\t';
	stream << "{" << endl;
	for (vector<Section*>::const_iterator its = _sections.begin ( ); _sections.end ( ) != its; its++)
	{
		(*its)->print (stream, indent + 1);
		stream << endl;
	}	// for (vector<Section*>::iterator its = _sections.begin ( );
	for (vector<NamedValue*>::const_iterator itnv = _namedValues.begin ( ); _namedValues.end ( ) != itnv; itnv++)
	{
		(*itnv)->print (stream, indent + 1);
		stream << endl;
	}	// for (vector<NamedValue*>::iterator itnv = _namedValues.begin ( );
	for (i = 0; i < indent; i++) stream << '\t';
	stream << "}" << '\t' << "// " << getName ( );
	if (0 != getComment ( ).length ( ))
		stream << "\t// " << getComment ( ).ascii ( );
}	// Section::print


void Section::copy (const Section& section)
{
	Element::copy (section);

	vector<Section*>	subSections	= section.getSections ( );
	for (vector<Section*>::const_iterator its	= subSections.begin ( ); subSections.end ( ) != its; its++)
		_sections.push_back (dynamic_cast<Section*>((*its)->clone ( )));

	vector<NamedValue*>	values	= section.getNamedValues ( );
	for (vector<NamedValue*>::const_iterator itv	= values.begin ( ); values.end ( ) != itv; itv++)
		_namedValues.push_back (dynamic_cast<NamedValue*>((*itv)->clone ( )));
}	// Section::copy

END_NAMESPACE_PREFS

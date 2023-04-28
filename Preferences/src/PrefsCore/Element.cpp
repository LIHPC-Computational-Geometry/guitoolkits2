#include "PrefsCore/Element.h"
#include "PrefsCore/Section.h"
#include <TkUtil/Exception.h>
#include <TkUtil/UTF8String.h>

#include <assert.h>


USING_STD
USING_UTIL


BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");


// ===========================================================================
//                       LA CLASSE Element::OriginInfos
// ===========================================================================


Element::OriginInfos::OriginInfos (const UTF8String& name)
	: _name (name)
{
}	// OriginInfos::OriginInfos


Element::OriginInfos::OriginInfos (const Element::OriginInfos& toCopy)
	: _name (toCopy.getName ( ))
{
}	// OriginInfos::OriginInfos


Element::OriginInfos& Element::OriginInfos::operator = (const Element::OriginInfos& toCopy)
{
	if (&toCopy != this)
		copy (toCopy);

	return *this;
}	// OriginInfos::operator =


Element::OriginInfos::~OriginInfos ( )
{
}	// OriginInfos::~OriginInfos


Element::OriginInfos* Element::OriginInfos::clone ( ) const
{
	return new Element::OriginInfos (*this);
}	// OriginInfos::clone


void Element::OriginInfos::copy (const OriginInfos& toCopy)
{
	if (&toCopy != this)
		_name	= toCopy.getName ( );
}	// OriginInfos::copy


// ===========================================================================
//                            LA CLASSE Element
// ===========================================================================


Element::Element (const UTF8String& name, const UTF8String& comment, bool overloadable, bool safeguardable)
	: _name ( ), _overloadable (overloadable), _safeguardable (safeguardable), _appType ( ), _parent (0), _comment (comment), _infos ( )
{
	setName (name);
}	// Element::Element


Element::Element (const Element& element)
	: _name ( ), _overloadable (element._overloadable), _safeguardable (element._safeguardable), _appType ( ), _parent (0), _comment (element.getComment ( )),_infos ( )
{
	setName (element.getName ( ));
}	// Element::Element (const Element&)


Element& Element::operator = (const Element& element)
{
	copy (element);

	return *this;
}	// Element::operator =


bool Element::isOverloadable ( ) const					// v 5.7.0
{
	if ((true == hasParent ( )) && (false == getParent ( ).isOverloadable ( )))
		return false;
			
	return _overloadable;
}	// Element::isOverloadable


void Element::setOverloadable (bool overloadable)		// v 5.7.0
{
	_overloadable	= overloadable;
}	// Element::setOverloadable


bool Element::checkForModification (bool raise)			// v 5.7.0
{
	if (true == isOverloadable ( ))
		return true;
		
	if (true == raise)
	{
		UTF8String	message (charset);
		message << "Element::checkForModification. L'élément " << getName ( ) << " n'est pas modifiable.";
		throw Exception (message);
	}	// if (true == raise)
	
	return false;
}	// Element::checkForModification


bool Element::isSafeguardable ( ) const					// v 5.7.0
{
	if ((true == hasParent ( )) && (false == getParent ( ).isSafeguardable ( )))
		return false;
		
	return _safeguardable;
}	// Element::isSafeguardable


void Element::setSafeguardable (bool safeguardable)		// v 5.7.0
{
	_safeguardable	= safeguardable;
}	// Element::setSafeguardable


bool Element::checkForSafeguard (bool raise)			// v 5.7.0
{
	if (true == isSafeguardable ( ))
		return true;
		
	if (true == raise)
	{
		UTF8String	message (charset);
		message << "Element::checkForSafeguard. L'élément " << getName ( ) << " n'est pas enregistrable.";
		throw Exception (message);
	}	// if (true == raise)
	
	return false;
}	// Element::checkForSafeguard


const UTF8String& Element::getAppType ( ) const
{
	return _appType;
}	// Element::getAppType


void Element::setAppType (const UTF8String& appType)
{
	_appType	= appType;
}	// Element::setAppType


Section& Element::getParent ( ) const
{
	if (0 == _parent)
	{
		UTF8String	message (charset);
		message << "Element::getParent. L'élément " << getName ( ) << " n'a pas de parent.";
		throw Exception (message);
	}	// if (0 == _parent)

	return *_parent;
}	// Element::getParent


const UTF8String& Element::getComment ( ) const
{
	return _comment;
}	// Element::getComment


void Element::setComment (const UTF8String& comment)
{
	_comment	= comment;
}	// Element::setComment


bool Element::hasComment ( ) const
{
	return 0 == getComment ( ).length ( ) ? false : true;
}	// Element::hasComment


bool Element::isAscii ( ) const
{
	if (false == getAppType ( ).isAscii ( ))
		return false;

	if (false == getName ( ).isAscii ( ))
		return false;

	if (false == getComment ( ).isAscii ( ))
		return false;

	return true;
}	// Element::isAscii


bool Element::isIso ( ) const
{
	if (false == getAppType ( ).isIso ( ))
		return false;

	if (false == getName ( ).isIso ( ))
		return false;

	if (false == getComment ( ).isIso ( ))
		return false;

	return true;
}	// Element::isIso


void Element::setName (const UTF8String& name)
{
	// On joue avec le charset car en cas de variables statiques globales dérivées d'Element setName peut être appelé avant que la variable
	// statique charset de ce fichier ne soit initialisée ...
	// Du coup la préparation du message d'erreur lève une exception car charset.charset ( ) vaut UNKNOWN ...
	Charset	cs (Charset::UNKNOWN == charset.charset ( ) ? Charset::ASCII : charset.charset ( ));
//	UTF8String	errorMsg (charset);
	UTF8String	errorMsg (cs);
	if (Charset::UNKNOWN != charset.charset ( ))
		errorMsg << "Impossibilité de renommer l'élément " << getName ( ) << " en " << name << " : ";
	else
		errorMsg << "Impossibilite de renommer l'element " << getName ( ) << " en " << name << " : ";

	if (0 == name.length ( ))
	{
		errorMsg << "nom nul.";
		throw Exception (errorMsg);
	}	// if (0 == _name.length ( ))

	if (string::npos != name.ascii ( ).find (' '))
	{
		errorMsg << " nom contenant des espaces.";
		throw Exception (errorMsg);
	}	// if (string::npos != name.find (' '))

	_name	= name;
}	// Element::setName


void Element::setParent (Section* parent)
{
	assert ((0 == _parent) && "Element::setParent : parent is already affected.");
	_parent	= parent;
}	// Element::setParent


void Element::setOriginInfos (Element::OriginInfos* infos)
{
	_infos.reset (infos);
}	// Element::setOriginInfos


Element::OriginInfos* Element::getOriginInfos ( )
{
	return _infos.get ( );
}	// Element::getOriginInfos


const Element::OriginInfos* Element::getOriginInfos ( ) const
{
	return _infos.get ( );
}	// Element::getOriginInfos


void Element::copy (const Element& element)
{
	if (&element != this)
	{
		setAppType (element.getAppType ( ));
		setName (element.getName ( ));
		setComment (element.getComment ( ));
		if (0 != element.getOriginInfos ( ))
			setOriginInfos (element.getOriginInfos ( )->clone ( ));
		else
			setOriginInfos (0);
	}	// if (&element != this)
}	// Element::copy


ostream& operator << (ostream& os, const Element& elem)
{
	elem.print (os, 0);
	return os;
}	// operator << (ostream& os, const Element& elem)


END_NAMESPACE_PREFS

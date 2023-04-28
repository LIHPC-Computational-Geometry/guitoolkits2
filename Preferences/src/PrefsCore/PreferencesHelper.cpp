#include "PrefsCore/PreferencesHelper.h"
#include <TkUtil/Exception.h>
#include <TkUtil/UTF8String.h>

#include <assert.h>

#include <iostream>


USING_STD
USING_UTIL


BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");


PreferencesHelper::PreferencesHelper ( )
{
	assert (0 && "PreferencesHelper::PreferencesHelper is not allowed.");
}	// PreferencesHelper::PreferencesHelper


PreferencesHelper::PreferencesHelper (const PreferencesHelper&)
{
	assert (0 && "PreferencesHelper::PreferencesHelper is not allowed.");
}


PreferencesHelper& PreferencesHelper::operator = (const PreferencesHelper&)
{
	assert (0 && "PreferencesHelper::operator = is not allowed.");
	return *this;
}	// PreferencesHelper::operator =


PreferencesHelper::~PreferencesHelper ( )
{
	assert (0 && "PreferencesHelper::~PreferencesHelper is not allowed.");
}	// PreferencesHelper::~PreferencesHelper


Section& PreferencesHelper::getSection (Section& parent, const UTF8String& name, const UTF8String& comment, bool overloadable, bool safeguardable)
{
	try
	{
		return parent.getSection (name);
	}
	catch (...)
	{
		Section*	section	= new Section (name, comment, overloadable, safeguardable);
		parent.addSection (section);
		return parent.getSection (name);
	}
}	// PreferencesHelper::getSection


BoolNamedValue& PreferencesHelper::getBooleanNamedValue (Section& parent, const UTF8String& name, bool value, const UTF8String& comment, bool overloadable, bool safeguardable)
{
	try
	{
		NamedValue&			nv	= parent.getNamedValue (name);
		BoolNamedValue*	boolean	= dynamic_cast<BoolNamedValue*>(&nv);
		if (0 == boolean)
		{
			UTF8String	errorMsg (charset);
			errorMsg << name.ascii ( ) << " n'est pas une valeur nommée de type bouléen.";
			throw Exception (errorMsg);
		}

		return *boolean;
	}
	catch (...)
	{
		BoolNamedValue*	boolean	= new BoolNamedValue (name, value, comment, overloadable, safeguardable);
		parent.addNamedValue (boolean);
		return *boolean;
	}
}	// PreferencesHelper::getBooleanNamedValue


StringNamedValue& PreferencesHelper::getStringNamedValue (Section& parent, const UTF8String& name, const UTF8String& value, const UTF8String& comment, bool overloadable, bool safeguardable)
{
	try
	{
		NamedValue&			nv	= parent.getNamedValue (name);
		StringNamedValue*	str	= dynamic_cast<StringNamedValue*>(&nv);
		if (0 == str)
		{
			UTF8String	errorMsg (charset);
			errorMsg << name.ascii ( ) << " n'est pas une valeur nommée de type chaine de caractères.";
			throw Exception (errorMsg);
		}

		return *str;
	}
	catch (...)
	{
		StringNamedValue*	str	= new StringNamedValue (name, value, comment, overloadable, safeguardable);
		parent.addNamedValue (str);
		return *str;
	}
}	// PreferencesHelper::getStringNamedValue


LongNamedValue& PreferencesHelper::getLongNamedValue (Section& parent, const UTF8String& name, long value, const UTF8String& comment, bool overloadable, bool safeguardable)
{
	try
	{
		NamedValue&		nv	= parent.getNamedValue (name);
		LongNamedValue*	l	= dynamic_cast<LongNamedValue*>(&nv);
		if (0 == l)
		{
			UTF8String	errorMsg (charset);
			errorMsg << name.ascii ( ) << " n'est pas une valeur nommée de type entier long.";
			throw Exception (errorMsg);
		}

		return *l;
	}
	catch (...)
	{
		LongNamedValue*	l	= new LongNamedValue (name, value, comment, overloadable, safeguardable);
		parent.addNamedValue (l);
		return *l;
	}
}	// PreferencesHelper::getLongNamedValue


UnsignedLongNamedValue& PreferencesHelper::getUnsignedLongNamedValue (Section& parent, const UTF8String& name, unsigned long value, const UTF8String& comment, bool overloadable, bool safeguardable)
{
	try
	{
		NamedValue&				nv	= parent.getNamedValue (name);
		UnsignedLongNamedValue*	ul	=dynamic_cast<UnsignedLongNamedValue*>(&nv);
		if (0 == ul)
		{
			UTF8String	errorMsg (charset);
			errorMsg << name.ascii ( ) << " n'est pas une valeur nommée de type entier long non signé.";
			throw Exception (errorMsg);
		}

		return *ul;
	}
	catch (...)
	{
		UnsignedLongNamedValue*	ul	= new UnsignedLongNamedValue (name, value, comment, overloadable, safeguardable);
		parent.addNamedValue (ul);
		return *ul;
	}
}	// PreferencesHelper::getUnsignedLongNamedValue


DoubleNamedValue& PreferencesHelper::getDoubleNamedValue (Section& parent, const UTF8String& name, double value, const UTF8String& comment, bool overloadable, bool safeguardable)
{
	try
	{
		NamedValue&			nv	= parent.getNamedValue (name);
		DoubleNamedValue*	d	= dynamic_cast<DoubleNamedValue*>(&nv);
		if (0 == d)
		{
			UTF8String	errorMsg (charset);
			errorMsg << name.ascii ( ) << " n'est pas une valeur nommée de type réel double précision.";
			throw Exception (errorMsg);
		}

		return *d;
	}
	catch (...)
	{
		DoubleNamedValue*	d	= new DoubleNamedValue (name, value, comment, overloadable, safeguardable);
		parent.addNamedValue (d);
		return *d;
	}
}	// PreferencesHelper::getDoubleNamedValue


DoubleTripletNamedValue& PreferencesHelper::getDoubleTripletNamedValue (Section& parent, const UTF8String& name, double x, double y, double z, const UTF8String& comment, bool overloadable, bool safeguardable)
{
	try
	{
		NamedValue&					nv		= parent.getNamedValue (name);
		DoubleTripletNamedValue*	triplet	= dynamic_cast<DoubleTripletNamedValue*>(&nv);
		if (0 == triplet)
		{
			UTF8String	errorMsg (charset);
			errorMsg << name.ascii ( ) << " n'est pas une valeur nommée de type triplet de réels double précision.";
			throw Exception (errorMsg);
		}

		return *triplet;
	}
	catch (...)
	{
		DoubleTripletNamedValue*	triplet	= new DoubleTripletNamedValue(name, x, y, z, comment, overloadable, safeguardable);
		parent.addNamedValue (triplet);
		return *triplet;
	}
}	// PreferencesHelper::getDoubleTripletNamedValue


ColorNamedValue& PreferencesHelper::getColorNamedValue (Section& parent, const UTF8String& name, double r, double g, double b, const UTF8String& comment, bool overloadable, bool safeguardable)
{
	try
	{
		NamedValue&			nv		= parent.getNamedValue (name);
		ColorNamedValue*	color	= dynamic_cast<ColorNamedValue*>(&nv);
		if (0 == color)
		{
			UTF8String	errorMsg (charset);
			errorMsg << name.ascii ( ) << " n'est pas une valeur nommée de type couleur.";
			throw Exception (errorMsg);
		}

		return *color;
	}
	catch (...)
	{
		ColorNamedValue*	color	= new ColorNamedValue (name, r, g, b, comment, overloadable, safeguardable);
		parent.addNamedValue (color);
		return *color;
	}
}	// PreferencesHelper::getColorNamedValue


void PreferencesHelper::updateBoolean (Section& section, const UTF8String& boolName, bool value)
{
	BoolNamedValue&	boolean	= PreferencesHelper::getBooleanNamedValue (section, boolName);
	if (true == boolean.isOverloadable ( ))	// v 5.7.0
		boolean.setValue (value);
}	// PreferencesHelper::updateBoolean


void PreferencesHelper::updateBoolean (Section& section, const BoolNamedValue& boolean)
{
	BoolNamedValue&	b	= PreferencesHelper::getBooleanNamedValue (section, boolean.getName ( ), boolean.getValue ( ), boolean.getComment ( ), true, boolean.isSafeguardable ( ));
	if (true == b.isOverloadable ( ))	// v 5.7.0
		b	= boolean;
}	// PreferencesHelper::updateBoolean


void PreferencesHelper::updateString (Section& section, const UTF8String& strName, const UTF8String& value)
{
	StringNamedValue&	str	= PreferencesHelper::getStringNamedValue (section, strName);
	if (true == str.isOverloadable ( ))	// v 5.7.0
		str.setValue (value);
}	// PreferencesHelper::updateString


void PreferencesHelper::updateString (Section& section, const StringNamedValue& str)
{
	StringNamedValue&	s	= PreferencesHelper::getStringNamedValue (section, str.getName ( ), str.getValue ( ), str.getComment ( ), true, str.isSafeguardable ( ));
	if (true == s.isOverloadable ( ))	// v 5.7.0
		s	= str;
}	// PreferencesHelper::updateString


void PreferencesHelper::updateLong (Section& section, const UTF8String& name, long value)
{
	LongNamedValue&	l	= PreferencesHelper::getLongNamedValue (section, name);
	if (true == l.isOverloadable ( ))	// v 5.7.0
		l.setValue (value);
}	// PreferencesHelper::updateLong


void PreferencesHelper::updateLong (Section& section, const LongNamedValue& value)
{
	LongNamedValue&	l	= PreferencesHelper::getLongNamedValue (section, value.getName ( ), value.getValue ( ), value.getComment ( ), true, value.isSafeguardable ( ));
	if (true == l.isOverloadable ( ))	// v 5.7.0
		l	= value;
}	// PreferencesHelper::updateLong


void PreferencesHelper::updateUnsignedLong (Section& section, const UTF8String& name, unsigned long value)
{
	UnsignedLongNamedValue&	ul	= PreferencesHelper::getUnsignedLongNamedValue (section, name);
	if (true == ul.isOverloadable ( ))	// v 5.7.0
		ul.setValue (value);
}	// PreferencesHelper::updateUnsignedLong


void PreferencesHelper::updateUnsignedLong (Section& section, const UnsignedLongNamedValue& value)
{
	UnsignedLongNamedValue&	ul	= PreferencesHelper::getUnsignedLongNamedValue (section, value.getName ( ), value.getValue ( ), value.getComment ( ), true, value.isSafeguardable ( ));
	if (true == ul.isOverloadable ( ))	// v 5.7.0
		ul	= value;
}	// PreferencesHelper::updateUnsignedLong


void PreferencesHelper::updateDouble (Section& section, const UTF8String& name, double value)
{
	DoubleNamedValue&	d	= PreferencesHelper::getDoubleNamedValue (section, name);
	if (true == d.isOverloadable ( ))	// v 5.7.0
		d.setValue (value);
}	// PreferencesHelper::updateDouble


void PreferencesHelper::updateDouble (Section& section, const DoubleNamedValue& value)
{
	DoubleNamedValue&	d	= PreferencesHelper::getDoubleNamedValue (section, value.getName ( ), value.getValue ( ), value.getComment ( ), true, value.isSafeguardable ( ));
	if (true == d.isOverloadable ( ))	// v 5.7.0
		d	= value;
}	// PreferencesHelper::updateDouble


void PreferencesHelper::updateDoubleTriplet (Section& section, const UTF8String& name, double x, double y, double z)
{
	DoubleTripletNamedValue&	d	= PreferencesHelper::getDoubleTripletNamedValue (section, name);
	if (true == d.isOverloadable ( ))	// v 5.7.0
		d.setValue (x, y, z);
}	// PreferencesHelper::updateDoubleTriplet


void PreferencesHelper::updateDoubleTriplet (Section& section, const DoubleTripletNamedValue& triplet)
{
	DoubleTripletNamedValue&	dt	= PreferencesHelper::getDoubleTripletNamedValue (section, triplet.getName ( ), triplet.getX ( ), triplet.getY ( ), triplet.getZ ( ), triplet.getComment ( ), true, triplet.isSafeguardable ( ));
	if (true == dt.isOverloadable ( ))	// v 5.7.0
		dt	= triplet;
}	// PreferencesHelper::updateDoubleTriplet


void PreferencesHelper::updateColor (Section& section, const UTF8String& colorName,double red, double green, double blue)
{
	ColorNamedValue&	color	= PreferencesHelper::getColorNamedValue (section, colorName);
	if (true == color.isOverloadable ( ))	// v 5.7.0
		color.setValue (red, green, blue);
}	// PreferencesHelper::updateColor


void PreferencesHelper::updateColor (Section& section, const ColorNamedValue& color)
{
	ColorNamedValue&	c	= PreferencesHelper::getColorNamedValue (section, color.getName ( ), color.getRed ( ), color.getGreen ( ), color.getBlue ( ), color.getComment ( ), true, color.isSafeguardable ( ));
	if (true == c.isOverloadable ( ))	// v 5.7.0
		c	= color;
}	// PreferencesHelper::updateColor


void PreferencesHelper::getBoolean (const Section& section, const UTF8String& name, bool& value)
{
	try
	{
		NamedValue&		nv	= section.getNamedValue (name);
		BoolNamedValue*	b	= dynamic_cast<BoolNamedValue*>(&nv);
		if (0 == b)
		{
			UTF8String	mess (charset);
			mess << "Booléen " << name << " non trouvé dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
			value	= b->getValue ( );
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Booléen " << name << " non trouvé dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getBoolean


void PreferencesHelper::getBoolean (const Section& section, BoolNamedValue& value)
{
	try
	{
		NamedValue&		nv	= section.getNamedValue (value.getName ( ));
		BoolNamedValue*	b	= dynamic_cast<BoolNamedValue*>(&nv);
		if (0 == b)
		{
			UTF8String	mess (charset);
			mess << "Booléen " << value.getName ( ) << " non trouvé dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
		{
			value	= *b;
			value.setSafeguardable (b->isSafeguardable ( ));	// v 5.7.0
			value.setOverloadable (b->isOverloadable ( ));		// v 5.7.0
		}
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Booléen " << value.getName ( ) << " non trouvé dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getBoolean


void PreferencesHelper::getString (const Section& section, const UTF8String& name, UTF8String& value)
{
	try
	{
		NamedValue&			nv	= section.getNamedValue (name);
		StringNamedValue*	s	= dynamic_cast<StringNamedValue*>(&nv);
		if (0 == s)
		{
			UTF8String	mess (charset);
			mess << "Chaine de caractères " << name << " non trouvée dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
			value	= s->getValue ( );
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Chaine de caractères " << name << " non trouvée dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getString


void PreferencesHelper::getString (const Section& section, StringNamedValue& value)
{
	try
	{
		NamedValue&			nv	= section.getNamedValue (value.getName ( ));
		StringNamedValue*	s	= dynamic_cast<StringNamedValue*>(&nv);
		if (0 == s)
		{
			UTF8String	mess (charset);
			mess << "Chaine de caractères " << value.getName ( ) << " non trouvée dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
		{
			value	= *s;
			value.setSafeguardable (s->isSafeguardable ( ));	// v 5.7.0
			value.setOverloadable (s->isOverloadable ( ));		// v 5.7.0
		}
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Chaine de caractères " << value.getName ( ) << " non trouvée dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getString


void PreferencesHelper::getString (const Section& section, const UTF8String& name, string& value)
{
	UTF8String	uValue (value);
	PreferencesHelper::getString (section, name, uValue);

	value	= uValue.ascii ( );
}	// PreferencesHelper::getString


void PreferencesHelper::getLong (const Section& section, const UTF8String& name, long& value)
{
	try
	{
		NamedValue&		nv	= section.getNamedValue (name);
		LongNamedValue*	l	= dynamic_cast<LongNamedValue*>(&nv);
		if (0 == l)
		{
			UTF8String	mess (charset);
			mess << "Entier long " << name << " non trouvé dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
			value	= l->getValue ( );
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Entier long " << name << " non trouvé dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getLong


void PreferencesHelper::getLong (const Section& section, LongNamedValue& value)
{
	try
	{
		NamedValue&		nv	= section.getNamedValue (value.getName ( ));
		LongNamedValue*	l	= dynamic_cast<LongNamedValue*>(&nv);
		if (0 == l)
		{
			UTF8String	mess (charset);
			mess << "Entier long " << value.getName ( ) << " non trouvé dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
		{
			value	= *l;
			value.setSafeguardable (l->isSafeguardable ( ));	// v 5.7.0
			value.setOverloadable (l->isOverloadable ( ));		// v 5.7.0
		}
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Entier long " << value.getName ( ) << " non trouvé dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getLong


void PreferencesHelper::getUnsignedLong (const Section& section, const UTF8String& name, unsigned long& value)
{
	try
	{
		NamedValue&				nv	= section.getNamedValue (name);
		UnsignedLongNamedValue*	ul	= dynamic_cast<UnsignedLongNamedValue*>(&nv);
		if (0 == ul)
		{
			UTF8String	mess (charset);
			mess << "Entier long non signé " << name << " non trouvé dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
			value	= ul->getValue ( );
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Entier long non signé " << name << " non trouvé dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getUnsignedLong


void PreferencesHelper::getUnsignedLong (const Section& section, UnsignedLongNamedValue& value)
{
	try
	{
		NamedValue&				nv	= section.getNamedValue (value.getName ( ));
		UnsignedLongNamedValue*	ul	= dynamic_cast<UnsignedLongNamedValue*>(&nv);
		if (0 == ul)
		{
			UTF8String	mess (charset);
			mess << "Entier long non signé " << value.getName ( ) << " non trouvé dans la section " << section.getName ( ) << ".";
		}
		else
		{
			value	= *ul;
			value.setSafeguardable (ul->isSafeguardable ( ));	// v 5.7.0
			value.setOverloadable (ul->isOverloadable ( ));		// v 5.7.0
		}
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Entier long non signé " << value.getName ( ) << " non trouvé dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getUnsignedLong


void PreferencesHelper::getDouble (const Section& section, const UTF8String& name, double& value)
{
	try
	{
		NamedValue&			nv	= section.getNamedValue (name);
		DoubleNamedValue*	d	= dynamic_cast<DoubleNamedValue*>(&nv);
		if (0 == d)
		{
			UTF8String	mess (charset);
			mess << "Réel double précision " << name << " non trouvé dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
			value	= d->getValue ( );
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Réel double précision " << name << " non trouvé dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getDouble


void PreferencesHelper::getDouble (const Section& section, DoubleNamedValue& value)
{
	try
	{
		NamedValue&			nv	= section.getNamedValue (value.getName ( ));
		DoubleNamedValue*	d	= dynamic_cast<DoubleNamedValue*>(&nv);
		if (0 == d)
		{
			UTF8String	mess (charset);
			mess << "Réel double précision " << value.getName ( ) << " non trouvé dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
		{
			value	= *d;
			value.setSafeguardable (d->isSafeguardable ( ));	// v 5.7.0
			value.setOverloadable (d->isOverloadable ( ));		// v 5.7.0
		}
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Réel double précision " << value.getName ( ) << " non trouvé dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getDouble


void PreferencesHelper::getDoubleTriplet (const Section& section, const UTF8String& name, double& x, double& y, double& z)
{
	try
	{
		NamedValue&					nv	= section.getNamedValue (name);
		DoubleTripletNamedValue*	dt	= dynamic_cast<DoubleTripletNamedValue*>(&nv);
		if (0 == dt)
		{
			UTF8String	mess (charset);
			mess << "Triplet de réels double précision " << name << " non trouvé dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
		{
			x	= dt->getX ( );
			y	= dt->getY ( );
			z	= dt->getZ ( );
		}
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Triplet de réels double précision " << name << " non trouvé dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getDoubleTriplet


void PreferencesHelper::getDoubleTriplet (const Section& section, DoubleTripletNamedValue& triplet)
{
	try
	{
		NamedValue&					nv	= section.getNamedValue (triplet.getName ( ));
		DoubleTripletNamedValue*	dt	= dynamic_cast<DoubleTripletNamedValue*>(&nv);
		if (0 == dt)
		{
			UTF8String	mess (charset);
			mess << "Triplet de réels double précision " << triplet.getName ( )<< " non trouvé dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
		{
			triplet	= *dt;
			triplet.setSafeguardable (dt->isSafeguardable ( ));	// v 5.7.0
			triplet.setOverloadable (dt->isOverloadable ( ));		// v 5.7.0
		}
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Triplet de réels double précision " << triplet.getName ( ) << " non trouvé dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getDoubleTriplet


void PreferencesHelper::getColor (const Section& section, const UTF8String& name, double& r, double& g, double& b)
{
	try
	{
		NamedValue&			nv	= section.getNamedValue (name);
		ColorNamedValue*	c	= dynamic_cast<ColorNamedValue*>(&nv);
		if (0 == c)
		{
			UTF8String	mess (charset);
			mess << "Couleur " << name  << " non trouvée dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
		{
			r	= c->getRed ( );
			g	= c->getGreen ( );
			b	= c->getBlue ( );
		}
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Couleur " << name << " non trouvée dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getColor


void PreferencesHelper::getColor (const Section& section, ColorNamedValue& color)
{
	try
	{
		NamedValue&			nv	= section.getNamedValue (color.getName ( ));
		ColorNamedValue*	c	= dynamic_cast<ColorNamedValue*>(&nv);
		if (0 == c)
		{
			UTF8String	mess (charset);
			mess << "Couleur " << color.getName ( ) << " non trouvée dans la section " << section.getName ( ) << ".";
			ConsoleOutput::cout ( ) << mess << co_endl;
		}
		else
		{
			color	= *c;
			color.setSafeguardable (c->isSafeguardable ( ));	// v 5.7.0
			color.setOverloadable (c->isOverloadable ( ));		// v 5.7.0
		}
	}
	catch (...)
	{
		UTF8String	mess (charset);
		mess << "Couleur " << color.getName ( ) << " non trouvée dans la section " << section.getName ( ) << ".";
		ConsoleOutput::cout ( ) << mess << co_endl;
	}
}	// PreferencesHelper::getColor


END_NAMESPACE_PREFS

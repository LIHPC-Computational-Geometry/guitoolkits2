#include "PrefsCore/Section.h"
#include "PrefsCore/BoolNamedValue.h"
#include "PrefsCore/ColorNamedValue.h"
#include "PrefsCore/LongNamedValue.h"
#include "PrefsCore/UnsignedLongNamedValue.h"
#include "PrefsCore/DoubleNamedValue.h"
#include "PrefsCore/DoubleTripletNamedValue.h"
#include "PrefsCore/StringNamedValue.h"
#include "PrefsXerces/XMLLoader.h"
#include <TkUtil/Exception.h>
#include <TkUtil/UTF8String.h>

#include <iostream>
#include <stdexcept>


USING_PREFS
USING_UTIL
USING_STD

static const Charset	charset ("àéèùô");

static void addLevel (Section& section, size_t depth);


int main (int argc, char* argv [])
{
	const size_t depth	= 3;

	Section*	mainSection	= 0;
	Section*	copy		= 0;

	try
	{
		cout << "Creating tree. Depth = " << depth << " ..." << endl;
		mainSection	= new Section ("Main");
		addLevel (*mainSection, depth - 1);
		cout << "Tree created." << endl;
		cout << *mainSection << endl;

		cout << endl << endl << "Copying tree ..." << endl;
		copy	= new Section (*mainSection);
		cout << "Section copied." << endl;
		cout << *copy << endl;

		if (1 < argc)
		{
			cout << "Saving configuration in XML file " << argv [1] 
			     << " ..." << endl;
			XMLLoader::save (*mainSection, argv [1]);
			cout << "Configuration saved in " << argv [1] << "." << endl;
		}	// if (1 < argc)
	}
	catch (const Exception& exc)
	{
		cout << "Exception caught : " << exc.getMessage ( ) << endl;
		return -1;
	}
	catch (const exception& stdExc)
	{
		cout << "Standard exception caught : " << stdExc.what ( ) << endl;
		return -1;
	}
	catch (...)
	{
		cout << "Unexpected exception caught." << endl;
		return -1;
	}

	delete mainSection;
	delete copy;

	return 0;
}	// main



static void addLevel (Section& section, size_t depth)
{
	if (0 == depth)
		return;

	for (size_t i = 1; i <= 3; i++)
	{
		const UTF8String	commentBase ("Commentaire de l'element ", charset);
		UTF8String	name;
		name << "Section_" << (unsigned long)depth << "_" << (unsigned long)i;
		Section*	subsection	= new Section (name.ascii ( ));
		UTF8String	comment	= commentBase + subsection->getName ( );
		subsection->setComment (comment);
		addLevel (*subsection, depth - 1);
		section.addSection (subsection);

		static bool			boolValue	= true;
		BoolNamedValue*		bValue	= 
					new BoolNamedValue ("bvalue", boolValue = !boolValue);
		comment	= commentBase + bValue->getName ( );
		bValue->setComment (comment);
		subsection->addNamedValue (bValue);

		static size_t		count	= 0;
		LongNamedValue*		lValue	= 
			new LongNamedValue ("lvalue", true == boolValue ? count++ : -count);
		comment	= commentBase + lValue->getName ( );
		lValue->setComment (comment);
		subsection->addNamedValue (lValue);

		UnsignedLongNamedValue*		ulValue	= 
						new UnsignedLongNamedValue ("ulvalue", count++);
		comment	= commentBase + ulValue->getName ( );
		ulValue->setComment (comment);
		subsection->addNamedValue (ulValue);

		double				doubleValue	=
				true == boolValue ? count++ / 3. : -(count++ / 3.);
		DoubleNamedValue*	dValue	= 
						new DoubleNamedValue ("dvalue", doubleValue);
		comment	= commentBase + dValue->getName ( );
		dValue->setComment (comment);
		subsection->addNamedValue (dValue);
		DoubleTripletNamedValue*	d3Value	= 
			new DoubleTripletNamedValue ("d3value", doubleValue, -doubleValue, doubleValue);
		comment	= commentBase + d3Value->getName ( );
		d3Value->setComment (comment);
		subsection->addNamedValue (d3Value);
		ColorNamedValue*	color	=
			new ColorNamedValue ("color", 1. / (double)(2 * count), 
					1. / (double)count, 1. / (double)(count++));
		comment	= commentBase + color->getName ( );
		color->setComment (comment);
		subsection->addNamedValue (color);

		UTF8String		sname (charset);
		sname << "svalue_" << (unsigned long)count++;
		UTF8String		stringValue (charset);
		stringValue << "STR_" << doubleValue;
		StringNamedValue*	svalue	= 	
				new StringNamedValue (sname.ascii ( ), stringValue.ascii ( ));
		comment	= commentBase + svalue->getName ( );
		svalue->setComment (comment);
		subsection->addNamedValue (svalue);
	}
}	// addLevel



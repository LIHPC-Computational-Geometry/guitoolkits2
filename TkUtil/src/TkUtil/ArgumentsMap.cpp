#include "TkUtil/ArgumentsMap.h"
#include "TkUtil/Exception.h"
#include "TkUtil/NumericConversions.h"

using namespace std;


namespace TkUtil
{

static const Charset    charset ("àéèùô");


ArgumentsMap::ArgumentsMap (bool multi)
	: _arguments ( ), _allowedArgs ( ), _multi (multi)
{
}	// ArgumentsMap::ArgumentsMap


ArgumentsMap::ArgumentsMap (int argc, const char* argv[], bool multi)
	: _arguments ( ), _allowedArgs ( ), _multi (multi)
{
	parseArgs (argc, argv);
}	// ArgumentsMap::ArgumentsMap


ArgumentsMap::ArgumentsMap (const ArgumentsMap& am)
	: _arguments (am._arguments), _allowedArgs (am._allowedArgs),
	  _multi (am._multi)
{
}	// ArgumentsMap::ArgumentsMap


ArgumentsMap& ArgumentsMap::operator = (const ArgumentsMap& am)
{
	if (&am != this)
	{
		_arguments		= am._arguments;
		_allowedArgs	= am._allowedArgs;
		_multi			= am._multi;
	}	// if (&am != this)

	return *this;
}	// ArgumentsMap::operator =


ArgumentsMap::~ArgumentsMap ( )
{
}	// ArgumentsMap::~ArgumentsMap


void ArgumentsMap::allowsArgs (const vector<string>& args)
{
	for (vector<string>::const_iterator it = args.begin ( );
	     args.end ( ) != it; it++)
		_allowedArgs.insert (*it);
}	// ArgumentsMap::allowsArgs


void ArgumentsMap::parseArgs (int argc, const char* argv[], bool raises)
{
	UTF8String	unallowed (charset);

	// 2 types d'arguments avec valeur :
	// -[-]name=value
	// -[-]name value
	for (int i = 0; i < argc; i++)
	{
		const string		arg (argv [i]);
		const string		next (i < argc - 1 ? argv [i + 1] : "");
		const string::size_type	eq	= arg.find ('=');
		bool			alone	= (0 == next.size ( )) || ('-' == next [0]) ? true : false;

		pair<string, string>	p (arg, true == alone ? string ( ) : next);
		if (string::npos != eq)
		{
			string::size_type	l	= arg.length ( ) - eq;
			p.first		= arg.substr (0, eq);
			p.second	= arg.substr (eq + 1, l);
			alone	= true;
		}	// if (string::npos != eq)

		/** Cet argument fait il partie de la liste des arguments autorisés ? */
		if (true == raises)
		{
			if (_allowedArgs.end ( ) == _allowedArgs.find (p.first))
			{
				if (true == unallowed.empty ( ))
					unallowed << "Arguments non pris en compte :";
				unallowed << " " << p.first;
			}	// if (_allowedArgs.end ( ) == _allowedArgs.find (p.first))
		}	// if (true == raises)

		if (false == _multi)
			_arguments.erase (arg);
		_arguments.insert (p);
		if (false == alone)
			i++;
	}	// for (int i = 0; i < argc; i++)

	if ((true == raises) && (false == unallowed.empty ( )))
		throw Exception (unallowed);
}	// ArgumentsMap::parseArgs


bool ArgumentsMap::hasArg (const string& arg) const
{
	return _arguments.end ( ) != _arguments.find (arg);
}	// ArgumentsMap::hasArg


string ArgumentsMap::getArgValue (const string& arg, const string def, bool raises) const
{
	const vector<string>	values	= getArgValues (arg, raises);

	if(true == values.empty ( ))
		return def;

	return values [values.size ( ) - 1];
}	// ArgumentsMap::getArgValue


vector<string> ArgumentsMap::getArgValues (const string& arg, bool raises) const
{
	// lower_bound : premier itérateur ayant pour valeur key
	// upper_bound : premier itérateur après key
	// equal_range : itérateurs ci-dessus
	vector<string>	values;

	pair<multimap<string, string>::const_iterator, multimap<string, string>::const_iterator> range	= _arguments.equal_range (arg);
	for (multimap<string, string>::const_iterator	it = range.first; range.second != it; it++)
		values.push_back ((*it).second);

	if ((true == values.empty ( )) && (true == raises))
	{
		UTF8String	msg (charset);
		msg << "Absence d'argument de nom \"" << arg << "\".";
		throw Exception (msg);
	}	// if ((true == values.empty ( )) && (true == raises))

	return values;
}	// ArgumentsMap::getArgValues


long ArgumentsMap::getArgLongValue (const string& arg, const string def, bool raises) const
{
	const string	str	= getArgValue (arg, def, raises);

	return NumericConversions::strToLong (str);
}	// ArgumentsMap::getArgLongValue


vector<long> ArgumentsMap::getArgLongValues (const string& arg, bool raises) const
{
	vector<string>	strValues	= getArgValues (arg, raises);
	vector<long>	values;
	for (vector<string>::const_iterator it = strValues.begin ( ); strValues.end ( ) != it; it++)
		values.push_back (NumericConversions::strToLong (*it));

	return values;
}	// ArgumentsMap::getArgLongValues


unsigned long ArgumentsMap::getArgULongValue (const string& arg, const string def, bool raises) const
{
	const string	str	= getArgValue (arg, def, raises);

	return NumericConversions::strToULong (str);
}	// ArgumentsMap::getArgULongValue


vector<unsigned long> ArgumentsMap::getArgULongValues (const string& arg, bool raises) const
{
	vector<string>		strValues	= getArgValues (arg, raises);
	vector<unsigned long>	values;
	for (vector<string>::const_iterator it = strValues.begin ( ); strValues.end ( ) != it; it++)
		values.push_back (NumericConversions::strToULong (*it));

	return values;
}	// ArgumentsMap::getArgULongValues


double ArgumentsMap::getArgDoubleValue (const string& arg, const string def, bool raises) const
{
	const string	str	= getArgValue (arg, def, raises);

	return NumericConversions::strToDouble (str);
}	// ArgumentsMap::getArgDoubleValue


vector<double> ArgumentsMap::getArgDoubleValues (const string& arg, bool raises) const
{
	vector<string>	strValues	= getArgValues (arg, raises);
	vector<double>	values;
	for (vector<string>::const_iterator it = strValues.begin ( ); strValues.end ( ) != it; it++)
		values.push_back (NumericConversions::strToDouble (*it));

	return values;
}	// ArgumentsMap::getArgLongValues


}	// namespace TkUtil


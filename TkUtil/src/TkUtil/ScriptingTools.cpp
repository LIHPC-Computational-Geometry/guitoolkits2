#include "TkUtil/ScriptingTools.h"

#include "TkUtil/Exception.h"
#include "TkUtil/UTF8String.h"

#include <sstream>
#include <assert.h>


USING_STD
USING_UTIL


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


string	ScriptingTools::tagDeclaration ("!!!TAG");
string	ScriptingTools::inputFileTag ("FICHIER_ENTREE");
string	ScriptingTools::outputFileTag ("FICHIER_SORTIE");
string	ScriptingTools::fileBeginingTag ("DEBUT_FICHIER");
string	ScriptingTools::endOfFileTag ("FIN_FICHIER");


ScriptingTools::ScriptingTools ( )
{
	assert (0 && "ScriptingTools constructor is not allowed.");
}	// ScriptingTools::ScriptingTools


ScriptingTools::ScriptingTools (const ScriptingTools&)
{
	assert (0 && "ScriptingTools copy constructor is not allowed.");
}	// ScriptingTools::ScriptingTools


ScriptingTools& ScriptingTools::operator = (const ScriptingTools&)
{
	assert (0 && "ScriptingTools operator = is not allowed.");	
	return *this;
}	// ScriptingTools::operator =


ScriptingTools::~ScriptingTools ( )
{
	assert (0 && "ScriptingTools constructor is not allowed.");
}	// ScriptingTools::.ScriptingTools


vector < TaggedValue > ScriptingTools::findTags (
  istream& in, const string& tagDeclaration, bool (*commentLine)(const string&))
{
	vector<TaggedValue>	tags;
	size_t				lineCount	= 0;
	char				buffer [10001];

	while ((true == in.good ( )) && (false == in.eof ( )))
	{
		lineCount++;
		in.getline (buffer, 10000);
		if ((true == in.fail ( )) && (false == in.eof ( )))
		{
			UTF8String	msg (charset);
			msg << "Erreur de lecture de la " << (unsigned long)lineCount
			    << "-ème ligne.";
			throw Exception (msg);
		}	// if ((true == in.fail ( )) && (false == in.eof ( )))
		const string	str (buffer);
		if (false == commentLine (str))
			continue;

		const size_t	pos		= str.find (tagDeclaration);
		if ((size_t)-1 != pos)
		{
			istringstream	stream (
					str.substr (pos + tagDeclaration.length ( )).c_str ( ));
			try
			{
				tags.push_back (TaggedValue (stream));
			}
			catch (...)
			{	// Certaines lignes peuvent comporter tagDeclaration sans pour
				// être pour autant une ligne de valeur balisée.
			}
/*			string	tag, value;
			stream >> tag >> value;
			if ((false == stream.fail ( )) && (false == stream.bad ( )))
				tags.push_back (pair <string, string> (tag, value));*/
		}	// if ((size_t)-1 != pos)
	}	// while ((true == in.good ( )) && (false == in.eof ( )))

	return tags;
}	// ScriptingTools::findTags


void ScriptingTools::replace (
	istream& in, ostream& os, const vector < pair < string, string > >& words)
{
	size_t	lineCount	= 0;
	char	buffer [10001];

	while ((true == in.good ( )) && (false == in.eof ( )))
	{
		lineCount++;
		in.getline (buffer, 10000);
		if ((true == in.fail ( )) && (false == in.eof ( )))
		{
			UTF8String	msg (charset);
			msg << "Erreur de lecture de la " << (unsigned long)lineCount
			    << "-ème ligne.";
			throw Exception (msg);
		}	// if ((true == in.fail ( )) && (false == in.eof ( )))

		string	str	= buffer;
		for (vector < pair < string, string > >::const_iterator
			it = words.begin ( ); words.end ( ) != it; it++)
			replace (str, (*it).first, (*it).second);

		os << str;
		if (false == in.eof ( ))
			os << endl;
		if (false == os.good ( ))	
		{
			UTF8String	msg (charset);
			msg << "Erreur d'écriture de la " << (unsigned long)lineCount
			    << "-ème ligne.";
			throw Exception (msg);
		}	// if (false == os.good ( ))
	}	// while ((true == in.good ( )) && (false == in.eof ( )))
}	// ScriptingTools::replace


void ScriptingTools::replace (
						string& str, const string& what, const string& with)
{
	if (what == with)
		return;

	UTF8String	newstr (charset);
	size_t	pos		= str.find (what);
	size_t	current	= 0;
	while ((size_t)-1 != pos)
	{
		if (pos != current)
			newstr << str.substr (current, pos - current);
		newstr << with;
		current	= pos + what.length ( );
		pos	= str.find (what, current);
	}	// while ((size_t)-1 != pos)
	if (current < str.length ( ))
		newstr << str.substr (current);

	str	= newstr;
}	// ScriptingTools::replace


void ScriptingTools::extractFile (
				istream& in, ostream& os, const string& fileName,
				const string& tagDeclaration, const string& beginingTag,
				const string& endTag, bool (*commentLine)(const IN_STD string&),
				string (*uncomment)(const string&))
{
	size_t	lineCount	= 0;
	char	buffer [10001];
	bool	copying	= false;
	while ((true == in.good ( )) && (false == in.eof ( )))
	{
		lineCount++;
		in.getline (buffer, 10000);
		if ((true == in.fail ( )) && (false == in.eof ( )))
		{
			UTF8String	msg (charset);
			msg << "Erreur de lecture de la " << (unsigned long)lineCount
			    << "-ème ligne.";
			throw Exception (msg);
		}	// if ((true == in.fail ( )) && (false == in.eof ( )))
		const string	str (buffer);
		if (false == commentLine (str))
		{
			if (true == copying)
			{
				UTF8String	message (charset);
				message << "Erreur, absence de balise de déclaration de fin du "
				        << "fichier " << fileName << ".";
				throw Exception (message);
			}	// if (true == copying)

			continue;
		}	// if (false == commentLine (str))

		// Est-ce le début ou la fin du fichier ?
		const size_t	pos		= str.find (tagDeclaration);
		if ((size_t)-1 != pos)
		{
			istringstream	stream (
					str.substr (pos + tagDeclaration.length ( )).c_str ( ));
			string	tag, value;
			stream >> tag >> value;
			if ((true == stream.fail ( )) && (true == stream.bad ( )))
			{
				if (true == copying)
					os << uncomment (str) << endl;
				continue;
			}	// if ((true == stream.fail ( )) && (true == stream.bad ( )))

			if ((beginingTag == tag) && (fileName == value))
			{
				if (true == copying)
					throw Exception (UTF8String ("Seconde balise de début de fichier détectée.", charset));
				copying	= true;
				continue;
			}	// if ((beginingTag == tag) && (fileName == value))
			if ((endTag == tag) && (true == copying))
				break;

			if (true == copying)
				os << uncomment (str) << endl;
		}	// if ((size_t)-1 != pos)
		else	// => ça n'est pas une balise
			if (true == copying)
				os << uncomment (str) << endl;
	}	// while ((true == in.good ( )) && (false == in.eof ( )))
}	// ScriptingTools::extractFile


bool ScriptingTools::pythonCommentLine (const IN_STD string& line)
{
	if ((0 == line.length ( )) || ('#' != line [0]))
		return false;

	return true;
}	// ScriptingTools::pythonCommentLine


string ScriptingTools::uncommentPythonLine (const string& line)
{
	if (line [0] != '#')
	{
		UTF8String	message (charset);
		message << "\"" << line << "\" n'est pas une ligne de commentaire de "
		        << "script python.";
		throw Exception (message);
	}	// if (line [0] != '#')

	return line.substr (1);
}	// ScriptingTools::uncommentPythonLine


END_NAMESPACE_UTIL

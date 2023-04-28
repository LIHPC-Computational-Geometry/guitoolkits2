#include "TkUtil/LoadableFileSet.h"
#include "TkUtil/UTF8String.h"
#include "TkUtil/Exception.h"

#include <iostream>

USING_STD


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


LoadableFileOption::LoadableFileOption (const string& name, const string& value)
	: pair <string, string> (name, value)
{
}	// LoadableFileOption::LoadableFileOption


LoadableFileOption::LoadableFileOption (const LoadableFileOption& opt)
	: pair <string, string> (opt.getName ( ), opt.getValue ( ))
{
}	// LoadableFileOption::LoadableFileOption (const LoadableFileOption&)


LoadableFileOption& LoadableFileOption::operator = (
											const LoadableFileOption& opt)
{
	if (&opt != this)
	{
		first	= opt.getName ( );
		second	= opt.getValue ( );
	}	// if (&opt != this)

	return *this;
}	//  LoadableFileOption::operator =


LoadableFileOption::~LoadableFileOption ( )
{
}	// LoadableFileOption::~LoadableFileOption


const string& LoadableFileOption::getName ( ) const
{
	return first;
}	// LoadableFileOption::getName


const string& LoadableFileOption::getValue ( ) const
{
	return second;
}	// LoadableFileOption::getValue


LoadableFileSet::LoadableFileSet ( )
	: _fileNames ( ), _options ( )
{
}	// LoadableFileSet::LoadableFileSet


LoadableFileSet::LoadableFileSet (const LoadableFileSet& fileset)
	: _fileNames ( ), _options ( )
{
	for (size_t f = 0; f < fileset.getFileCount ( ); f++)
		_fileNames.push_back (fileset.getFileName (f));
	for (size_t o = 0; o < fileset.getOptionsCount ( ); o++)
		_options.push_back (fileset.getOption (o));
}	// LoadableFileSet::LoadableFileSet (const LoadableFileSet&)


LoadableFileSet& LoadableFileSet::operator = (const LoadableFileSet& fileset)
{
	if (&fileset != this)
	{
		_fileNames.clear ( );
		_options.clear ( );

		for (size_t f = 0; f < fileset.getFileCount ( ); f++)
			_fileNames.push_back (fileset.getFileName (f));
		for (size_t o = 0; o < fileset.getOptionsCount ( ); o++)
			_options.push_back (fileset.getOption (o));
	}	// if (&fileset != this)

	return *this;
}	// LoadableFileSet::operator =


LoadableFileSet::~LoadableFileSet ( )
{
}	// LoadableFileSet::~LoadableFileSet


size_t LoadableFileSet::getFileCount ( ) const
{
	return _fileNames.size ( );
}	// LoadableFileSet::getFileCount


string LoadableFileSet::getFileName (size_t index) const
{
	if (index >= _fileNames.size ( ))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "LoadableFileSet::getFileName. Indice invalide ("
		         << (unsigned long)index << "). Nombre d'éléments : " 
		         << (unsigned long)_fileNames.size ( ) << ".";
		throw Exception (errorMsg);
	}	// if (index >= _fileNames.size ( ))

	return _fileNames [index];
}	// LoadableFileSet::getFileName


void LoadableFileSet::addFileName (const string& name)
{
	for (vector<string>::const_iterator it = _fileNames.begin ( );
	     _fileNames.end ( ) != it; it++)
		if (*it == name)
			return;

	_fileNames.push_back (name);
}	// LoadableFileSet::addFileName


size_t LoadableFileSet::getOptionsCount ( ) const
{
	return _options.size ( );
}	// LoadableFileSet::getOptionsCount


LoadableFileOption LoadableFileSet::getOption (size_t index) const
{
	if (index >= _options.size ( ))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "LoadableFileSet::getOption. Indice invalide ("
		         << (unsigned long)index << "). Nombre d'options : " 
		         << (unsigned long)_options.size ( ) << ".";
		throw Exception (errorMsg);
	}	// if (index >= _options.size ( ))

	return _options [index];
}	// LoadableFileSet::getOption


void LoadableFileSet::addOption (const string& name, const string& value)
{
	LoadableFileOption	option (name, value);

	for (vector<LoadableFileOption>::const_iterator it = _options.begin ( );
	     _options.end ( ) != it; it++)
//		if ((*it).getName ( ) == name)	// V 2.6.2
		if (((*it).getName ( ) == name) && ((*it).getValue ( ) == value))
			return;

	_options.push_back (option);
}	// LoadableFileSet::addOption


void LoadableFileSet::print (IN_STD ostream& stream) const
{
	stream << "Fichiers : " << getFileCount ( ) << endl;
	for (size_t f = 0; f < getFileCount ( ); f++)
		stream << getFileName (f) << endl;
	stream << "Options : " << getOptionsCount ( ) << endl;
	for (size_t o = 0; o < getOptionsCount ( ); o++)
	{
		LoadableFileOption	option	= getOption (o);
		stream << option.getName ( );
		if (0 != option.getValue ( ).length ( ))
			stream << " = " << option.getValue ( );
		stream << endl;
	}
}	// LoadableFileSet::print


vector<LoadableFileSet> LoadableFileSet::createFileSets(int argc, char* argv [])
{
	vector<LoadableFileSet>	files;

	int	i	= 0;
	while (i < argc)
	{
		// Recherche du premier fichier :
		while ((i < argc) && (argv [i][0] == '-') &&
		       (string ("-file") != argv [i]))
			i++;

		if (argc == i)
			break;
		if (string ("-file") == argv [i])
			i++;
		if (argc == i)
			break;

		// On est sur le premier fichier :
		LoadableFileSet	fileset;
		while ((i < argc) && (argv [i][0] != '-'))
			fileset.addFileName (argv [i++]);

		// On ajoute les eventuelles options :
		while ((i < argc) && (argv [i][0] == '-') &&
		       (string ("-file") != argv [i]))
		{
			if ((i < argc - 1) && (argv [i + 1][0] != '-'))
			{
				fileset.addOption (argv [i], argv [i + 1]);
				i	+= 2;
			}	// if ((i < argc - 1) && (argv [i + 1][0] != '-'))
			else
				fileset.addOption (argv [i++], "");
		}	// while ((i < argc) && (argv [i][0] == '-'))

		files.push_back (fileset);
	}	// while (i < argc)

	return files;
}	// LoadableFileSet::createFileSets


ostream& operator << (ostream& os, const LoadableFileSet& fileset)
{
	fileset.print (os);
	return os;
}	// operator << (ostream&, const LoadableFileSet&)



END_NAMESPACE_UTIL

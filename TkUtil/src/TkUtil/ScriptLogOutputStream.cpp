#include "TkUtil/ScriptLogOutputStream.h"
#include "TkUtil/ScriptingTools.h"
#include "TkUtil/InformationLog.h"
#include "TkUtil/Exception.h"
#include "TkUtil/InternalError.h"
#include "TkUtil/Date.h"
#include "TkUtil/UserData.h"
#include "TkUtil/MachineData.h"
#include "TkUtil/Process.h"
#include "TkUtil/UtilInfos.h"

#include <fstream>
#include <assert.h>


USING_STD


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");

ScriptLogOutputStream::ScriptLogOutputStream (
	const UTF8String& fileName, Log::TYPE mask, bool enableDate, bool enableTime, bool executable)
	: FileLogOutputStream (fileName, true, mask, enableDate, enableTime),
	  _shell ( ), _packagePaths ( ), _packages ( ), _headerWritten (false),
	  _tagDeclaration (ScriptingTools::tagDeclaration),
	  _inputFileTag (ScriptingTools::inputFileTag),
	  _outputFileTag (ScriptingTools::outputFileTag),
	  _fileBeginingTag (ScriptingTools::fileBeginingTag),
	  _endOfFileTag (ScriptingTools::endOfFileTag),
	  _recordInputFiles (false), _recordOutputFiles (false), _executable (executable)
{
	setScriptRights ( );
}	// ScriptLogOutputStream::ScriptLogOutputStream ( )


ScriptLogOutputStream::ScriptLogOutputStream (
	const UTF8String& fileName, const Charset& charset, Log::TYPE mask, bool enableDate, bool enableTime, bool executable)
	: FileLogOutputStream (fileName, charset, true, mask, enableDate, enableTime),
	  _shell ( ), _packagePaths ( ), _packages ( ), _headerWritten (false),
	  _tagDeclaration (ScriptingTools::tagDeclaration),
	  _inputFileTag (ScriptingTools::inputFileTag),
	  _outputFileTag (ScriptingTools::outputFileTag),
	  _fileBeginingTag (ScriptingTools::fileBeginingTag),
	  _endOfFileTag (ScriptingTools::endOfFileTag),
	  _recordInputFiles (false), _recordOutputFiles (false), _executable (executable)
{
	setScriptRights ( );
}	// ScriptLogOutputStream::ScriptLogOutputStream ( )


ScriptLogOutputStream::ScriptLogOutputStream (const ScriptLogOutputStream& stream)
	: FileLogOutputStream (UTF8String (charset)),
	  _shell ( ), _packagePaths ( ), _packages ( ), _headerWritten (false),
	  _tagDeclaration (stream._tagDeclaration),
	  _inputFileTag (stream._inputFileTag),
	  _outputFileTag (stream._outputFileTag),
	  _fileBeginingTag (ScriptingTools::fileBeginingTag),
	  _endOfFileTag (ScriptingTools::endOfFileTag),
	  _recordInputFiles (stream._recordInputFiles),
	  _recordOutputFiles (stream._recordOutputFiles),
	  _executable (stream._executable)
{
	assert (0 && "ScriptLogOutputStream copy constructor is not allowed.");
}	// ScriptLogOutputStream::ScriptLogOutputStream (const ScriptLogOutputStream&)


ScriptLogOutputStream& ScriptLogOutputStream::operator = (const ScriptLogOutputStream&)
{
	assert (0 && "ScriptLogOutputStream assignment operator is not allowed.");

	return *this;
}	// ScriptLogOutputStream::operator =


ScriptLogOutputStream::~ScriptLogOutputStream ( )
{
}	// ScriptLogOutputStream::~ScriptLogOutputStream


void ScriptLogOutputStream::log (const Log& log)
{
	LOCK_LOG_STREAM

	if (0 == (log.getType ( ) & getMask ( )))
		return;

	if (false == isEnabled ( ))
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Impossibilite d'enregistrer un log dans le fichier "
		         << getFileName ( ) << ".";

		throw Exception (errorMsg);
	}	// if (false == isEnabled ( ))

	if (Log::SCRIPTING != log.getType ( ))
		logComment (log);
	else
	{
		const ScriptingLog*	slog	= dynamic_cast<const ScriptingLog*>(&log);
		if (0 == slog)
		{
			UTF8String	message (charset);
			message << "Le log " << log.getText ( )
			        << " de type SCRIPTING n'est pas de type ScriptingLog.";
			INTERNAL_ERROR (exc, message, "ScriptLogOutputStream::log")
			throw exc;
		}	// if (0 == slog)

		addBlankLine ( );
		logInstruction (*slog);
	}	// else if (Log::SCRIPTING != log.getType ( ))
}	// ScriptLogOutputStream::log


void ScriptLogOutputStream::setFile (const UTF8String& fileName, bool)
{
	LOCK_LOG_STREAM

	FileLogOutputStream::setFile (fileName, true);
	setScriptRights ( );
	_headerWritten	= false;
	writeHeader ( );
}	// ScriptLogOutputStream::setFile


void ScriptLogOutputStream::setShell (const string& shell)
{
	_shell	= shell;
}	// ScriptLogOutputStream::setShell


vector<string> ScriptLogOutputStream::getPackages ( ) const
{
	return _packages;
}	// ScriptLogOutputStream::getPackages


void ScriptLogOutputStream::addPackage (const string& packageName)
{
	LOCK_LOG_STREAM

	if (packageName.length ( ) == 0)
	{
		UTF8String	message (charset);
		message << "Ajout d'un paquetage au script " << getFileName ( )
		        << " impossible : nom du paquetage nul.";
		throw Exception (message);
	}	// if (packageName.length ( ) == 0)

	for (vector<string>::const_iterator it = _packages.begin ( );	
	     _packages.end ( ) != it; it++)
		if (*it == packageName)
			return;

	_packages.push_back (packageName);
}	// ScriptLogOutputStream::addPackage


vector<string> ScriptLogOutputStream::getPackagePaths ( ) const
{
	return _packagePaths;
}	// ScriptLogOutputStream::getPackagePaths


void ScriptLogOutputStream::addPackagePath (const string& packagePath)
{
	LOCK_LOG_STREAM

	if (packagePath.length ( ) == 0)
	{
		UTF8String	message (charset);
		message << "Ajout au script " << getFileName ( )
		        << " d'un chemin d'accès à des paquetages impossible : "
		        << "nom du paquetage nul.";
		throw Exception (message);
	}	// if (packagePath.length ( ) == 0)

	for (vector<string>::const_iterator it = _packagePaths.begin ( );	
	     _packagePaths.end ( ) != it; it++)
		if (*it == packagePath)
			return;

	_packagePaths.push_back (packagePath);
}	// ScriptLogOutputStream::addPackagePath


void ScriptLogOutputStream::writeHeader ( )
{
	if (true == _headerWritten)
	{
		UTF8String	message (charset);
		message << "Ecriture de l'entête du script " << getFileName ( )
		        << " impossible : entête déjà écrit.";
		throw Exception (message);
	}	// if (true == _headerWritten)

	writeShellDeclaration ( );
	writeScriptComments ( );

	addBlankLine ( );
	UTF8String	w1 (charset);
	w1 << "NE PAS MODIFIER LES LIGNES COMMENCANT PAR \""
	   << getTagDeclaration ( ) << "\".";
	logComment (InformationLog (w1));
	addBlankLine ( );

	writePackagesDeclaration ( );

	_headerWritten	= true;
}	// ScriptLogOutputStream::writeHeader


void ScriptLogOutputStream::setScriptRights ( )	// v 5.11.0
{
	if (true == _executable)
	{
		try
		{
			File	file (getFileName ( ));
			if (true == file.exists ( ))
			{
				const mode_t	rights	= file.getAccessRights ( );
				if (0 == (rights & S_IXUSR))
					file.setAccessRights (rights | S_IXUSR);
			}	// if (true == file.exists ( ))
		}
		catch (const Exception& e)
		{
			ConsoleOutput::cerr ( ) << "Impossibilité de modifier les droits d'accès du script " << getFileName ( ) << " :" << co_endl
				<< e.getFullMessage ( ) << co_endl;
		}
		catch (...)
		{
			ConsoleOutput::cerr ( ) << "Impossibilité de modifier les droits d'accès du script " << getFileName ( ) << " :" << co_endl
				<< "Erreur non documentée" << co_endl;
		}
	}	// if (true == _executable)
}	// ScriptLogOutputStream::setScriptRights


void ScriptLogOutputStream::writeShellDeclaration ( )
{
	if (0 == getShell ( ).length ( ))
	{
		UTF8String	message (charset);
		message << "Ecriture de l'entête du script " << getFileName ( )
		        << " impossible : shell non déclaré.";
		throw Exception (message);
	}	// if (0 == getShell ( ).length ( ))

	UTF8String	declaration (charset);
	declaration << "#!" << getShell ( );
	write (declaration);
}	// ScriptLogOutputStream::writeShellDeclaration


void ScriptLogOutputStream::writeScriptComments ( )
{
	addBlankLine ( );

	Date			date;
	OperatingSystem	os;
	UTF8String	c1 (charset), c2 (charset), c3 (charset), c4 (charset),
				c5 (charset), c6 (charset);
	c1 << "Logiciel               : " << Process::getCurrentSoftware ( );
	logComment (InformationLog (c1));
	c2 << "Version                : "
	   << Process::getCurrentSoftwareVersion ( ).getVersion ( );
	logComment (InformationLog (c2));
	c3 << "Système d'exploitation : " 
	   << os.getName ( ) << " " << os.getVersion ( ).getVersion ( );
	logComment (InformationLog (c3));
	c4 << "Auteur                 : " << UserData ( ).getName ( );
	logComment (InformationLog (c4));
	c5 << "Date                   : "
	   << date.getDate ( ) << " " << date.getTime ( );
	logComment (InformationLog (c5));
	c6 << "Version API TkUtil     : "
	   << UtilInfos::getVersion ( ).getVersion ( );
	logComment (InformationLog (c6));
}	// ScriptLogOutputStream::writeScriptComments


void ScriptLogOutputStream::logInstruction (const ScriptingLog& log)
{
	const UTF8String	comment	= log.getComment ( );
	if (0 != comment.length ( ))
		logComment (InformationLog (comment));
	if (true == log.hasTags ( ))
		logTags (log.getTags ( ));

	UTF8String	instruction;
	if (true == log.isFormated ( ))
		instruction	= log.getText ( );
	else
		instruction	= formatInstruction (log);
	write (instruction);
}	// ScriptLogOutputStream::logInstruction


void ScriptLogOutputStream::logTags (const vector< TaggedValue >& tags)
{
	for (vector< TaggedValue >::const_iterator it = tags.begin ( );
	     tags.end ( ) != it; it++)
		logTag (*it);
}	// ScriptLogOutputStream::logTags


void ScriptLogOutputStream::logTag (const TaggedValue& taggedValue)
{
	UTF8String	line (charset);
	line << getTagDeclaration ( ) << ' ' << taggedValue.getTag ( )
	     << ' ' << taggedValue.getName ( );

	const vector<string>&	attributes	= taggedValue.getAttributes ( );
	for (vector<string>::const_iterator it = attributes.begin ( );
	     attributes.end ( ) != it; it++)
		line << ' ' << *it;
	UTF8String	instruction	= formatComment (InformationLog (line), false);
	write (instruction);

	if ((
			(true == recordInputFiles ( )) &&
			(getInputFileTag ( ) == taggedValue.getTag ( ))
		) ||
	    (
			(true == recordOutputFiles ( )) &&
			(getOutputFileTag ( ) == taggedValue.getTag ( ))
		))
		recordFileVariable (taggedValue.getName ( ));
}	// ScriptLogOutputStream::logTags


void ScriptLogOutputStream::logComment (const Log& log)
{
	UTF8String	instruction	= formatComment (log, false);
	write (instruction);
}	// ScriptLogOutputStream::logComment


void ScriptLogOutputStream::recordFileVariable (const IN_STD string& fullName)
{
	try
	{
		File	file (fullName);
		if (false == file.exists ( ))
			throw Exception (UTF8String ("Ce fichier n'existe pas.", charset));
		if (false == file.isReadable ( ))
			throw Exception (UTF8String ("Vous n'avez pas les droits en lecture sur ce fichier.", charset));

		UTF8String	fileHeader (charset);
		fileHeader << getTagDeclaration ( ) << ' ' << getFileBeginingTag ( )
		           << ' ' << fullName;
		UTF8String	instruction	=
							formatComment (InformationLog (fileHeader), true);
		write (instruction);

		ifstream	ifs (fullName.c_str ( ));
		size_t		lineCount	= 0;
		char		buffer [10001];
		if (false == ifs.good ( ))
			throw Exception (UTF8String ("Erreur lors de l'ouverture en lecture du fichier.", charset));

		while ((true == ifs.good ( )) && (false == ifs.eof ( )))
		{
			lineCount++;
			ifs.getline (buffer, 10000);
			if ((true == ifs.fail ( )) && (false == ifs.eof ( )))
			{
				UTF8String	msg (charset);
				msg << "Erreur de lecture de la " << (unsigned long)lineCount
				    << "-ème ligne.";
				throw Exception (msg);
			}	// if ((true == ifs.fail ( )) && (false == ifs.eof ( )))
			if (true == ifs.eof ( ))
				break;

			instruction	= formatComment (InformationLog (
									UTF8String (buffer, charset)), true);
			write (instruction);
		}	// while ((true == ifs.good ( )) && (false == ifs.eof ( )))

		UTF8String	fileTrailer (charset);
		fileTrailer << getTagDeclaration ( ) << ' ' << getEndOfFileTag ( )
		            << ' ' << fullName;
		instruction	= formatComment (InformationLog(fileTrailer), true);
		write (instruction);
	}
	catch (const Exception& exc)
	{

		UTF8String	msg (charset);
		msg << "Impossibilité d'enregistrer le contenu du fichier "
		    << fullName << " : " << exc.getFullMessage ( );
		throw Exception (msg);
	}
	catch (...)
	{
		UTF8String	msg (charset);
		msg << "Impossibilité d'enregistrer le contenu du fichier "
		    << fullName << " : Erreur non documentée.";
		throw Exception (msg);
	}
}	// ScriptLogOutputStream::recordFileVariable


void ScriptLogOutputStream::write (const UTF8String& str)
{
	LOCK_LOG_STREAM

	//
	// CP v 4.2.0 : ATTENTION, endl dans un flux n'est pas forcément
	// remplacé par un '\n' si endl est surchargé (erreur malheureuse) => pas
	// de sauts de ligne dans le fichier généré.
	// => On ajoute UTF8String("\n", Charset::ASCII) à la ligne à écrire et là
	// ça fonctionne bien.
	//

	static const UTF8String	lf ("\n", Charset::ASCII);
	const Charset::CHARSET	cs		= getCharset ( ).charset ( );
	const UTF8String		line	= str + lf;
	string	required;
	switch (cs)
	{
		case Charset::ISO_8859	: required	= line.iso ( );		break;
		case Charset::UTF_8		: required	= line.utf8 ( );	break;
		case Charset::ASCII		: required	= line.ascii ( );	break;
		default					: required	= line.ascii ( );
	}	// switch (cs)

	getFileStream ( ) << required << flush;
}	// ScriptLogOutputStream::write


END_NAMESPACE_UTIL


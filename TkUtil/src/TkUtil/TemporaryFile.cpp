#include "TkUtil/TemporaryFile.h"
#include "TkUtil/Exception.h"
#include "TkUtil/UTF8String.h"


USING_STD


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


TemporaryFile::TemporaryFile (
		bool asFile, const string& prefix, bool inTmpDir, bool autoRemovable)
	: File (""), _autoRemovable (false)
{
	try
	{
		const string	tmpName	= File::createTemporaryName (prefix, inTmpDir);
		setFullFileName (tmpName);
		create (asFile);
		_autoRemovable	= autoRemovable;
	}
	catch (const Exception& e)
	{
		UTF8String	message (charset);
		message << "Erreur lors de la création du fichier temporaire de prefix "
		        << prefix << " "
			    << (true == inTmpDir ?
						"dans le répertoire des fichiers temporaires" :
						"dans le répertoire courrant")
		        << " :" << '\n'
		        << e.getFullMessage ( );
		throw Exception (message);
	}
	catch (...)
	{
		UTF8String	message (charset);
		message << "Erreur non documentée lors de la création du fichier "
		        << "temporaire de prefix " << prefix << " "
			    << (true == inTmpDir ?
						"dans le répertoire des fichiers temporaires" :
						"dans le répertoire courrant") << ".";
		throw Exception (message);
	}
}	// TemporaryFile::TemporaryFile


TemporaryFile::TemporaryFile (const TemporaryFile& tmpFile)
	: File (tmpFile), _autoRemovable (false)
{
}	// TemporaryFile::TemporaryFile (const TemporaryFile&)


TemporaryFile& TemporaryFile::operator = (const TemporaryFile& tmpFile)
{
	if (this != &tmpFile)
		File::operator = (tmpFile);

	return *this;
}	// TemporaryFile::operator =


TemporaryFile::~TemporaryFile ( )
{
	if (true == isAutoRemovable ( ))
	{
		try
		{
			remove ( );
		}
		catch (const Exception& e)
		{
			UTF8String	message (charset);
			message << "Erreur lors de la suppression du fichier temporaire "
			        << getFullFileName ( ) << " :"
			        << '\n'
			        << e.getFullMessage ( );
			ConsoleOutput::cout ( ) << message << '\a' << co_endl;
		}
		catch (...)
		{
			ConsoleOutput::cout ( )
			     << "Erreur non documentée lors de la suppression du fichier "
			     << "temporaire " << getFullFileName ( ) << "." << '\a'
			     << co_endl;
		}
	}	// if (true == isAutoRemovable ( ))
}	// TemporaryFile::~TemporaryFile


void TemporaryFile::print (ostream& stream) const
{
	File::print (stream);
	stream << "Suppression automatique : "
	       << (true == isAutoRemovable ( ) ? "oui" : "non") << endl;
}	// TemporaryFile::print


ostream& operator << (ostream& os, const TemporaryFile& file)
{
	file.print (os);
	return os;
}	// operator << (ostream&, const TemporaryFile&)

END_NAMESPACE_UTIL

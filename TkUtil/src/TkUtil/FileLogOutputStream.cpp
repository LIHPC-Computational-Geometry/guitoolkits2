#include "TkUtil/FileLogOutputStream.h"
#include "TkUtil/Exception.h"

#include <assert.h>
#include <stdexcept>


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


FileLogOutputStream::FileLogOutputStream (
		const UTF8String& fileName, bool truncate, Log::TYPE mask,
		bool enableDate, bool enableTime, bool enableThreadID)
	: DefaultLogOutputStream (mask, enableDate, enableTime, enableThreadID),
	  _fileName (""), _fileStream ( )	// v 5.11.4 : on ne transmet pas fileName, sera fait via setFile
{
	setFile (fileName, truncate);
}	// FileLogOutputStream::FileLogOutputStream ( )


FileLogOutputStream::FileLogOutputStream (
		const UTF8String& fileName, const Charset& charset, bool truncate,
		Log::TYPE mask, bool enableDate, bool enableTime, bool enableThreadID)
	: DefaultLogOutputStream (
				charset, false, mask, enableDate, enableTime, enableThreadID),
	  _fileName (""), _fileStream ( )	// v 5.11.4 : on ne transmet pas fileName, sera fait via setFile
{
	setFile (fileName, truncate);
}	// FileLogOutputStream::FileLogOutputStream ( )


FileLogOutputStream::FileLogOutputStream (const FileLogOutputStream& stream)
	: DefaultLogOutputStream (stream)
{
	assert (0 && "FileLogOutputStream copy constructor is not allowed.");
}	// FileLogOutputStream::FileLogOutputStream (const FileLogOutputStream&)


FileLogOutputStream& FileLogOutputStream::operator = (const FileLogOutputStream&)
{
	assert (0 && "FileLogOutputStream assignment operator is not allowed.");

	return *this;
}	// FileLogOutputStream::operator =


FileLogOutputStream::~FileLogOutputStream ( )
{
	_fileStream.reset (0);
}	// FileLogOutputStream::~FileLogOutputStream


bool FileLogOutputStream::isEnabled ( ) const
{
	if (0 == _fileStream.get ( ))
		return false;

	return _fileStream->good ( );
}	// FileLogOutputStream::isEnabled


void FileLogOutputStream::log (const Log& log)
{
	LOCK_LOG_STREAM

	if (0 == (log.getType ( ) & getMask ( )))
		return;

	if (false == isEnabled ( ))
	{
		UTF8String	errorMsg (
					"Impossibilite d'enregistrer un log dans le fichier ");
		errorMsg	<< getFileName ( ) << ".";

//		throw IN_STD runtime_error (errorMsg.iso ( ));
		throw IN_STD runtime_error (errorMsg.utf8 ( ));	// v 5.6.1
	}	// if (false == isEnabled ( ))
//	getFileStream ( ) << format (log).format (100) << IN_STD_IO endl;

	try
	{
		getFileStream ( ) << format (log, getCharset ( )) << IN_STD_IO endl;
	}
	catch (const IN_UTIL Exception& exc)
	{
		if (true == throwExceptionIfFails ( ))
			throw;
		else
			IN_STD_IO cerr
			     << "Erreur lors de l'ecriture du message suivant dans le "
			     << "fichier " << getFileName ( ).utf8 ( ) << " :"
			     << IN_STD_IO endl << log.getText ( ).utf8 ( ) << IN_STD_IO endl
			     << exc.getFullMessage ( ) << IN_STD_IO endl;
		
	}
	catch (...)
	{
		if (true == throwExceptionIfFails ( ))
			throw;
		else
			IN_STD_IO cerr
			     << "Erreur non documentée lors de l'ecriture du message "
			     << "suivant dans le fichier " << getFileName ( ).utf8 ( )
			     << " :" << IN_STD_IO endl << log.getText ( ).utf8 ( )
			     << IN_STD_IO endl;
	}
}	// FileLogOutputStream::log


void FileLogOutputStream::setFile (const UTF8String& fileName, bool truncate)
{
	LOCK_LOG_STREAM

	if ((_fileName == fileName) && (false == truncate))
		return;

	_fileName	= fileName;
	if (0 == _fileName.length ( ))
	{
		_fileStream.reset (0);
		return;
	}	// if (0 == _fileName.length ( ))

#if defined (__osf__) && !defined (__USE_STD_IOSTREAM)
	IN_STD_IO ios::open_mode	mode	= true == truncate ? IN_STD_IO ios::trunc : IN_STD_IO ios::app;
#else
	IN_STD_IO ios_base::openmode	mode	= IN_STD_IO ios_base::out;
	if (true == truncate)
		mode	|= IN_STD_IO ios_base::trunc;
	else
		mode	|= IN_STD_IO ios_base::app;
#endif	// defined (__osf__) && !defined (__USE_STD_IOSTREAM)
	IN_STD_IO ofstream*	fileStream	= 
//					new IN_STD_IO ofstream (fileName.iso ( ).c_str ( ), mode);
					new IN_STD_IO ofstream (fileName.utf8 ( ).c_str ( ), mode);	// v 5.6.1
	_fileStream.reset (fileStream);
}	// FileLogOutputStream::setFile


void FileLogOutputStream::addBlankLine ( )
{
	LOCK_LOG_STREAM

	getFileStream ( ) << IN_STD_IO endl;
}	// FileLogOutputStream::addBlankLine


IN_STD_IO ofstream& FileLogOutputStream::getFileStream ( )
{
	if (0 == _fileStream.get ( ))
		throw Exception (UTF8String ("FileLogOutputStream::getFileStream : flux non initialisé.", charset));

	return *_fileStream.get ( );
}	// FileLogOutputStream::getFileStream


const IN_STD_IO ofstream& FileLogOutputStream::getFileStream ( ) const
{
	if (0 == _fileStream.get ( ))
		throw Exception (UTF8String ("FileLogOutputStream::getFileStream : flux non initialisé.", charset));

	return *_fileStream.get ( );
}	// FileLogOutputStream::getFileStream



END_NAMESPACE_UTIL


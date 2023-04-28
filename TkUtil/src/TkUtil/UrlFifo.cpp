#include "TkUtil/UrlFifo.h"
#include "TkUtil/Exception.h"
#include "TkUtil/File.h"

#include <fstream>
#include <stdexcept>

USING_STD
USING_UTIL


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");

UrlFifo::UrlFifo (size_t maxURLs, const IN_STD string file)
	: _capacity (maxURLs), _fifo ( )
{
	if (false == file.empty ( ))
		load (file);
}	// UrlFifo::UrlFifo


UrlFifo::UrlFifo (const UrlFifo& urlFifo)
	: _capacity (urlFifo._capacity), _fifo (urlFifo._fifo)
{
}	// UrlFifo::UrlFifo


UrlFifo& UrlFifo::operator = (const UrlFifo& urlFifo)
{
	if (&urlFifo != this)
	{
		_capacity	= urlFifo._capacity;
		_fifo		= urlFifo._fifo;
	}	// if (&urlFifo != this)

	return *this;
}	// UrlFifo::operator =


UrlFifo::~UrlFifo ( )
{
}	// UrlFifo::~UrlFifo


void UrlFifo::load (const string& fileName)
{
	_fifo.erase (_fifo.begin ( ), _fifo.end ( ));
	IN_UTIL File	file (fileName);
	UTF8String	error;
	error << "Impossibilité de lire le fichier d'URLs "
	      << file.getFullFileName ( ) << " : ";
	try
	{	
		if (false == file.exists ( ))
			throw Exception (UTF8String ("ce fichier n'existe pas", charset));
		if (true == file.isDirectory ( ))
			throw Exception (UTF8String ("ce fichier est un répertoire", charset));
		if (false == file.isReadable ( ))
			throw Exception (UTF8String ("absence de droits en lecture", charset));

		ifstream	ifs (file.getFullFileName ( ).c_str ( ));
		size_t		lineCount	= 0;
		char		buffer [10001];
		if (false == ifs.good ( ))
			throw Exception (UTF8String ("erreur lors de l'ouverture du fichier.", charset));

		while ((true == ifs.good ( )) && (false == ifs.eof ( )))
		{
			lineCount++;
			ifs.getline (buffer, 10000);
			if ((true == ifs.fail ( )) && (false == ifs.eof ( )))
			{
				UTF8String	msg (charset);
				msg << "erreur de lecture de la " << (unsigned long)lineCount
				    << "-ème ligne.";
				throw Exception (msg);
			}	// if ((true == ifs.fail ( )) && (false == ifs.eof ( )))
			string	line (buffer);
			if (line.empty ( ) == false)
				add (line, false);

			if (true == ifs.eof ( ))
				break;
		}	// while ((true == ifs.good ( )) && (false == ifs.eof ( )))
	}
	catch (const Exception& exc)
	{
		error << exc.getFullMessage ( ) << ".";
		throw Exception (error);
	}
	catch (const exception& stdexc)
	{
		error << stdexc.what ( ) << ".";
		throw Exception (error);
	}
	catch (...)
	{
		error << "erreur non documentée.";
		throw Exception (error);
	}
}	// UrlFifo::load


void UrlFifo::save (const string& fileName)
{
	IN_UTIL File	file (fileName);
	UTF8String	error (charset);
	error << "Impossibilité d'écrire le fichier d'URLs "
	      << file.getFullFileName ( ) << " : ";
	try
	{	
		if (true == file.isDirectory ( ))
			throw Exception (UTF8String ("ce fichier est un répertoire", charset));
		if (false == file.isWritable ( ))
			throw Exception (UTF8String ("absence de droits en écriture", charset));

		ofstream	ofs (file.getFullFileName ( ).c_str ( ), ios_base::trunc);
		size_t		lineCount	= 0;
		if (false == ofs.good ( ))
			throw Exception (UTF8String ("erreur lors de l'ouverture du fichier.", charset));
		for (deque<string>::const_iterator it = _fifo.begin ( );
		     _fifo.end ( ) != it; it++, lineCount++)
		{
			if (0 != lineCount)
				ofs << endl;
//				ofs << "\n";
			ofs << *it;
			if (false == ofs.good ( ))
				throw Exception (UTF8String ("erreur lors de l'écriture dans le fichier.", charset));
		}	// for (deque<string>::const_iterator it = _fifo.begin ( );
	}
	catch (const Exception& exc)
	{
		error << exc.getFullMessage ( ) << ".";
		throw Exception (error);
	}
	catch (const exception& stdexc)
	{
		error << stdexc.what ( ) << ".";
		throw Exception (error);
	}
	catch (...)
	{
		error << "erreur non documentée.";
		throw Exception (error);
	}
}	// UrlFifo::save


bool UrlFifo::has (const string& url) const
{
	if (url.empty ( ) == true)
	{
		UTF8String	message (charset);
		message << "UrlFifo::has. URL vide.";
		throw Exception (message);
	}	// if (url.empty ( ) == true)

	const IN_UTIL File	file (url);
	for (deque<string>::const_iterator it = _fifo.begin ( );
	     _fifo.end ( ) != it; it++)
	{
		const IN_UTIL File	f (*it);
		if (f.getFullFileName ( ) == file.getFullFileName ( ))
			return true;
	}	// for (deque<string>::const_iterator it = _fifo.begin ( ); ...	

	return false;
}	// UrlFifo::has


void UrlFifo::add (const string& url, bool atFront)
{
	if (url.empty ( ) == true)
	{
		UTF8String	message (charset);
		message << "UrlFifo::add. URL vide.";
		throw Exception (message);
	}	// if (url.empty ( ) == true)

	// L'URL est-elle déjà dans la FIFO ?
//	if (has (url) == true)
//		return;
	// V 2.36.0 : url sera nécessairement au début ou à la fin
	const IN_UTIL File	file (url);
	for (deque<string>::iterator it = _fifo.begin ( );
	     _fifo.end ( ) != it; it++)
	{
		const IN_UTIL File	f (*it);
		if (f.getFullFileName ( ) == file.getFullFileName ( ))
		{
			_fifo.erase (it);
			break;
		}	// if (f.getFullFileName ( ) == file.getFullFileName ( ))
	}	// for (deque<string>::iterator it = _fifo.begin ( ); ...	

	// La FIFO est-elle pleine ?
	if (_fifo.size ( ) == _capacity)
		_fifo.pop_back ( );

	if (true == atFront)
		_fifo.push_front (url);
	else
		_fifo.push_back (url);
}	// UrlFifo::add


string UrlFifo::url (size_t i) const
{
	if (i >= count ( ))
	{
		UTF8String	message (charset);
		message << "UrlFifo::url. Index demandé (" << i
		        << ") supérieur au nombre d'éléments stockés (" << count ( )
		        << ").";
		throw Exception (message);
	}	// if (i >= count ( ))

	return _fifo [i];
}	// UrlFifo::url


END_NAMESPACE_UTIL

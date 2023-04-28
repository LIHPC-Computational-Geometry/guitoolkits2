#include "TkUtil/PrintCommand.h"
#include "TkUtil/UTF8String.h"
#include "TkUtil/Exception.h"

#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <iostream>

// ATTENTION : sous SGI/g++ ces declarations doivent venir apres les 
// en-tetes c++
# if defined(__GNUC__) && defined(__sgi)
#define _POSIX_C_SOURCE 1
#endif	// if defined(__GNUC__) && defined(__sgi)
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


USING_UTIL
USING_STD

static const Charset	charset ("àéèùô");


// ===========================================================================
//                            Stucture CommandOptions
// ===========================================================================

PrintCommand::CommandOptions::CommandOptions ( )
	: eraseFile (false), copiesNumber (1)
{
}	// CommandOptions::CommandOptions


PrintCommand::CommandOptions::CommandOptions (const CommandOptions& copied)
	: eraseFile (copied.eraseFile), copiesNumber (copied.copiesNumber)
{
}	// CommandOptions::CommandOptions


PrintCommand::CommandOptions& PrintCommand::CommandOptions::operator = (
						const CommandOptions& copied)
{
	if (&copied != this)
	{
		eraseFile		= copied.eraseFile;
		copiesNumber	= copied.copiesNumber;
	}	// if (&copied != this)

	return *this;
}	// CommandOptions::CommandOptions


// ===========================================================================
//                              Classe PrintCommand
// ===========================================================================


PrintCommand::PrintCommand (const string& prn, const string& file, 
                            const PrintCommand::CommandOptions& opts)
	: Process ("lp"), _printer (prn), _file (file), _options (opts)
{
	Process::ProcessOptions&	options	= getOptions ( );
	UTF8String	copiesNumber (charset);
	copiesNumber << (unsigned long)_options.copiesNumber;

	options.addOption ("-d");
	options.addOption (_printer);
	options.addOption ("-n");
	options.addOption (copiesNumber.iso ( ));
	options.addOption (_file);
}	// PrintCommand::PrintCommand


PrintCommand::PrintCommand (const PrintCommand& copied)
	: Process (copied.getName ( ))
{
	assert (0 && "PrintCommand::PrintCommand (const PrintCommand&) is not allowed.");
}	// PrintCommand::PrintCommand (const PrintCommand&)


PrintCommand& PrintCommand::operator = (const PrintCommand&)
{
	assert (0 && "PrintCommand::operator = (const PrintCommand&) is not allowed.");
	return *this;
}	// PrintCommand::PrintCommand (const PrintCommand&)


PrintCommand::~PrintCommand ( )
{
	if (true == _options.eraseFile)
	{
		errno	= 0;
		if (0 != remove (_file.c_str ( )))
		{
			cerr << "Suppression impossible du fichier temporaire d'impression "
			     << _file.c_str ( ) << " : " << strerror (errno) << "."
			     << endl;
			errno	= 0;
		}	// if (0 != remove (_file.c_str ( )))
	}	// if (true == _options.eraseFile)
}	// PrintCommand::~PrintCommand


void PrintCommand::execute ( )
{
	try
	{
		Process::execute ( );
	}
	catch (const Exception& exc)
	{
		UTF8String	message (charset);
		message << "Il est impossible d'imprimer le fichier " << _file
		        << " sur l'imprimante " << _printer << " : "
		        << exc.getFullMessage ( );
		throw Exception (message);
	}
	catch (...)
	{
		UTF8String	message (charset);
		message << "Il est impossible d'imprimer le fichier " << _file
		        << " sur l'imprimante " << _printer << " : "
		        << "Erreur non renseignée.";
		throw Exception (message);
	}
}	// PrintCommand::execute


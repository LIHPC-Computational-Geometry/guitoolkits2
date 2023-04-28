#include "TkUtil/MemoryError.h"

USING_STD


BEGIN_NAMESPACE_UTIL


static const Charset	charset ("àéèùô");

UTF8String MemoryError::getFullMessage ( ) const
{
	char	buffer [10];
	sprintf (buffer, "%d", getLine ( ));

	UTF8String	msg (charset);
	msg	<< "Erreur d'allocation mémoire ou pointeur nul survenu en ligne "
	    << string (buffer)
	    << " du fichier "
	    << getFileName ( )
	    << " :\n"
	    << getMessage ( );

	return msg;
}	// MemoryError::getFullMessage


END_NAMESPACE_UTIL

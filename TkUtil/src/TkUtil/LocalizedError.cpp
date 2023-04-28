#include "TkUtil/LocalizedError.h"

USING_STD


BEGIN_NAMESPACE_UTIL


static const Charset	charset ("àéèùô");

UTF8String LocalizedError::getFullMessage ( ) const
{
	char	buffer [10];
	sprintf (buffer, "%d", getLine ( ));

	UTF8String	msg (charset);
	msg	<< "Erreur survenue en ligne "
	    << string (buffer)
	    << " du fichier "
	    << getFileName ( )
	    << " :\n"
	    << getMessage ( );

	return msg;
}	// LocalizedError::getFullMessage


END_NAMESPACE_UTIL

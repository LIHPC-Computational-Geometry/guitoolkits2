#include "TkUtil/InternalError.h"

USING_STD


BEGIN_NAMESPACE_UTIL


static const Charset	charset ("àéèùô");

UTF8String InternalError::getFullMessage ( ) const
{
	char	buffer [10];
	sprintf (buffer, "%d", getLine ( ));

	UTF8String	msg	(charset);
	msg << "Erreur interne survenue en ligne "
	    << IN_STD string (buffer)
	    << " du fichier "
	    << getFileName ( )
	    << " :\n"
	    << getMessage ( );
	if (0 != _detail.length ( ))
	{
		msg	+= "\nDetail :\n";
		msg	+= _detail;
	}

	return msg;
}	// InternalError::getFullMessage


END_NAMESPACE_UTIL

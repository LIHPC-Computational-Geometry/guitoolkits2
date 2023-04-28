//
// Client TCP envoyant au serveur (machine/port donnés en arguments)
// des messages et récupérant ses réponses.
//

// Exemple d'utilisation : cf. echo_srv.cpp

#include <TkUtil/NumericConversions.h>
#include <TkUtil/UnicodeString.h>

#include <iostream>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace TkUtil;
using namespace std;


int main (int argc, char* argv[], char* envp[])
{
	const string	host	= 3 != argc ? string ( ) : string (argv [1]);
	const int		port	= 3 != argc ?
						  -1 : NumericConversions::strToLong (argv [2]);
	if (0 >= port)
	{
		cerr << "Syntax : " << argv [0] << " machine port_tcp" << endl;
		return -1;
	}	// if (0 >= port)

	int					descriptor	= socket (AF_INET, SOCK_STREAM, 0);

	const struct hostent*	ipserv	= gethostbyname (host.c_str ( ));
	if ((0 == ipserv) || (0 == ipserv->h_addr_list))
	{
		cerr << "Could not locate host " << host << "." << endl;
		return -1;
	}	// if (0 == ipserv)
	struct sockaddr_in  addr;
	memset (&addr, 0, sizeof (struct sockaddr_in));
	addr.sin_family	= ipserv->h_addrtype;
	addr.sin_port	= htons (port);
	memcpy ((char*)&addr.sin_addr.s_addr, ipserv->h_addr, ipserv->h_length);

	if ((descriptor < 0) ||
	    (-1 == connect (descriptor, (const struct sockaddr*)&addr,
		                sizeof (struct sockaddr_in))))
	{
		cerr << "Could not connect to server socket on port " << port
		     << " of machine " << host << "." <<endl;
		return -1;
	}	// if ((descriptor < 0) || ...

	// On se met en mode non bloquant en écoute :
	const int	flags	= fcntl (descriptor, F_GETFL);
	const int 	res		= fcntl (descriptor, F_SETFL, flags | O_NONBLOCK);
	if (-1 == res)
	{
		cerr << "Could not read server answers in not blocking mode." << endl;
		return -1;
	}	// if (-1 == res)

	const	int	msgCount		= 10;
	int			answers			= 0;
	int			answerLength	= -1;
	ssize_t		dataRead		= 0;
	char	recBuffer [2049];
	memset (recBuffer, '\0', 2049);
	for (int i = 0; (i < msgCount) || (answers < msgCount) ; )
	{
		// A t'on encore un message à envoyer ?
		if (i < msgCount)
		{
			UnicodeString	message;
			message << "Message " << (unsigned long)i << " from client.";
			char	buffer [2049];
			memset (buffer, '\0', 2049);
			int	length	= message.length ( );
			memcpy (buffer, message.iso ( ).c_str ( ), message.length ( ));
			if ((-1 == send (descriptor, &length, sizeof (int), 0)) ||
			    (-1 == send (
					descriptor, message.iso( ).c_str( ), message.length( ), 0)))
			{
				cerr << "Failed to send message \"" << message << "\"." << endl;
				return -1;
			}	// if (-1 == send (descriptor, message.iso ( )...
			i++;
		}	// if (i < msgCount)

		// A t'on encore des réponses du serveur en attente ?
		// ==> Hyper-important, refaire selectSet et timeout à chaque appel
		// de select, sinon absence de réponse <==
		// man select
		// ...
		// On success, select() and pselect() return the number of file
		// descriptors contained in the three returned descriptor sets (that is,
		// the total number of bits that are set in readfds, writefds,
		// exceptfds) which may be zero if the timeout expires before anything
		// interesting happens.  On error, -1 is returned, and errno is set
		// appropriately; the sets and timeout become undefined, so do not
		// rely on their contents after an error.
		fd_set	selectSet;
		FD_ZERO (&selectSet);
		FD_SET (descriptor, &selectSet);
		struct timeval	timeout;
		timeout.tv_sec	= 0;
		timeout.tv_usec	= 100;
		const int	answer	= select (FD_SETSIZE, &selectSet, 0, 0, &timeout);
		if ((0 < answer) && (0 != FD_ISSET (descriptor, &selectSet)))
		{
			bool	completed	= false;
			while (false == completed)
			{
				if (answerLength < 0)
				{
					if (-1 == recv (descriptor, &answerLength, sizeof (int), 0))
					{
						answerLength	= -1;
						break;
					}	// if (-1 == recv (descriptor, &answerLength, ...
				}	// if (answerLength < 0)
//				if (-1 == recv (descriptor, buffer, answerLength, 0))
				while (dataRead < answerLength)
				{
					int	flags	= 0 == dataRead ? 0 : MSG_WAITALL;
					ssize_t	read	= recv (descriptor, recBuffer + dataRead,
												answerLength - dataRead, flags);
					if (-1 == read)
					{
						cerr << "Error during message reception." << endl;
						completed	= true;	// Pas encore arrivé
						break;
					}	// if (-1 == recv (descriptor, recBuffer, answerLength, 0))
					else
						dataRead	+= read;
				}	// while (dataRead < answerLength)
				if ((false == completed) && (0 != strlen (recBuffer)))
				{
					answerLength	= -1;
					dataRead		= 0;
					cout << "SRV ANSWER " << (++answers) << " : " << recBuffer
					     << endl;
					memset (recBuffer, '\0', 2049);
				}	// if (0 != strlen (recBuffer))
			}	// while (false == completed)
		}	// if ((0 < answer) && (0 != FD_ISSET (descriptor, &selectSet)))
	}	// for (int i = 0; i < msgCount; )

	cout << "Sending shutown message to server ..." << endl;
	const string	shutdown ("shutdown");
	int	length	= shutdown.length ( );
	send (descriptor, &length, sizeof (int), 0);
	send (descriptor, shutdown.c_str ( ), shutdown.length ( ), 0);

	// Pas beau, mais il ne faut pas que le client tombe, le serveur 
//	sleep (30);

	close (descriptor);		descriptor	= -1;

	return 0;
}	// main


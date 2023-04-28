#include "TkUtil/NetworkData.h"
#include "TkUtil/MachineData.h"
#include "TkUtil/NumericConversions.h"
#include "TkUtil/RemoteProcess.h"
#include "TkUtil/Exception.h"

#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>		// OSF : MAXHOSTNAMELEN
#include <netdb.h>			// SUN : MAXHOSTNAMELEN
#include <arpa/inet.h>		// inet_pton
#include <errno.h>

#include <memory>
#include <iostream>

#if defined(linux)
#define MAX_HOSTNAME_LENGTH		255
#else
#define MAX_HOSTNAME_LENGTH		MAXHOSTNAMELEN
#endif


USING_STD


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


NetworkData::NetworkData ( )
	: _hostName (getCurrentHostName ( )), _fullHostName ( ),
	  _inetAddresses ( ), _aliases ( )
{
	getHostInfos ( );
}	// NetworkData::NetworkData ( )


NetworkData::NetworkData (const string& hostName)
	: _hostName (hostName), _fullHostName ( ),
	  _inetAddresses ( ), _aliases ( )
{
	getHostInfos ( );
}	// NetworkData::NetworkData ( )


NetworkData::NetworkData (const NetworkData& network)
	: _hostName (network._hostName), _fullHostName (network._fullHostName),
	  _inetAddresses (network._inetAddresses), _aliases (network._aliases)
{
}	// NetworkData::NetworkData (const NetworkData&)


NetworkData& NetworkData::operator = (const NetworkData& network)
{
	if (&network != this)
	{
		_hostName		= network._hostName;
		_fullHostName	= network._fullHostName;
		_inetAddresses	= network._inetAddresses;
		_aliases		= network._aliases;
	}	// if (&network != this)

	return *this;
}	// NetworkData::operator =


NetworkData::~NetworkData ( )
{
//	delete _hostInfos;		_hostInfos	= 0;
}	// NetworkData::~NetworkData


const string NetworkData::getHostName ( ) const
{
	UTF8String	hostName (_hostName, charset);
	size_t	pos	= hostName.find ('.');
	if ((size_t)-1 != pos)
		return _hostName.substr (0, pos);

	return _hostName;
}	// NetworkData::getHostName


string NetworkData::getCurrentHostName ( )
{
	char	name [MAX_HOSTNAME_LENGTH + 1];
	errno	= 0;
	memset (name, '\0', (MAX_HOSTNAME_LENGTH + 1) * sizeof (char));
	gethostname (name, MAX_HOSTNAME_LENGTH);

	return name;
}	// NetworkData::getCurrentHostName


string NetworkData::getCurrentFullHostName ( )
{
	return getLocalHost ( ).getFullHostName ( );
}	// NetworkData::getCurrentFullHostName


string NetworkData::getFullHostName ( ) const
{
	return _fullHostName;
}	// NetworkData::getFullHostName


string NetworkData::getInetAddress ( ) const
{
	vector<string>	addresses	= getInetAddr ( );

	if (0 == addresses.size ( ))
	{
		UTF8String	error (charset);
		error << "Impossibilité d'obtenir les adressses TCP/IP de la machine "
		      << getHostName ( ) << ".";
		throw Exception (error);
	}	// if (0 == addresses.size ( ))

	if ((string ("127.0.0.1") == addresses [0]) && (1 < addresses.size ( )))
		return addresses [1];

	return addresses [0];
}	// NetworkData::getInetAddress


string NetworkData::getInetAddress (const IN_STD string& network) const
{
	vector<string>	addresses	= getInetAddr ( );

	if (0 == addresses.size ( ))
	{
		UTF8String	error (charset);
		error << "Impossibilité d'obtenir les adressses TCP/IP de la machine "
		      << getHostName ( ) << ".";
		throw Exception (error);
	}	// if (0 == addresses.size ( ))

	// On veille à ne pas avoir à comparer une adresse de 1 ou 2 caractères
	// sans 0 en tête avec une autre contenant des 0 en tête.
	const string	ns	=
		NumericConversions::toStr (NumericConversions::strToULong (network), 3);
	for (vector<string>::const_iterator it = addresses.begin ( );
	     addresses.end ( ) != it; it++)
	{
		// On extrait l'adresse du réseau :
		size_t	dot	= (*it).find ('.');
		if (((size_t)-1 == dot) || (0 == dot))
			continue;
		const string	addr	= (*it).substr (0, dot);
		const string	addr0	=
			NumericConversions::toStr (NumericConversions::strToULong(addr), 3);
		if (ns != addr0)
			continue;

		return *it;
	}	// for (vector<string>::const_iterator it = addresses.begin ( ); ...

	return "";
}	// NetworkData::getInetAddress


vector<string> NetworkData::getInetAddr ( ) const
{
	return _inetAddresses;
}	// NetworkData::getInetAddr


string NetworkData::getUsableInetAddress (const string& name)
{
	NetworkData		machine (name);
	vector<string>  addresses   = machine.getInetAddr ( );

	for (vector<string>::const_iterator it = addresses.begin ( );
	     addresses.end ( ) != it; it++)
	{
		try
		{
			NetworkData machineByIP = NetworkData::getMachineInfos (*it);

			return *it;
		}
		catch (...)
		{
			ConsoleOutput::cout ( )
			     << "NetworkData::getUsableInetAddress. Adresse IP " << *it
			     << " invalide pour accéder à la machine " << name
			     << " depuis cette machine ("
			     << getLocalHost ( ).getFullHostName ( ) << ")" << co_endl;
		}
	}   // for (vector<string>::const_iterator it = addresses.begin ( );

	UTF8String	message (charset);
	message << "La machine " << name
	        << " n'a pas d'adresse IP accessible depuis cette machine ("
	        << getLocalHost ( ).getFullHostName ( ) << ")";
	throw Exception (message);
}	// NetworkData::getUsableInetAddress


string NetworkData::getUsableInetAddress (const string& name, const string& network)
{
	NetworkData		machine (name);
	const string	ipAddr	= machine.getInetAddress (network);

	// On vérifie (autant que possible) que l'adresse TCP/IP est fonctionnelle :
	try
	{
		NetworkData machineByIP = NetworkData::getMachineInfos (ipAddr);

		return ipAddr;
	}
	catch (...)
	{
		ConsoleOutput::cout ( )
		     << "NetworkData::getUsableInetAddress. Adresse IP " << ipAddr
		     << " invalide pour accéder à la machine " << name
		     << " du réseau " << network << " depuis cette machine ("
		     << getLocalHost ( ).getFullHostName ( ) << ")" << co_endl;
	}

	UTF8String	message (charset);
	message << "La machine " << name
	        << " n'a pas d'adresse IP accessible sur le réseau " << network
	        << " depuis cette machine ("
	        << getLocalHost ( ).getFullHostName ( ) << ")";
	throw Exception (message);
}	// NetworkData::getUsableInetAddress


vector<string> NetworkData::getAliases ( ) const
{
	return _aliases;
}	// NetworkData::getAliases


string NetworkData::getNetworksIdentifications ( ) const
{
	UTF8String	infos (charset);
	infos << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << '\n'
	      << "Nom qualifié : " << getFullHostName ( ) << '\n'
	      << "Nom : " << getHostName ( ) << '\n'
	      << "Alias :";
	vector<string>	aliases	= getAliases ( );
	for (vector<string>::iterator   ita = aliases.begin ( );
	     aliases.end ( ) != ita; ita++)
		infos << ' ' << *ita;
	infos << '\n'
	      << "Adresses TCP/IP :";
	vector<string>	ipAddr	= getInetAddr ( );
	for (vector<string>::iterator   itip = ipAddr.begin ( );
	     ipAddr.end ( ) != itip; itip++)
		infos << ' ' << *itip;
	infos << '\n'
	      << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@";

//	return infos.iso ( );
	return infos.utf8 ( );	// v 5.6.1
}	// NetworkData::getNetworksIdentifications


NetworkData NetworkData::getMachineInfos (const string& hostName)
{
	try
	{
		unique_ptr<RemoteProcess>	ls (new RemoteProcess (hostName, "ls"));
		ls->execute (false);

//		NetworkData	machine (ls->getHostName ( ));
		NetworkData	machine (hostName);
		return machine;
	}
	catch (const Exception& exc)
	{
		UTF8String	error (charset);
		error << "Impossibilité d'avoir des informations sur la machine "
		      << hostName << " :" << '\n'
		      << exc.getFullMessage ( );
		throw Exception (exc);
	}
	catch (...)
	{
		UTF8String	error (charset);
		error << "Impossibilité d'avoir des informations sur la machine "
		      << hostName << " :" << '\n'
		      << "Erreur non documentée.";
		throw Exception (error);
	}
}	// NetworkData::getMachineInfos


const NetworkData& NetworkData::getLocalHost ( )
{
	return MachineData::instance ( );
}	// NetworkData::getLocalHost


bool NetworkData::is (const string& nameOrIp) const
{
	if (_hostName == nameOrIp)
		return true;
	if (_fullHostName == nameOrIp)
		return true;
	if (nameOrIp == "localhost")
		return true;
	for (vector<string>::const_iterator itip	= _inetAddresses.begin ( ); _inetAddresses.end ( ) != itip; itip++)
		if (*itip == nameOrIp)
			return true;
	for (vector<string>::const_iterator ita	= _aliases.begin ( ); _aliases.end ( ) != ita; ita++)
		if (*ita == nameOrIp)
			return true;

	return false;
}	// NetworkData::iss


void NetworkData::getHostInfos ( )
{
	_fullHostName	= "";
	_inetAddresses.clear ( );
	_aliases.clear ( );

	struct hostent	hostInfos, *result	= 0;
	char	buffer [2049];
	int	h_errnop	= 0;
	int	ret	= gethostbyname_r (
		_hostName.c_str ( ), &hostInfos, buffer, sizeof (buffer), &result,
		&h_errnop);
	if (0 != ret)
	{
		UTF8String	message (charset);
		message << "Impossibilité d'avoir des informations sur la machine "
		        << _hostName << " : " << '\n';
		switch (h_errno)
		{
			case HOST_NOT_FOUND :
				message << "Machine inconnue";
			break;
			case NO_ADDRESS     :
				message << "Absence d'adresse IP.";
			break;
			case NO_RECOVERY    :
				message << "Erreur irrémédiable du serveur de noms.";
			break;
			case TRY_AGAIN      :
				message << "Erreur temporaire du serveur de noms.";
			break;
			default             :
				message << "Erreur non documentée.";
		}   // switch (h_errno)

		throw Exception (message);
	}	// if (0 != ret)

	_fullHostName	=  hostInfos.h_name;
	char**	ptr		= 0;
	for (ptr = hostInfos.h_addr_list; *ptr != 0; ptr++)
	{	// ATTENTION : code IP 4
		struct in_addr	inetAddr;
		memcpy (&inetAddr.s_addr, *ptr, sizeof (inetAddr.s_addr));
		char	addr [INET_ADDRSTRLEN];
		inet_ntop (AF_INET, &inetAddr, addr, sizeof (addr));
		_inetAddresses.push_back (addr);
	}	// for (ptr = hostInfos.h_addr_list; *ptr != 0; ptr++)
	for (ptr = hostInfos.h_aliases; *ptr != 0; ptr++)
		_inetAddresses.push_back (*ptr);
}	// NetworkData::getHostInfos


bool NetworkData::isValidInetAddress (const string& address)	// v 5.10.0
{
	int	olderrno	= errno;

	unsigned char buf[sizeof (struct in6_addr)];
	errno	= 0;

	int	result	= inet_pton (AF_INET, address.c_str ( ), buf);

	errno	= olderrno;	// Rem : ne pas se fier à errno, positionné que dans certains cas.
	
	return 1 == result ? true : false;
}	// NetworkData::isValidInetAddress


END_NAMESPACE_UTIL

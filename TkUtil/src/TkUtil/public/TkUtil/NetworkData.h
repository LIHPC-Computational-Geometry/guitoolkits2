#ifndef NETWORK_DATA_H
#define NETWORK_DATA_H

#include <TkUtil/util_config.h>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>


struct hostent;


BEGIN_NAMESPACE_UTIL


/**
 * Cette classe permet d'accéder à des informations relatives au réseau.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 * @warning		Certaines méthodes nécessitent que la classe <I>Process</I> et
 *				<I>RemoteProcess</I> aient été initialisées.
 */
class NetworkData
{
	public :

	/**
	 * Constructeur par défaut. 
	 * La machine est celle où tourne le process courrant.
	 */
	NetworkData ( );

	/**
	 * Constructeur "nommé".
	 * @param		Nom ou adresse TCP/IP de la machine représentée.
	 */
	NetworkData (const IN_STD string& hostName);

	/**
	 * Constructeurs de copie et opérateur = : RAS.
	 */
	NetworkData (const NetworkData&);
	NetworkData& operator = (const NetworkData&);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~NetworkData ( );

	/**
	 * @return		Le nom de la machine (hors domaine).
	 * @see			getFullHostName
	 */
	virtual const IN_STD string getHostName ( ) const;

	/**
	 * @return		Le nom de la machine où tourne le process courrant.
	 */
	static IN_STD string getCurrentHostName ( );

	/**
	 * @return		Le nom complet de la machine où tourne le process courrant,
	 *				domaine compris.
	 */
	static IN_STD string getCurrentFullHostName ( );

	/**
	 * @return		Le nom complet de la machine, domaine compris
	 * @exception	Une exception est levée en cas d'erreur.
	 * @see			getHostName
	 */
	virtual IN_STD string getFullHostName ( ) const;

	/**
	 * @return		La première adresse TCP/IP de la machine.
	 * @exception	Une exception est levée en cas d'erreur.
	 * @see			getInetAddr
	 * @warning		Si 127.0.0.1 est la première adresse TCP/IP de la machine,
	 *				retourne, s'il y en a une, la seconde adresse TCP/IP.
	 */
	virtual IN_STD string getInetAddress ( ) const;

	/**
	 * @return		La première adresse TCP/IP de la machine appartenant au
	 *				réseau network (8 premiers bits de l'adresse TCP/IP, par
	 *				exemple 162 pour 162.0.0.43). Une adresse vide est
	 *				retournée si la machine n'a pas d'adresse TCP/IP sur le
	 *				réseau demandé.
	 * @param		Adresse du réseau dans lequel on recherche l'adresse TCP/IP
	 *				de la machine (162 dans l'exemple ci-dessus).
	 * @exception	Une exception est levée en cas d'erreur.
	 * @see			getInetAddr
	 */
	virtual IN_STD string getInetAddress (const IN_STD string& network) const;

	/**
	 * @return		La liste des adresses TCP/IP de la machine.
	 * @exception	Une exception est levée en cas d'erreur.
	 * @see			getInetAddress
	 */
	virtual IN_STD vector<IN_STD string> getInetAddr ( ) const;

	/**
	 * @param		Nom de machine évalué (idéalement nom qualifié pour
	 *				un calculateur).
	 * @return		Adresse IP valide pour accéder à cette machine depuis
	 *				la machine locale (certaines machines ont plusieures
	 *				adresse IP sur des réseaux différents).
	 */
	static IN_STD string getUsableInetAddress (const IN_STD string& name);

	/**
	 * @param		Nom de machine évalué (idéalement nom qualifié pour
	 *				un calculateur).
	 * @param		Réseau visé (on souhaite en retour une adresse TCP/IP sur
	 *				ce réseau, pour les machines ayant plusieurs cartes
	 *				réseau).
	 * @return		Adresse IP valide pour accéder à cette machine depuis
	 *				la machine locale (certaines machines ont plusieures
	 *				adresse IP sur des réseaux différents). Une adresse vide est
	 *				retournée si la machine n'a pas d'adresse TCP/IP sur le
	 *				réseau demandé.
	 * @see			getInetAddr
	 */
	static IN_STD string getUsableInetAddress (
					const IN_STD string& name, const IN_STD string& network);

	/**
	 * @return		La liste des alias de la machine.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	virtual IN_STD vector<IN_STD string> getAliases ( ) const;

	/**
	 * Fonction visant à offrir un moyen générique d'identification d'une
	 * machine où tourne un serveur.
	 * Cette fonction retourne une chaine de caractère de format suivant :
	 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	 * Nom qualifié : nom_machine.domaine
	 * Nom : nom_machine
	 * Alias : alias_1 alias_2 ...
	 * Adresses TCP/IP : adresse_1 adresse_2 ...
	 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	 * @return		Les informations réseau caractérisant la machine au format
	 * 				décrit ci-dessus.
	 */
	virtual IN_STD string getNetworksIdentifications ( ) const;

	/**
	 * @param		Nom d'une machine distance
	 * @return		Une instance représentant une machine distante avec des
	 *				informations réalistes vue de la machine locale. Utile
	 *				lorsqu'il s'agit d'un noeud d'un calculateur : cette
	 *				fonction retourne alors un nom et une adresse TCP/IP
	 *				utilisables par le processus local.
	 * @exception	Une exception est levée en cas d'erreur.
	 * @warning		Nécessite que <I>Process::initialize</I> ait été
	 *				préalablement appelé.
	 * @see			Process
	 */
	static NetworkData getMachineInfos (const IN_STD string& hostName);

	/**
	 * @return		La machine locale.
	 */
	static const NetworkData& getLocalHost ( );

	/**
	 * @return		<I>true</I> si la machine dont le nom ou l'adresse TCP/IP est transmise en argument est la
	 * 				machine locale, <I>false</I> dans le cas contraire.
	 */
	bool is (const IN_STD string& nameOrIp) const;

	/**
	 * @return		<I>true</I> si la chaine de caractères transmise en argument est une adresse TCP/IP valide,
	 * 				<I>false</I> dans le cas contraire.
	 */
	static bool isValidInetAddress (const IN_STD string& address);	// v 5.10.0
	 

	protected :

	/**
	 * Récupère les différentes infos réseau de la machine.
	 */
	virtual void getHostInfos ( );


	private :


	/** Le nom de la machine. */
	IN_STD string						_hostName;

	/** Le nom complet de la machine. */
	IN_STD string						_fullHostName;

	/** Les adresses IP de la machine. */
	IN_STD vector<IN_STD string>		_inetAddresses;

	/** Les alias de la machine. */
	IN_STD vector<IN_STD string>		_aliases;
};	// class NetworkData



END_NAMESPACE_UTIL


#endif	// NETWORK_DATA_H

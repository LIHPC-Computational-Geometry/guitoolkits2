#ifndef REMOTE_PROCESS_H
#define REMOTE_PROCESS_H

#include <TkUtil/Process.h>


BEGIN_NAMESPACE_UTIL


/** <P>
 * Classe de lancement de processus fils sur une autre machine du réseau. Elle
 * permet de vérifier que le processus fils a bien été lancé et d'accéder à son
 * pid.
 * </P>
 * <P>
 * Cette classe utilise à cet effet sur la machine distante un "<B>lanceur</B>"
 * qui récupère le nom de machine distante exact, le pid et le path complet du 
 * processus fils.  Ce lanceur doit écrire, en tant que processus fils, et en 
 * première ligne sur la sortie standard, son pid et le chemin d'accès complet
 * à l'exécutable à lancer. Dans le cas contraire cette classe considère qu'il
 * y a eu une erreur au lancement du processus fils.
 * </P>
 * <P>La commande d'exécution d'un shell distant, par défaut <I>ssh</I>,
 * est modifiable.
 * </P>
 * <P>Voir la documentation de la classe parente <B>Process</B>.</P>
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 * @warning		Attention, cette classe positionne une action sur SIGCHLD.
 */
class RemoteProcess : public Process
{
	public :

	/** 
	 * Constructeur.
	 * @param		machine où doit être exécuté le processus fils
	 * @param		Nom de l'exécutable
	 * @warning		Il est indispensable d'utiliser l'opérateur new. 
	 */
	RemoteProcess (const IN_STD string& host, const IN_STD string& processName);

	/** 
	 * Constructeur.
	 * @param		machine où doit être exécuté le processus fils
	 * @param		Nom de l'exécutable
	 * @param		listes d'options à transmettre à <I>remoteShell</I>
	 *				(par exemple -L ou -R pour <I>ssh</I>).
	 * @warning		Il est indispensable d'utiliser l'opérateur new. 
	 */
	RemoteProcess (const IN_STD string& host, const IN_STD string& processName,
	               const IN_STD vector<IN_STD string>& rshOptions);

	/**
	 * Destructeur : appelé par l'instance en fin d'exécution si wait n'est pas
	 * invoqué, ou à appeler en sortie de wait.
	 * @see			wait
	 */
	virtual ~RemoteProcess ( );

	/**
	 * @return		L'exécutable du processus, chemin d'accès complet compris.
	 * @exception	Une exception est levée si l'exécutable est inaccessible.
	 * @see			getRemotePid
	 */
	virtual IN_STD string getRemotePath ( );

	/**
	 * @return		Le PID du processus fils sur la machine distante. getPid
	 *				retourne le PID du processus servant à lancer le processus
	 *				sur la machine distante.
	 * @see			getRemotePath
	 * @see			getPid
	 */
	virtual pid_t getRemotePid ( ) const
	{ return _remotePid; }

	/**
	 * Exécute le process avec ses options.
	 * @param			true si l'instance doit se détruire automatiquement
	 *					à la fin du processus fils, sinon false.
	 * @exception		Une exception de type Exception est levée en cas 
	 *					d'erreur.
	 * @see				setServerWaitingMessage
	 */
	virtual void execute (bool autoDelete = true);

	/**
	 * Attend la fin du processus fils distant.
	 * @return			status de retour du processus fils.
	 * @see				getCompletionCode
	 * @see				getErrorMessage
	 */
	virtual int wait ( );

	/**
	 * Tue le processus fils.
	 */
	virtual void kill ( );

	/**
	 * @return		La machine où est executé le processus fils.
	 */
	virtual const IN_STD string& getHostName ( ) const
	{ return _host; }

	/**
	 * @return		Le processus fils à exécuter.
	 */
	virtual const IN_STD string& getRemoteProcess ( ) const
	{ return _remoteProcess; }

	/**
	 * @return		La chaine de caractère indiquant que le processus fils est
	 *				un serveur et attend maintenant des requêtes.
	 * @see			setServerWaitingMessage
	 * @see			execute
	 */
	virtual const IN_STD string& getServerWaitingMessage ( ) const
	{ return _serverWaitingMessage; }

	/**
	 * @param		La chaine de caractère indiquant que le processus fils est
	 *				un serveur et attend maintenant des requêtes. Si cette
	 *				chaine est non nulle, la méthode execute attend que le
	 *				processus fils l'ait écrit avant de retourner.
	 * @see			execute
	 * @see			getServerWaitingMessage
	 */
	virtual void setServerWaitingMessage (const IN_STD string& waitingMsg);

	/**
	 * La commande d'exécution d'un shell distant (<I>ssh</I> par défaut).
	 */
	static IN_STD string	remoteShell;

	/**
	 * Le chemin d'accès (complet) l'exécutable de lancement des processus sur
	 * les machines du réseau.
	 */
	static IN_STD string	launcher;


	protected :

	/**
	 * Affecte le chemin d'accès complet à l'exécutable.
	 */
	virtual void setRemotePath (const IN_STD string& path);


	private :

	/** Constructeur de copie et operateur = : interdits. */
	RemoteProcess (const RemoteProcess&);
	RemoteProcess& operator = (const RemoteProcess&);

	/** La machine où est exécuté le processus fils. */
	IN_STD string			_host;

	/** Le processus fils à exécuter. */
	IN_STD string			_remoteProcess;

	/** Le chemin d'accès complet à l'exécutable. */
	IN_STD string			_remotePath;

	/** Le PID du processus fils lancé sur la machine distante. */
	pid_t					_remotePid;

	/** Le message du serveur (processus fils) indiquant qu'il est maintenant
	 * disponible pour traiter des requêtes.
	 */
	IN_STD string			_serverWaitingMessage;
};	// class RemoteProcess


END_NAMESPACE_UTIL


#endif	// REMOTE_PROCESS_H

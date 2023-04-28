#ifndef USER_DATA_H
#define USER_DATA_H

#include <TkUtil/util_config.h>

#include <unistd.h>	// uid_t
#include <string>
#include <ostream>


BEGIN_NAMESPACE_UTIL


/**
 * Cette classe permet d'accéder à des informations propres à un utilisateur du
 * système.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class UserData
{
	public :

	/**
	 * Constructeur par défaut. 
	 * L'utilisateur est celui qui est logué si real vaut true, sinon c'est
	 * celui qui a lancé le process.
	 */
	UserData (bool real = true);

	/**
	 * Constructeur "nommé". 
	 * @param		Nom de l'utilisateur.
	 */
	UserData (const IN_STD string& name);

	/**
	 * Constructeurs de copie et opérateur = : RAS.
	 */
	UserData (const UserData&);
	UserData& operator = (const UserData&);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~UserData ( );

	/**
	 * Affiche les informations sur l'utilisateur dans le flux reçu en 
	 * argument.
	 */
	virtual void print (IN_STD ostream& stream) const;

	/**
	 * @return		L'uid de l'utilisateur
	 */
	virtual uid_t getUid ( ) const
	{ return _userId; }

	/**
	 * @return		L'identifiant de groupe de l'utilisateur
	 */
	virtual gid_t getGid ( ) const
	{ return _groupId; }

	/**
	 * @return		Le nom de l'utilisateur
	 */
	virtual const IN_STD string& getName ( ) const
	{ return _name; }

	/**
	 * @return		Le nom du groupe de de l'utilisateur
	 */
	virtual const IN_STD string& getGroupName ( ) const
	{ return _groupName; }

	/**
	 * @return		Le répertoire "home" de l'utilisateur
	 */
	virtual const IN_STD string& getHome ( ) const
	{ return _home; }

	/**
	 * @return		Le "shell" de l'utilisateur
	 */
	virtual const IN_STD string& getShell ( ) const
	{ return _shell; }


	private :


	/** Le user id de l'utilisateur. */
	uid_t								_userId;

	/** L'identifiant du groupe de l'utilisateur. */
	gid_t								_groupId;

	/** Le nom de l'utilisateur. */
	IN_STD string						_name;

	/** Le nom du groupe de l'utilisateur. */
	IN_STD string						_groupName;

	/** Le répertoire "home" de l'utilisateur. */
	IN_STD string						_home;

	/** Le shell utilisé par l'utilisateur. */
	IN_STD string						_shell;
};	// class UserData


IN_STD ostream& operator << (IN_STD ostream& os, const UserData& user);


END_NAMESPACE_UTIL


#endif	// USER_DATA_H

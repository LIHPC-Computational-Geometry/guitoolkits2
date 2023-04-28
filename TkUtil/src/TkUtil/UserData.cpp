#include "TkUtil/UserData.h"

#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>


USING_STD


BEGIN_NAMESPACE_UTIL


UserData::UserData (bool real)
	: _userId (-1), _groupId (-1),
	  _name ("Invalid name"), _groupName ("Invalid group name"),
	  _home ("Invalid home"), _shell ("Invalid shell")
{
	_userId	= true == real ? getuid ( ) : geteuid ( );

	const struct passwd*	pwd	= getpwuid (_userId);
	if (0 != pwd)
	{
		_name		= pwd->pw_name;
		_groupId	= pwd->pw_gid;
		_home		= pwd->pw_dir;
		_shell		= pwd->pw_shell;
	}	// if (0 != pwd)

	const struct group*		group	= getgrgid (_groupId);
	if (0 != group)
		_groupName	= group->gr_name;
}	// UserData::UserData ( )


UserData::UserData (const string& name)
	: _userId (-1), _groupId (-1),
	  _name (name), _groupName ("Invalid group name"),
	  _home ("Invalid home"), _shell ("Invalid shell")
{
	const struct passwd*	pwd	= getpwnam (_name.c_str ( ));
	if (0 != pwd)
	{
		_userId		= pwd->pw_uid;
		_groupId	= pwd->pw_gid;
		_home		= pwd->pw_dir;
		_shell		= pwd->pw_shell;
	}	// if (0 != pwd)

	const struct group*		group	= getgrgid (_groupId);
	if (0 != group)
		_groupName	= group->gr_name;
}	// UserData::UserData ( )


UserData::UserData (const UserData& user)
	: _userId (user.getUid ( )), _groupId (user.getGid ( )),
	  _name (user.getName ( )), _groupName (user.getGroupName ( )),
	  _home (user.getHome ( )), _shell (user.getShell ( ))
{
}	// UserData::UserData (const UserData&)


UserData& UserData::operator = (const UserData& user)
{
	if (&user != this)
	{
		_userId		= user.getUid ( );
		_groupId	= user.getGid ( );
		_name		= user.getName ( );
		_groupName	= user.getGroupName ( );
		_home		= user.getHome ( );
		_shell		= user.getShell ( );
	}	// if (&user != this)

	return *this;
}	// UserData::operator =


UserData::~UserData ( )
{
}	// UserData::~UserData


void UserData::print (ostream& stream) const
{
	stream << "Utilisateur           : " << getName ( ) << endl
	       << "Identifiant           : " << getUid ( ) << endl
	       << "Groupe                : " << getGroupName ( ) << endl
	       << "Identifiant de groupe : " << getGid ( ) << endl
	       << "Home                  : " << getHome ( ) << endl
	       << "Shell                 : " << getShell ( ) << endl;
}	// UserData::print


ostream& operator << (ostream& os, const UserData& user)
{
	user.print (os);
	return os;
}	// operator << (ostream& os, const UserData& user)



END_NAMESPACE_UTIL

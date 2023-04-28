#ifndef FILE_REDIRECTION_H
#define FILE_REDIRECTION_H


#include <TkUtil/util_config.h>

#include <string>


BEGIN_NAMESPACE_UTIL


/**
 * Classe permettant de rediriger un fichier dans un autre, de manière
 * temporaire ou définitive. Elle peut être utilisée, par exemple, pour
 * rediriger les flux standards.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class FileRedirection
{
	public :

	/**
	 * Constructeur.
	 * @param		Descripteur du fichier à rediriger (ex : stderr)
	 * @param		Nom du fichier de redirection
	 * @param		true si la redirection est définitive, false si elle ne
	 *				dure que la durée de vie de cette instance.
	 * @param		true si le fichier de redirection doit être détruit à la
	 *				fin de vie de cette instance, sinon false.
	 * @exception	Une exception est levée en cas d'échec
	 */
	FileRedirection (FILE* fileDesc, const IN_STD string& fileName,
	                 bool always, bool toRemove = false);

	/**
	 * Rétablit éventuellement la situation initiale selon le troisième argument
	 * donné au constructeur.
	 */
	virtual ~FileRedirection ( );

	/**
	 * Met fin à la redirection, sous réserve que le troisème argument du
	 * constructeur soit true.
	 */
	virtual void terminate ( );

	/**
	 * @return		Une chaine de caractères contenant le texte redirigé.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	virtual IN_STD string getText ( ) const;


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	FileRedirection (const FileRedirection&);
	FileRedirection& operator = (const FileRedirection&);

	/** L'éventuel descripteur du fichier redirigé. */	
	FILE*				_fileDesc;

	/** La copie de sauvegarde du escripteur redirigé, à des fins de 
	 * restauration à l'état initial à la destruction de cette instance. */
	int					_backupFileDesc;

	/** Le nom du fichier utilisé pour la redirection. */
	IN_STD string		_fileName;

	/** Le nom du fichier temporaire utilisé pour sauvegarder le flux initial.
	 */
	IN_STD string		_backupFileName;

	/** Faut-il détruire le fichier de redirection. */
	bool				_removeAtCompletion;
};	// class FileRedirection

END_NAMESPACE_UTIL

#endif	// FILE_REDIRECTION_H

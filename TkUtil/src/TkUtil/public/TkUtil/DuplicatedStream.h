#ifndef DUPLICATED_STREAM_H
#define DUPLICATED_STREAM_H


#include <TkUtil/util_config.h>

#include <string>


BEGIN_NAMESPACE_UTIL


/**
 * <P>
 * Classe permettant de rediriger un flux dans un autre et d'avoir une copie
 * de ce flux. Elle peut être utilisée, par exemple, pour avoir une copie d'un
 * flux standard comme stdout.
 * </P>
 * @warning		ATTENTION, jamais testé avec des flux en lecture.
 * @warning		ATTENTION, il arrive que la redirection dans le flux d'origine
 *				ne se produise pas. Pb de synchro des flux ???
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class DuplicatedStream
{
	public :

	/**
	 * Constructeur.
	 * @param		Descripteur du fichier à rediriger (ex : stderr)
	 * @param		Nom du fichier de redirection
	 * @exception	Une exception est levée en cas d'échec
	 */
	DuplicatedStream (FILE* fileDesc, const IN_STD string& fileName);

	/**
	 */
	virtual ~DuplicatedStream ( );

	/**
	 * Met fin à la duplication.
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
	DuplicatedStream (const DuplicatedStream&);
	DuplicatedStream& operator = (const DuplicatedStream&);

	/** L'éventuel descripteur du fichier copié. */	
	FILE*				_fileDesc;

	/** La copie de sauvegarde du descripteur copié, à des fins de 
	 * restauration à l'état initial à la destruction de cette instance. */
	int					_backupFileDesc;

	/** Le nom du fichier utilisé pour la redirection. */
	IN_STD string		_fileName;

	/** Le nom du fichier temporaire utilisé pour sauvegarder le flux initial.
	 */
	IN_STD string		_backupFileName;

	pid_t				_childPid;
};	// class DuplicatedStream

END_NAMESPACE_UTIL

#endif	// DUPLICATED_STREAM_H

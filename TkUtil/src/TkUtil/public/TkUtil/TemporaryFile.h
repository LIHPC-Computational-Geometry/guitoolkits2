#ifndef TEMPORARY_FILE_H
#define TEMPORARY_FILE_H

#include <TkUtil/File.h>


BEGIN_NAMESPACE_UTIL


/**
 * Cette classe permet de gérer, au niveau système, des fichiers temporaires.
 * Elle permet, par exemple, la destruction du fichier lorsque l'instance de
 * cette classe est elle-même détruite.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class TemporaryFile : public File
{
	public :

	/**
	 * Constructeur par défaut. Créé le fichier temporaire.
	 * @param		true s'il s'agit d'un fichier, false s'il s'agit d'un
	 *				répertoire.
	 * @param		Préfix du nom du fichier créé.
	 * @param		true si le fichier doit être créé dans le répertoire des
	 *				fichiers temporaires, false si il doit être créé en relatif
	 *				par rapport à prefix.
	 * @param		true si le fichier doit être détruit avec cette instance,
	 *				sinon false.
	 * @see			isAutoRemovable
	 * @see			setAutoRemovable
	 */
	TemporaryFile (bool asFile, const IN_STD string& prefix, bool inTmpDir,
	               bool autoDelete);

	/**
	 * Constructeurs de copie et opérateur =.
	 * @see			isAutoRemovable
	 * @see			setAutoRemovable
	 * @warning		Le caractère d'autodestruction n'est pas transmis.
	 */
	TemporaryFile (const TemporaryFile&);
	TemporaryFile& operator = (const TemporaryFile&);

	/**
	 * <P>Destructeur.
	 * Détruit le fichier si le caractère d'auto-destruction lui a
	 * été conféré.
	 * </P>
	 * <P>En cas d'erreur un message est affiché sur la sortie standard.
	 * </P>
	 * @see			isAutoRemovable
	 * @see			setAutoRemovable
	 */
	virtual ~TemporaryFile ( );

	/**
	 * Affiche les informations sur le fichier dans le flux reçu en
	 * argument.
	 */
	virtual void print (IN_STD ostream& stream) const;

	/**
	 * @return		true si le fichier sera détruit à la destruction de cette
	 *				instance, false dans le cas contraire.
	 */
	virtual bool isAutoRemovable ( ) const
	{ return _autoRemovable; }

	/**
	 * @param		Nouveau caractère d'auto-destruction de l'instance.
	 */
	virtual void setAutoRemovable (bool autoRemovable)
	{ _autoRemovable	= autoRemovable; }


	private :


	/** Le caractère d'autodestruction du fichier. */
	bool						_autoRemovable;
};	// class TemporaryFile


IN_STD ostream& operator << (IN_STD ostream& os, const TemporaryFile& file);


END_NAMESPACE_UTIL


#endif	// TEMPORARY_FILE_H

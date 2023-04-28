#ifndef FILE_LOG_OUTPUT_STREAM_H
#define FILE_LOG_OUTPUT_STREAM_H

#include <TkUtil/util_config.h>
#include <TkUtil/DefaultLogOutputStream.h>
#include <string>

#include <memory>

#include <fstream>


BEGIN_NAMESPACE_UTIL


/**
 * <P>
 * Cette classe représente un flux sortant de logs dirigé dans
 * un fichier ASCII.
 * </P>
 * <P>Les écritures se font avec le jeu de caractères demandé, mais aucune
 * action de plus n'est effectuée (type <I>BOM</I> ou autre).
 * </P>
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class FileLogOutputStream : public DefaultLogOutputStream
{
	public :

	/**
	 * Constructeur. Invoque setFile et est de ce fait susceptible de lever
	 * une exception.
	 * @param		Nom du fichier utilisé pour écrire les logs.
	 * @param		si true, le fichier sera préalablement effacé s'il
	 *				existe, alors que dans le cas contraire les messages reçus
	 *				seront écrits à la fin.
	 * @param		Masque appliqué.
	 * @param		true si les dates doivent être affichées
	 * @param		true si les heures doivent être affichées
	 * @param		true si les IDs de thread doivent être affichés
	 * @see			setFile
	 */
	FileLogOutputStream (
		const UTF8String& fileName, bool truncate = false,
        Log::TYPE mask = Log::PRODUCTION, bool enableDate = true,
		bool enableTime = true, bool enableThreadID = true);

	/**
	 * Constructeur. Invoque setFile et est de ce fait susceptible de lever
	 * une exception.
	 * @param		Nom du fichier utilisé pour écrire les logs.
	 * @param		si true, le fichier sera préalablement effacé s'il
	 *				existe, alors que dans le cas contraire les messages reçus
	 *				seront écrits à la fin.
	 * @param		Jeu de caractères à utiliser lors des écritures
	 * @param		Masque appliqué.
	 * @param		true si les dates doivent être affichées
	 * @param		true si les heures doivent être affichées
	 * @param		true si les IDs de thread doivent être affichés
	 * @see			setFile
	 */
	FileLogOutputStream (	// v 4.0.0
		const UTF8String& fileName, const IN_UTIL Charset& charset,
		bool truncate = false,
        Log::TYPE mask = Log::PRODUCTION, bool enableDate = true,
		bool enableTime = true, bool enableThreadID = true);

	/**
	 * Destructeur. Ferme le fichier.
	 */
	virtual ~FileLogOutputStream ( );

	/**
	 * @return 		true si le flux est fonctionnel (fichier ouvert et en
	 *				état de fonctionnement. Retourne false dans le cas
	 *				contraire.
	 */
	virtual bool isEnabled ( ) const;

	/**
	 * Ecrit le log reçu en argument s'il est compatible avec le masque en
	 * cours.
	 * @exception		Méthode susceptible de lever une exception.
	 */
	virtual void log (const Log& log);

	 /* Change le fichier d'écriture des logs.
	 * @param		Nom du fichier utilisé pour écrire les logs.
	 * @param		si true, le fichier sera préalablement effacé s'il
	 *				existe, alors que dans le cas contraire les messages reçus
	 *				seront écrits à la fin.
	 * @exception	Une exception est levée en cas d'erreur
	 */
	virtual void setFile (const UTF8String& fileName, bool truncate);

	/**
	 * @return		Le nom du fichier utilisé
	 */
	virtual const UTF8String& getFileName ( ) const
	{ return _fileName; }

	/**
	 * Ajoute une ligne blanche au fichier.
	 * @exception	Une exception est levée en cas d'erreur
	 */
	virtual void addBlankLine ( );


	protected :

	/**
	 * @return		Le flux utilisé pour écrire les logs.
	 * @exception	Une exception est levée en cas de flux non initialisé.
	 */
	virtual IN_STD_IO ofstream& getFileStream ( );
	virtual const IN_STD_IO ofstream& getFileStream ( ) const;


	private :

	/**
	 * Constructeurs de copie et opérateur = : interdits.
	 */
	FileLogOutputStream (const FileLogOutputStream&);
	FileLogOutputStream& operator = (const FileLogOutputStream&);


	/** Le nom du fichier en cours. */
	UTF8String									_fileName;

	/** Le flux en cours. */
	IN_STD unique_ptr <IN_STD_IO ofstream>		_fileStream;
};	// class LogOutputStream


END_NAMESPACE_UTIL


#endif	// FILE_LOG_OUTPUT_STREAM_H

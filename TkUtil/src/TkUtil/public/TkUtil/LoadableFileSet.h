#ifndef LOADABLE_FILE_SET_H
#define LOADABLE_FILE_SET_H

#include <TkUtil/util_config.h>
#include <string>
#include <ostream>
#include <utility>
#include <vector>


BEGIN_NAMESPACE_UTIL


/**
 * Cette classe gère une liste d'options à appliquer à des fichiers.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class LoadableFileOption : protected IN_STD pair<IN_STD string, IN_STD string>
{
	public :

	/**
	 * Constructeur.
	 * @param		Nom de l'option
	 * @param		Valeur de l'option
	 */
	LoadableFileOption (const IN_STD string& name, const IN_STD string& value);

	/**
	 * Constructeur de copie. RAS.
	 */
	LoadableFileOption (const LoadableFileOption&);

	/**
	 * Opérateur =. RAS.
	 */
	LoadableFileOption& operator = (const LoadableFileOption&);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~LoadableFileOption ( );

	/**
	 * @return		Le nom de l'option.
	 */
	virtual const IN_STD string& getName ( ) const;

	/**
	 * @return		La valeur de l'option.
	 */
	virtual const IN_STD string& getValue ( ) const;
};	// class LoadableFileOption


/**
 * Cette classe gère une liste de noms de fichiers et d'options à
 * appliquer à ces fichiers.
 */
class LoadableFileSet
{
	public :

	/**
	 * Constructeur par défaut. RAS.
	 */
	LoadableFileSet ( );

	/**
	 * Constructeurs de copie et opérateur = : RAS.
	 */
	LoadableFileSet (const LoadableFileSet&);
	LoadableFileSet& operator = (const LoadableFileSet&);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~LoadableFileSet ( );

	/**
	 * @return		Le nombre de fichiers de l'ensemble.
	 */
	virtual size_t getFileCount ( ) const;

	/**
	 * @return		Le nom du i-ème fichier de l'ensemble (1er indice : 0).
	 */
	virtual IN_STD string getFileName (size_t index) const;

	/**
	 * Ajoute le nom de fichier reçu en argument s'il ne figure pas 
	 * déjà dans l'ensemble.
	 */
	virtual void addFileName (const IN_STD string& name);

	/**
	 * @return		Le nombre d'options à appliquer aux fichiers.
	 */
	virtual size_t getOptionsCount ( ) const;

	/**
	 * @return		La i-ème option à appliquer aux fichiers (1er indice 
	 *				: 0).
	 */
	virtual LoadableFileOption getOption (size_t index) const;

	/**
	 * Ajoute l'option reçue en argument si elle ne figure pas déjà dans
	 * l'ensemble des options (nom/valeur).
	 * @param		Nom de l'option
	 * @param		Valeur de l'option
	 */
	virtual void addOption (
					const IN_STD string& name, const IN_STD string& value);

	/**
	 * Affiche l'ensembles des fichiers et leurs options dans le flux
	 * transmis en argument.
	 */
	virtual void print (IN_STD ostream& stream) const;

	/**
	 * Créé une liste d'instances de cette classe à partir des
	 * arguments tranmsis.
	 * Les noms de fichiers sont éventuellement précédés de
	 * "-file", et ils précèdent les éventuelles options qui leurs
	 * sont applicables.<BR>
	 * Les options à appliquer commencent par le caractère "-", et sont
	 * éventuellement suivies d'une valeur. Exception : "-file".<BR>
	 * Ex :<BR>
	 * <CODE>
	 * file1 file2 -opt1 val1 -opt2 -opt3 -file file3 file4 -file file5 -opt5
	 * </CODE>
	 */
	static IN_STD vector <LoadableFileSet> createFileSets (
												int argc, char* argv []);


	private :

	/** La liste des noms de fichiers de l'ensemble. */
	IN_STD vector <IN_STD string>		_fileNames;

	/** La liste des options à appliquer aux fichiers. */
	IN_STD vector <LoadableFileOption>	_options;
};	// class LoadableFileSet


IN_STD ostream& operator << (IN_STD ostream& os, const LoadableFileSet& file);


END_NAMESPACE_UTIL


#endif	// LOADABLE_FILE_SET_H

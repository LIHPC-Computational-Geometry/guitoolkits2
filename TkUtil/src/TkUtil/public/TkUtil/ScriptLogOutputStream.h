#ifndef SCRIPT_LOG_OUTPUT_STREAM_H
#define SCRIPT_LOG_OUTPUT_STREAM_H

#include <TkUtil/FileLogOutputStream.h>
#include <TkUtil/ScriptingLog.h>

#include <vector>


BEGIN_NAMESPACE_UTIL


/**
 * <P>Classe abstraite représentant un flux sortant de logs dirigé dans
 * un fichier ASCII de type script.</P>
 * <P>Un tel fichier est voué à être exécuté via un interpréteur de scripts
 * tel que python.</P>
 * <P>Cette classe supporte les scripts avec valeurs balisées
 * associés (cf. classe <I>ScriptingTools</I>. Ces valeurs balisées, précédées
 * par la chaîne de caractère retournée par la
 * méthode <I>getTagDeclaration</I>, permettent de définir dans le scripts des
 * chaînes de caractères identifiées par des balises dont le rôle est de définir
 * un type (fichier en entrée, nom de variable, ...). Ce dispositif permet à un
 * interpreteur de substituer les valeurs de certaines variables par d'autres
 * variables avant exécution du script.
 * </P>
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 * @see		ScriptingTools
 */
class ScriptLogOutputStream : public FileLogOutputStream
{
	public :

	/**
	 * Constructeur. Invoque setFile et est de ce fait susceptible de lever une exception. L'enregistrement des fichiers (entrée/sortie) est par
	 * défaut inactivée.
	 * @param		Nom du fichier script utilisé. Ecrase un éventuel fichier préexistant.
	 * @param		Masque appliqué.
	 * @param		true si les dates doivent être affichées (sous forme de commentaires)
	 * @param		true si les heures doivent être affichées (sous forme de commentaires)
	 * @param		true si le script doit être exécutable (flag S_IXUSR)
	 * @see			setFile
	 * @see			recordInputFiles
	 * @see			recordOutputFiles
	 * @see			setScriptRights
	 */
	ScriptLogOutputStream (
		const UTF8String& fileName, Log::TYPE mask = Log::SCRIPTING, bool enableDate = true, bool enableTime = true, bool executable = false);

	/**
	 * Constructeur. Invoque setFile et est de ce fait susceptible de lever une exception. L'enregistrement des fichiers (entrée/sortie) est par
	 * défaut inactivée.
	 * @param		Nom du fichier script utilisé. Ecrase un éventuel fichier préexistant.
	 * @param		Jeu de caractères à utiliser lors des écritures
	 * @param		Masque appliqué.
	 * @param		true si les dates doivent être affichées (sous forme de commentaires)
	 * @param		true si les heures doivent être affichées (sous forme de commentaires)
	 * @param		true si le script doit être exécutable (flag S_IXUSR)
	 * @see			setFile
	 * @see			recordInputFiles
	 * @see			recordOutputFiles
	 * @see			setScriptRights
	 */
	ScriptLogOutputStream (
		const UTF8String& fileName, const IN_UTIL Charset& charset, Log::TYPE mask = Log::SCRIPTING, bool enableDate = true, bool enableTime = true, bool executable = false);

	/**
	 * Destructeur. Ferme le fichier.
	 */
	virtual ~ScriptLogOutputStream ( );

	/**
	 * Ecrit le log reçu en argument s'il est compatible avec le masque en
	 * cours.
	 * Invoque logInstruction ou logComment selon la nature du log.
	 * @exception	Méthode susceptible de lever une exception notament si
	 *				writeHeader n'a pas été invoquée au préalable.
	 * @see			logComment
	 * @see			logInstruction
	 * @see			writeHeader
	 */
	virtual void log (const Log& log);

	/** Change le fichier d'écriture des logs et invoque writeHeader. Confère éventuellement le caractère 
	 * "exécutable" au fichier.
	 * @param		Nom du fichier utilisé pour écrire les logs.
	 * @param		Non utilisé. L'éventuel fichier existant sera effacé.
	 * @exception	Une exception est levée en cas d'erreur
	 * @see			setScriptRights
	 */
	virtual void setFile (const UTF8String& fileName, bool truncate);

	/**
	 * @return		Le shell à utiliser pour exécuter le script.
	 * @see			setShell
	 */
	virtual IN_STD string getShell ( ) const
	{ return _shell; }

	/**
	 * @param		Shell à utiliser pour exécuter le script.
	 * @see			getShell
	 * @see			writeHeader
	 */
	virtual void setShell (const IN_STD string& shell);

	/**
	 * @return		Les paquetages à utiliser pour pouvoir exécuter le script.
	 * @see			addPackage
	 */
	virtual IN_STD vector <IN_STD string> getPackages ( ) const;

	/**
	 * @param		Nom de paquetage à utiliser pour pouvoir exécuter le script.
	 * @see			getPackages
	 * @see			writeHeader
	 */
	virtual void addPackage (const IN_STD string& packageName);

	/**
	 * @return		Les chemins à utiliser pour pouvoir charger dynamiquement
	 *				des paquetages.
	 * @see			addPackagePath
	 */
	virtual IN_STD vector <IN_STD string> getPackagePaths ( ) const;

	/**
	 * @param		Chemin à utiliser pour pouvoir charger dynamiquement un ou
	 *				plusieurs paquetages.
	 * @see			addPackagePath
	 * @see			writeHeader
	 */
	virtual void addPackagePath (const IN_STD string& packagePath);

	/**
	 * Ecrit l'en-tête du script. Invoque à cet effet :<BR>
	 * <OL>
	 * <LI>writeShellDeclaration
	 * <LI>writeScriptComments
	 * <LI>writePackagesDeclaration
	 * </OL>
	 * Invoquer cette méthode en fin de surcharge.
	 * @see		isHeaderWritten
	 * @see		writeShellDeclaration
	 * @see		writeScriptComments
	 * @see		writePackagesDeclaration
	 */
	virtual void writeHeader ( );

	/**
	 * @return		true si l'en-tête a déjà été écrit, false dans le cas
	 *				contraire.
	 */
	virtual bool isHeaderWritten ( ) const
	{ return _headerWritten; }

	/**
	 * Déclare le début d'un bloc d'instructions.
	 * Ne fait rien par défaut, méthode à surcharger éventuellement selon les
	 * langages d'implémentation.
	 * @see			closeBlock
	 */
	virtual void declareBlock ( )
	{ }

	/**
	 * Déclare la fin d'un bloc d'instructions.
	 * Ne fait rien par défaut, méthode à surcharger éventuellement selon les
	 * langages d'implémentation.
	 * @see			declareBlock
	 */
	virtual void closeBlock ( )
	{ }

	/**
	 * Déclare le début d'un block try/catch.
	 * Ne fait rien par défaut, méthode à surcharger éventuellement selon les
	 * langages d'implémentation.
	 * @see		closeTryBlock
	 */
	virtual void declareTryBlock ( )
	{ }

	/**
	 * Déclare la fin d'un block try/catch.
	 * Ne fait rien par défaut, méthode à surcharger éventuellement selon les
	 * langages d'implémentation.
	 * @see		declareTryBlock
	 * @see		closeUtilExcTryBlock
	 */
	virtual void closeTryBlock ( )
	{ }

	/**
	 * Déclare la fin d'un block try/catch en récupérant une exception de type
	 * TkUtil::Exception puis une exception standard.
	 * Ne fait rien par défaut, méthode à surcharger éventuellement selon les
	 * langages d'implémentation.
	 * @see		declareTryBlock
	 * @see		Exception
	 */
	virtual void closeUtilExcTryBlock ( )
	{ }

	/**
	 * @return		<I>true</I> si les fichiers en entrée doivent être intégrés
	 *				au script (sous forme de commentaires), <I>false</I> dans
	 *				le cas contraire. De tels fichiers sont repérés à l'aide
	 *				du couple (balise, valeur) où <I>balise</I> vaut
	 *				la valeur retournée par <I>getInputFileTag</I> et
	 *				<I>valeur</I> est le nom du fichier.
	 * @see			recordInputFiles(bool)
	 * @see			getInputFileTag
	 */
	virtual bool recordInputFiles ( ) const
	{ return _recordInputFiles; }

	/**
	 * @param		<I>true</I> si les fichiers en entrée doivent être intégrés
	 *				au script (sous forme de commentaires), <I>false</I> dans
	 *				le cas contraire.
	 * @see			recordInputFiles()
	 * @see			getInputFileTag
	 */
	virtual void recordInputFiles (bool record)
	{ _recordInputFiles	= record; }


	/**
	 * @return		<I>true</I> si les fichiers en sortie doivent être intégrés
	 *				au script (sous forme de commentaires), <I>false</I> dans
	 *				le cas contraire. De tels fichiers sont repérés à l'aide
	 *				du couple (balise, valeur) où <I>balise</I> vaut
	 *				la valeur retournée par <I>getOutputFileTag</I> et
	 *				<I>valeur</I> est le nom du fichier.
	 * @see			recordOutputFiles(bool)
	 * @see			getOutputFileTag
	 */
	virtual bool recordOutputFiles ( ) const
	{ return _recordOutputFiles; }

	/**
	 * @param		<I>true</I> si les fichiers en sortie doivent être intégrés
	 *				au script (sous forme de commentaires), <I>false</I> dans
	 *				le cas contraire.
	 * @see			recordOutputFiles()
	 * @see			getOutputFileTag
	 */
	virtual void recordOutputFiles (bool record)
	{ _recordOutputFiles	= record; }


	protected :

	/**
	 * Affecte au script les droits d'accès spécifiés.
	 */
	 virtual void setScriptRights ( );		// v 5.11.0
	 
	/**
	 * Ecrit dans le script la déclaration du shell à invoquer.
	 * @exception	En cas d'erreur d'écriture ou de flux invalide.
	 * @see			setShell
	 */
	virtual void writeShellDeclaration ( );

	/**
	 * Ecrit dans le script des commentaires généraux (date de création,
	 * utilisateur, application qui a créé le script, ...
	 * @exception	En cas d'erreur d'écriture ou de flux invalide.
	 */
	virtual void writeScriptComments ( );

	/**
	 * Ecrit dans le script la déclaration des paquetages (et de leurs chemins
	 * d'accès) à utiliser.
	 * Ne fait rien par défaut, méthode à surcharger.
	 */
	virtual void writePackagesDeclaration ( )
	{ }

	/**
	 * Ecrit l'instruction tranmise en argument dans le script. Pour ce,
	 * invoque <I>formatInstruction</I> si le script n'est pas formaté, ou 
	 * <I>getText</I> s'il est formaté.
	 * @exception	En cas d'erreur d'écriture ou de flux invalide.
	 * @see			ScriptingLog::isFormated
	 * @see			formatInstruction
	 */
	virtual void logInstruction (const ScriptingLog& log);

	/**
	 * @return		La balise qui annonce la déclaration d'une valeur
	 *				balisée dans une ligne de commentaire. La valeur par défaut
	 *				est la variable de classe <I>tagDeclaration</I>.
	 * @see			logTags
	 * @see			setTagDeclaration
	 */
	virtual const IN_STD string& getTagDeclaration ( ) const
	{ return _tagDeclaration; }

	/**
	 * @param		Nouvelle balise de déclaration de valeur balisée dans une
	 *				ligne de commentaire.
	 * @see			getTagDeclaration
	 * @see			logTags
	 */
	virtual void setTagDeclaration (const IN_STD string& declaration)
	{ _tagDeclaration	= declaration; }

	/**
	 * Ecrit chaque valeur balisée sous forme d'une ligne de
	 * commentaire. La ligne de commentaire commence par la balise définie par
	 * tagDeclaration.
	 * @see			logTag
	 * @see			getTagDeclaration
	 */
	virtual void logTags (const IN_STD vector < TaggedValue >& tags);

	/**
	 * <P>Ecrit la valeur balisée sous forme d'une ligne de
	 * commentaire. La ligne de commentaire commence par la balise définie par
	 * <I>tagDeclaration</I>.</P>
	 * <P>Si la balise correspond à un fichier (entrée/sortie) et que ce type
	 * de fichier doit être enregistré, effectue alors l'enregistrement du 
	 * fichier sous forme de commentaire.
	 */
	virtual void logTag (const TaggedValue& taggedValue);

	/**
	 * @return		La balise associée à un fichier en entrée. La valeur suivant
	 *				cette balise est le nom complet du fichier. La valeur par
	 *				défaut de cette balise est
	 *				<I>ScriptingTools::inputFileTag</I>.
	 * @see			setInputFileTag
	 * @see			recordInputFiles
	 */
	virtual const IN_STD string& getInputFileTag ( ) const
	{ return _inputFileTag; }

	/**
	 * @param		La nouvelle balise associée à un fichier en entrée.
	 * @see			getInputFileTag
	 * @see			recordInputFiles
	 */
	virtual void setInputFileTag (const IN_STD string& tag)
	{ _inputFileTag	= tag; }

	/**
	 * @return		La balise associée à un fichier en sortie. La valeur suivant
	 *				cette balise est le nom complet du fichier. La valeur par
	 *				défaut de cette balise est
	 *				<I>ScriptingTools::outputFileTag</I>.
	 * @see			setOutputFileTag
	 * @see			recordOutputFiles
	 */
	virtual const IN_STD string& getOutputFileTag ( ) const
	{ return _outputFileTag; }

	/**
	 * @param		La nouvelle balise associée à un fichier en sortie.
	 * @see			getOutputFileTag
	 * @see			recordOutputFiles
	 */
	virtual void setOutputFileTag (const IN_STD string& tag)
	{ _outputFileTag	= tag; }

	/**
	 * @param		La déclaration de début de fichier enregistré dans le script
	 *				(à des fins d'extraction de son contenu).
	 * @see			getFileBeginingTag
	 * @see			setEndOfFileTag
	 */
	virtual const IN_STD string& getFileBeginingTag ( ) const
	{ return _fileBeginingTag; }

	/**
	 * @param		La nouvelle déclaration de début de fichier enregistré dans
	 *				le script.
	 * @see			getFileBeginingTag
	 */
	virtual void setFileBeginingTag (const IN_STD string& tag)
	{ _fileBeginingTag	= tag; }

	/**
	 * @param		La déclaration de fin de fichier enregistré dans le script
	 *				(à des fins d'extraction de son contenu).
	 * @see			setEndOfFileTag
	 * @see			setEndOfFileTag
	 */
	virtual const IN_STD string& getEndOfFileTag ( ) const
	{ return _endOfFileTag; }

	/**
	 * @param		La nouvelle déclaration de fin de fichier enregistré dans
	 *				le script.
	 * @see			getEndOfFileTag
	 */
	virtual void setEndOfFileTag (const IN_STD string& tag)
	{ _endOfFileTag	= tag; }

	/**
	 * Ecrit le commentaire tranmis en argument dans le script.
	 * @exception	En cas d'erreur d'écriture ou de flux invalide.
	 * @see			formatComment
	 */
	virtual void logComment (const Log& log);

	/**
	 * Enregistre sous forme de lignes commentées le fichier dont le nom complet
	 * est transmis en argument.
	 */
	virtual void recordFileVariable (const IN_STD string& fullName);

	/**
	 * Formate l'instruction tranmise en argument conformément au langage
	 * script représenté.
	 * @return		Instruction prête à être écrite dans le script.
	 * @exception	Méthode susceptible de lever une exception.
	 * @warning		Fait appel à formatInstanceAccess dans le cas ou le log
	 *				fait référence à un objet nommé.
	 * @see			formatResult
	 * @see			formatInstanceAccess
	 * @see			ScriptingLog#useObjectManagerApi
	 */
	virtual IN_STD string formatInstruction (const ScriptingLog& log) = 0;

	/**
	 * @return		La chaine de caractères d'affectation de l'opération à une
	 *				éventuelle variable du script, ou une chaine vide en
	 *				l'absence d'affectation.
	 * @see			ScriptingLog#hasResult
	 * @see			ScriptingLog#getResult
	 */
	virtual IN_STD string formatResult (const ScriptingLog& log) = 0;

	/**
	 * @return		La chaine de caractères permettant d'accéder à l'objet
	 *				nommé associé au log.
	 * @see			formatInstanceVariableAccess
	 */
	virtual IN_STD string formatInstanceAccess (const ScriptingLog& log) = 0;

	/**
	 * <P>Fonction utilisée pour accéder à une instance transmise en argument
	 * dans un script. L'API actuelle ne fait pas de distinction entre les
	 * instances passées en IN, OUT et INOUT.</P>
	 * <P>Si l'instance transmise en argument n'est pas recensée par le
	 * gestionnaire d'objets nommés (classe <I>ReferencedObjectManager</I>),
	 * cette méthode retourne alors la chaine de caractères tranmise en
	 * argument. Ce dispositif permet de transmettre en argument une variable
	 * du script.</P>
	 * @param		Nom unique d'une instance de classe dérivée de
	 *				ReferencedNamedObject dont on souhaite obtenir une
	 *				chaine de caractères permettant dans un script d'y
	 *				accéder en tant qu'argument d'une méthode. <B>En cas de
	 *				chaine vide l'instance est supposée être nulle.</B>
	 * @return		La chaine de caractères permettant d'accéder à l'objet
	 *				dont le nom unique est donné en argument.
	 * @warning		Susceptible d'évoluer.
	 * @see			ReferencedObjectManager
	 */
	virtual IN_STD string formatInstanceVariableAccess (const IN_STD string& uniqueName) = 0;

	/**
	 * Formate le commentaire tranmis en argument conformément au langage
	 * script représenté.
	 * @param		Commentaire à formater
	 * @param		<I>true</I> si le formatage doit être strict (minimum
	 *				nécessaire), <I>false s'il peut être agrémenté
	 *				(espaces, ...).
	 * @return		Commentaire prêt à être écrit dans le script.
	 * @exception	Méthode susceptible de lever une exception.
	 */
	virtual IN_UTIL UTF8String formatComment (const Log& log, bool scrict = false) = 0;

	/**
	 * Ecrit dans le script, sur une même ligne, la chaine de caractères
	 * transmise en argument, puis écrit un saut de ligne.
	 * @exception	Une exception est levée en cas d'erreur d'écriture.
	 */
	virtual void write (const IN_UTIL UTF8String& str);


	private :

	/**
	 * Constructeurs de copie et opérateur = : interdits.
	 */
	ScriptLogOutputStream (const ScriptLogOutputStream&);
	ScriptLogOutputStream& operator = (const ScriptLogOutputStream&);

	/** Le shell à utiliser pour exécuter le script. */
	IN_STD string						_shell;

	/** Les chemins permettant de charger dynamiquement des paquetages pour
	 * pouvoir exécuter le script. */
	IN_STD vector <IN_STD string>		_packagePaths;

	/** Les paquetages à utiliser pour pouvoir exécuter le script. */
	IN_STD vector <IN_STD string>		_packages;

	/** true si l'en-tête a déjà été écrit, false dans le cas contraire. */
	bool								_headerWritten;

	/** La balise qui annonce la déclaration d'une valeur balisée dans
	 * une ligne de commentaire. Vaut par défaut
	 * <I>ScriptingTools::tagDeclaration</I>.*/
	IN_STD string						_tagDeclaration;

	/** La balise correspondant à un fichier en entrée. Vaut 
	 * <I>ScriptingTools::inputFileTag</I> par défaut. */
	IN_STD string						_inputFileTag;

	/** La balise correspondant à un fichier en sortie. Vaut 
	 * <I>ScriptingTools::outputFileTag</I> par défaut. */
	IN_STD string						_outputFileTag;

	/** Les déclaration de début et de fin de fichier enregistré dans le script
	 * (à des fins d'extraction de son contenu). */
	IN_STD string						_fileBeginingTag, _endOfFileTag;

	/** Faut-il enregistrer les fichiers en entrée dans le script ? */
	bool								_recordInputFiles;

	/** Faut-il enregistrer les fichiers en sortie dans le script ? */
	bool								_recordOutputFiles;
	
	/** Le script doit il être créé "exécutable" (flag S_IXUSR) ? */
	bool								_executable;	// v 5.11.0
};	// class ScriptLogOutputStream


END_NAMESPACE_UTIL


#endif	// SCRIPT_LOG_OUTPUT_STREAM_H

#ifndef SCRIPTING_TOOLS_H
#define SCRIPTING_TOOLS_H

#include <TkUtil/TaggedValue.h>

#include <iostream>
#include <vector>
#include <string>


BEGIN_NAMESPACE_UTIL


/**
 * <P>Classe de services pour le scripting.
 * </P>
 * <P>Les services actuels reposent sur la notion de valeur balisée
 * dont les valeurs ont vocation à être éventuellement remplacés par un 
 * par un interpréteur avant ou pendant exécution du script. Les valeurs
 * servent par exemple à identifier une instance ou un nom de fichier. Ces
 * valeurs à remplacer sont déclarées dans une ligne de commentaire et
 * précédée d'une balise renseignant le type de la valeur. Une telle ligne de
 * commentaire est-elle même identifiée par une balise.<BR>
 * Exemple de ligne (cas d'un script <I>python</I>) : <BR>
 * <CODE>
 * # !!!TAG CONTENEUR Container_0
 * </CODE>
 * Ici :<BR>
 * <OL>
 * <LI><I>!!!TAG</I> définie une ligne déclarant une valeur balisée,
 * <LI><I>CONTENEUR</I> déclare que valeur balisée concerne une variable de type
 * conteneur,
 * <LI><I>Container_0</I> est l'identifiant du conteneur.
 * </OL>
 * <BR>
 * La méthode <I>replace</I> de cette classe permet par exemple de remplacer
 * toutes les occurences de <I>Container_0</I> par un autre identifiant.
 * </P>
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 * @see		ScriptLogOutputStream
 */
class ScriptingTools
{
	public :


	/**
	 * Recherche les valeurs balisées dans le flux transmis en argument.
	 * Ces valeurs balisées sont précédées elles-mêmes de la balise définie par 
	 * <I>tagDeclaration</I>.
	 * @param		flux utilisé pour la recherche
	 * @param		balise de déclaration d'une valeur balisée.
	 * @param		fonction retournant <I>true</I> si la ligne transmise en
	 *				argument est une ligne de commentaire, <I>false</I> dans le
	 *				cas contraire.
	 * @return		Les valeurs balisées trouvées.
	 */	
	static IN_STD vector < TaggedValue >
		findTags (IN_STD_IO istream& in, const IN_STD string& tagDeclaration,
		          bool (*commentLine)(const IN_STD string&));

	/**
	 * Recopie le script dans le flux en entrée dans le flux en sortie.
	 * Les occurences en premier argument des valeurs balisées sont remplacées
	 * par celles en second arguments.
	 */
	static void replace (IN_STD_IO istream& in, IN_STD_IO ostream& os,
	                     const IN_STD vector < IN_STD pair <
									IN_STD string, IN_STD string > >& words);

	/**
	 * Extrait un fichier inclu sous forme de commentaire dans un flux entrant
	 * et l'écrit dans un flux sortant.
	 * @param		flux entrant (script) d'où l'on va extraire le fichier.
	 * @param		flux sortant où l'on va écrire le fichier.
	 * @param		Nom complet du fichier à extraire
	 * @param		balise de déclaration d'une balise.
	 * @param		balise de déclaration du début du fichier.
	 * @param		balise de déclaration de la fin du fichier.
	 * @param		fonction retournant <I>true</I> si la ligne transmise en
	 *				argument est une ligne de commentaire, <I>false</I> dans le
	 *				cas contraire.
	 * @param		fonction retournant la ligne de commentaire transmise en
	 *				argument privée de ces caractères de déclaration de ligne de
	 *				commentaire.
	 * @see			pythonCommentLine
	 * @see			uncommentPythonLine
	 */
	static void extractFile (
		IN_STD_IO istream& in, IN_STD_IO ostream& os,
		const IN_STD string& fileName, const IN_STD string& tagDeclaration,
		const IN_STD string& beginingTag, const IN_STD string& endTag,
		bool (*commentLine)(const IN_STD string&),
		IN_STD string (*uncomment)(const IN_STD string&));

	/**
	 * Support de base pour les scripts <I>python</I>.
	 * @return		<I>true</I> si la ligne transmise en argument est une ligne
	 *				de commentaires <I>python</I>, <I>false</I> dans le cas
	 *				contraire.
	 */
	static bool pythonCommentLine (const IN_STD string& line);

	/**
	 * Support de base pour les scripts <I>python</I>.
	 * @return		La ligne de commentaire <I>python</I> transmise en argument
	 *				provée de sa déclaration de commentaire.
	 */
	static IN_STD string uncommentPythonLine (const IN_STD string& line);

	/** Chaîne de caractères permettant de déclarer la présence d'une valeur
	 * balisée dans une ligne de script commentée.<BR>
	 * Cette variable est publique afin de pouvoir la modifier par lecture en
	 * configuration. Vaut par défaut "!!!TAG".
	 */
	static IN_STD string			tagDeclaration;

	/** Chaîne de caractères définissant le type "fichier en entrée.".
	 * Cette variable est publique afin de pouvoir la modifier par lecture en
	 * configuration. Vaut par défaut "FICHIER_ENTREE".
	 */
	static IN_STD string			inputFileTag;

	/** Chaîne de caractères définissant le type "fichier en sortie.".
	 * Cette variable est publique afin de pouvoir la modifier par lecture en
	 * configuration. Vaut par défaut "FICHIER_SORTIE".
	 */
	static IN_STD string			outputFileTag;

	/** Les déclaration de début et de fin de fichier enregistré dans un script
	 * (à des fins d'extraction de son contenu). Ces variables sont publiques
	 * afin de pouvoir les modifier par lecture en configuration. 
	 * <I>fileBeginingTag</I> vaut par défaut "DEBUT_FICHIER", et 
	 * <I>endOfFileTag</I> vaut "FIN_FICHIER". */
	static IN_STD string			fileBeginingTag, endOfFileTag;

	
	protected :

	/**
	 * Remplace chaque occurence de <I>what</I> par <I>with</I> dans
	 * <I>str</I>.
	 */
	static void replace (
		IN_STD string& str, const IN_STD string& what, const IN_STD string& with);

	private :

	/**
	 * Constructeurs, destructeur et opérateur = : interdits.
	 */
	ScriptingTools ( );
	ScriptingTools (const ScriptingTools&);
	ScriptingTools& operator = (const ScriptingTools&);
	~ScriptingTools ( );
};	// class ScriptingTools


END_NAMESPACE_UTIL


#endif	// SCRIPTING_TOOLS_H

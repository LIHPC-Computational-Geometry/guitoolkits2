#ifndef LOCALE_H
#define LOCALE_H

#include <TkUtil/util_config.h>
#include <TkUtil/UTF8String.h>

#include <string>
#include <vector>


BEGIN_NAMESPACE_UTIL


/**
 * <P>
 * Cette classe permet d'accéder à des informations propres à l'environnement
 * d'exécution des programmes.
 * </P>
 * <P>Les informations se présentent sous la forme :<BR>
 * <BR>
 * langage[_territoire][.codeset][2modifier]
 * </P>
 * <P>Lors du lancement d'un programme le système doit initialiser
 * l'ensemble des informations d'environnement à "C". Le jeu de caractères
 * utilisé est le code ASCII, donc sans accent, et le caractère séparant la
 * partie entière des nombres est le point.
 * <P>
 * <P>Il est bien sûr possible de changer le jeu de caractères utilisé en
 * choisissant par exemple le jeu ISO8859-1 correspondant à l'Europe de
 * l'Ouest et donc à la France.
 * </P>
 * <P><B>ATTENTION :</B> <BR>
 * Pour chaque méthode get, ALL est testé en premier, puis la catégorie 
 * renseignée en argument, et enfin la variable d'environnement <B>LANG</B>.
 * </P>
 * @see		man locale
 * @see		man setlocale
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class Locale
{
	public :


	/**
	 * Les catégories de variables de l'environnements : tout, type de chaines
	 * de caractères, type de tri, date/heure, numérique, monnaie, messages.
	 */
	enum TYPE { ALL, C_TYPE, COLLATE, TIME, NUMERIC, MONETARY, MESSAGES };

	/**
	 * L'origine d'une variable <I>locale</I> :
	 * <OL>
	 * <LI><I>EXE</I> : variables réellement utilisées par le programme, et
	 * initialisées à "C" au lancement du programme, accessibles par
	 * <I>setlocale</I></LI>
	 * <LI><I>USER_ENV</I> : variables affectées dans le terminal de lancement
	 * du programme, accessibles par <I>getenv</I> sur les variables
	 * de catégorie <I>LC_</I> et <I>LANG</I>,</LI>
	 * <LI><I>BOTH</I> : EXE, puis BOTH.
	 * </OL>
	 */
	enum ORIGIN { EXE, USER_ENV, BOTH };

	/**
	 * Affiche les informations sur l'environnement dans le flux reçu en 
	 * argument.
	 * @param		Localisation de la recherche d'information
	 */
	static IN_STD string toStr (ORIGIN origin = EXE);

	/**
	 * @param		Variable de locale concernée
	 * @param		Localisation de la recherche d'information
	 * @return		La variable pour la catégorie renseignée en argument, ou "C"
	 *				si rien n'est définit.
	 */
	static IN_STD string getLocale (TYPE type, ORIGIN origin = EXE);

	/**
	 * @param		Variable de locale concernée
	 * @param		Nouvelle locale à appliquer.
	 * @exception	Une exception est levée en cas d'échec.
	 */
	static void setLocale (TYPE type, const IN_STD string& locale);

	/**
	 * @param		Localisation de la recherche d'information
	 * @return		La langue en cours, "C" si rien n'est définit.
	 * @warning		La valeur retournée n'est pas nécessairement portable,
	 *				notamment si différentes valeurs sont affectées aux
	 *				différentes catégories. La chaine retournée peut alors 
	 *				représenter ces différentes valeurs (ex : 
	 *				LC_CTYPE=fr_FR.iso88591;LC_NUMERIC=C; ... sous Linux,
	 *				/fr_FR.ISO8859-1/C/fr_FR.ISO8859-1/... sous Sun, ...).
	 */
	static IN_STD string getLanguage (ORIGIN origin = EXE);

	/**
	 * @param		Variable de locale concernée
	 * @param		Localisation de la recherche d'information
	 * @return		La langue en cours pour la catégorie de variables 
	 *				renseignée en argument, "C" si rien n'est définit.
	 */
	static IN_STD string getLanguage (TYPE type, ORIGIN origin = EXE);

	/**
	 * @param		Variable de locale concernée
	 * @param		Localisation de la recherche d'information
	 * @return		Le territoire en cours pour la catégorie de variables 
	 *				renseignée en argument, "C" si rien n'est définit.
	 * @warning		La valeur retournée n'est pas nécessairement portable
	 *				si type vaut ALL, notamment si différentes valeurs sont 
	 *				affectées aux différentes catégories.
	 */
	static IN_STD string getTerritory (TYPE type, ORIGIN origin = EXE);

	/**
	 * @param		Localisation de la recherche d'information
	 * @return		Le jeu de caractères ou identifiant d'encodage pour
	 *				le type C_TYPE, "C" si rien n'est définit.
	 */
	static IN_STD string getCharset (ORIGIN origin = EXE);

	/**
	 * Connaître le jeu de caractères de l'environnement d'exécution de
	 * l'application. Objectif : savoir, par exemple, si la sortie standard est
	 * ISO-8859 ou UTF8.
	 * @param		Une éventuelle chaîne de caractères de "référence" utilisée
	 *				pour reconnaître le jeu de caractères. En son absence le
	 *				jeu de caractères est déterminé à partir de l'environnement.
	 * @return		Le jeu de caractères utilisé par le processus qui a lancé
	 *				le processus courant (ex : console, xterm, ...).
	 * @warning		En cas d'échec de détection retourne
	 * 				Charset (Charset::UNKNOWN).
	 * @see			Charset
	 */
	static IN_UTIL Charset detectCharset (	// v 4.0.0
									const IN_STD string& refString);

	/**
	 * @param		Localisation de la recherche d'information
	 * @return		true si le jeu de caractères est un jeu ISO 8859.
	 */
	static bool isIsoCharset  (ORIGIN origin = EXE);

	/**
	 * @param		Localisation de la recherche d'information
	 * @return		la partie du jeu de caractères ISO (ex : 4 pour ISO 8859-4)
	 * @exception	Une excpetion est levée si le jeu de caractères n'est pas
	 *				ISO ou si la partie n'est pas déterminable.
	 */
	static size_t getIsoCharSetPart (ORIGIN origin = EXE);

	/**
	 * @param		Localisation de la recherche d'information
	 * @return		une chaine de caractères de type ISO-8859-X ou X est la 
	 *				partie du jeu de caractères ISO 8859
	 * @exception	Une exception est levée si le jeu de caractères n'est pas
	 *				ISO 8859. Le suffixe -X n'est pas ajouté si la partie n'est
	 *				pas déterminable.
	 */
	static IN_STD string getNormalizedIsoString (ORIGIN origin = EXE);

	/**
	 * @param		Variable de locale concernée
	 * @param		Localisation de la recherche d'information
	 * @return		Le jeu de caractères ou identifiant d'encodage en cours pour
	 *				la catégorie de variables renseignée en argument, "C" si 
	 *				rien n'est définit.
	 */
	static IN_STD string getCodeset (TYPE type, ORIGIN origin = EXE);

	/**
	 * @param		Variable de locale concernée
	 * @param		Localisation de la recherche d'information
	 * @return		Le modifieur en cours pour la catégorie de variables 
	 *				renseignée en argument, "C" si rien n'est définit.
	 */
	static IN_STD string getModifier (TYPE type, ORIGIN origin = EXE);

	/**
	 * @param		Localisation de la recherche d'information
	 * @return		La chaine de caractères servant à séparer la partie entière
	 *				des décimales pour les nombres scientifiques.
	 * @exception	Une exception est levée si cette information n'est pas 
	 *				accessible.
	 * @see			man localeconv
	 */
	static IN_STD string getDecimalPoint (ORIGIN origin = EXE);

	/**
	 * @return		La liste des locales disponibles sur la machine.
	 * @exception	Une exception peut être levée en cas d'erreur.
	 */
	static IN_STD vector<IN_STD string> getAvailableLocales ( );

	/**
	 * Affecte au programme d'exécution les variables lues dans l'environnement.
	 * @return		Un message détaillant les opérations effectuées.
	 * @exception	Une exception est levée en cas d'échec.
	 */
	static UTF8String setUserLocales ( );

	/**
	 * La valeur par défaut des locales : "C".
	 */
	static const char*		defaultLocale;


	private :

	/**
	 * Constructeurs de copie et opérateur = : interdits.
	 */
	Locale ( );
	Locale (const Locale&);
	Locale& operator = (const Locale&);

	/**
	 * Destructeur : interdit.
	 */
	~Locale ( );

	/**
	 * @param		EXE si la variable doit être recherchée dans 
	 *				l'environnement d'exécution (<I>setlocale</I> en 
	 *				consultation), USER_ENV si elle doit être retrouvée dans
	 *				l'environnement du terminal qui a lancé le programme,
	 *				BOTH si elle doit être recherchée dans l'environnement 
	 *				d'exécution puis dans l'environnement du terminal de 
	 *				lancement.
	 * @return		La variable à prendre en compte selon l'argument donné.
	 *				Ordre d'évaluation : ALL, type, LANG. "C" est retourné en
	 *				cas d'échec.
	 * @see			ORIGIN
	 */
	static IN_STD string getVar (TYPE type, ORIGIN origin);
};	// class Locale



END_NAMESPACE_UTIL


#endif	// LOCALE_H

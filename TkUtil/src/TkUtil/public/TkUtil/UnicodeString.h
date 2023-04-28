#ifndef UNICODE_STRING_H
#define UNICODE_STRING_H


#include <TkUtil/util_config.h>
#include <TkUtil/UTF8String.h>

#include <string>
#include <iostream>
#include <iomanip>


BEGIN_NAMESPACE_UTIL


/**
 * Classe de chaine de caractères unicode 16 bits. Les caractères sont stockés 
 * dans des unsigned short, et la fin de chaine est matérialisée par le 
 * unsigned short 0.
 * Offre de nombreux ponts vers les types <I>std::string</I>, <I>char*</I>,
 * <I>char</I>, et des possibilités d'insertion de nombres.
 * Cette classe peut également être utilisée comme un <I>stream</I> c++ sortant
 * à l'aide des opérateurs <I><<</I> et de certains <I>manipulateurs</I>.
 * @warning		Cette classe propose de nombreux opérateurs avec les nombres
 *				afin de pouvoir les "streamer". Attention avec les types 
 *				<B>char</B> et <B>unsigned short</B> considérés ici comme des
 *				chaines de caractères et non comme des nombres.
 * @warning		Les manipulateurs c++ de flux ne sont utilisés que par les
 *				opérateurs <<.
 * @deprecated	<B>Classe remplacée par la classe UTF8String à partir de la
 * 				version 4.0.0 de la bibliothèque <I>TkUtil</I>.</B>
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class UnicodeString
{
	public :

	friend UnicodeString& operator << (UnicodeString& ustr, setw width);
	friend UnicodeString& operator << (
							UnicodeString& ustr, setprecision prec);
	friend UnicodeString& operator << (
							UnicodeString& ustr, IN_STD ios_base::fmtflags);


	/**
	 * Constructeurs, opérateur = : RAS.
	 */
	UnicodeString ( );
	UnicodeString (const UnicodeString& str);
	UnicodeString (const UTF8String& ustr);
	UnicodeString (const WChar_t* str);
	UnicodeString (const IN_STD string& str);
	UnicodeString (const char* str);
	UnicodeString (WChar_t ws);
	UnicodeString (char c);
	UnicodeString (short s);
	UnicodeString (long l);
	UnicodeString (unsigned long ul);
	UnicodeString (long long l);
	UnicodeString (unsigned long long ul);
	UnicodeString (float f);
	UnicodeString (double d);
	UnicodeString (long double d);
	UnicodeString& operator = (const UnicodeString& str);
	UnicodeString& operator = (const WChar_t* str);
	UnicodeString& operator = (const IN_STD string& str);
	UnicodeString& operator = (const char* str);
	UnicodeString& operator = (WChar_t ws);
	UnicodeString& operator = (char c);
	UnicodeString& operator = (short s);
	UnicodeString& operator = (long l);
	UnicodeString& operator = (unsigned long ul);
	UnicodeString& operator = (long long l);
	UnicodeString& operator = (unsigned long long ul);
	UnicodeString& operator = (float f);
	UnicodeString& operator = (double d);
	UnicodeString& operator = (long double d);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~UnicodeString ( );

	/**
	 * Réinitialise la chaine à rien.
	 */
	virtual void clear ( );

	/**
	 * @return		La longueur de la chaine, caractère de terminaison 
	 *				non compris.
	 */
	IN_STD size_t length ( ) const;

	/**
	 * @return		true si la chaine est vide, sinon false.
	 */
	virtual bool empty ( ) const;

	/** 
	 * @return		false si la chaine contient des caractères non ASCII,
	 *				true dans le cas contraire.
	 */
	virtual bool isAscii ( ) const;

	/** 
	 * @return		false si la chaine contient des caractères non ISO 8859,
	 *				true dans le cas contraire.
	 */
	virtual bool isIso ( ) const;

	/**
	 * @return		La chaine de caractères sous forme unicode.
	 */
	virtual const WChar_t* unicode ( ) const;

	/**
	 * @return		La chaine de caractères sous forme ASCII. Les caractères
	 *				non ASCII ne sont pas insérés dans la chaine retournée.
	 */
	virtual IN_STD string ascii ( ) const;

	/**
	 * @return		La chaine de caractères sous forme ISO 8859. Les caractères
	 *				non ISO 8859 ne sont pas insérés dans la chaine retournée.
	 */
	virtual IN_STD string iso ( ) const;

	/**
	 * @return		true si la chaine contient au moins un espace.
	 */
	virtual bool hasWhiteSpace ( ) const;

	/**
	 * @return		true si les deux chaines sont égales, sinon false.
	 */
	bool operator == (const UnicodeString& toCompare) const;

	/**
	 * @return		true si les deux chaines sont différentes, sinon false.
	 */
	bool operator != (const UnicodeString& toCompare) const;

	/**
	 * @return		iso ( )
	 */
	virtual operator IN_STD string ( ) const;

	/**
	 * @return		Le caractère situé à la position donnée en argument.
	 * @throw		Une exception est levée en cas de position invalide.
	 */
	WChar_t operator [] (size_t pos) const;

	/**
	 * Fonctions de recherche à partir du début.
	 * @param		Elément ou occurence recherchée
	 * @param		Point de départ de la recherche
	 * @return		la position de l'occurence recherchée, ou (size_t)-1 si
	 *				elle ne figure pas dans la chaine.
	 * @exception	En cas de position invalide.
	 */
	size_t find (const UnicodeString& str, size_t pos = 0) const;
	size_t find (const WChar_t* str, size_t pos = 0) const;
	size_t find (const IN_STD string& str, size_t pos = 0) const;
	size_t find (const char* str, size_t pos = 0) const;
	size_t find (WChar_t wc, size_t pos = 0) const;
	size_t find (char c, size_t pos = 0) const;

	/**
	 * Fonctions de recherche à partir de la fin.
	 * @param		Elément ou occurence recherchée
	 * @param		Point de départ de la recherche, ou -1 si la recherche doit
	 *				être effectuée `partir de la fin.
	 * @return		la position de l'occurence recherchée, ou (size_t)-1 si
	 *				elle ne figure pas dans la chaine.
	 * @exception	En cas de position invalide.
	 */
	size_t rfind (const UnicodeString& str, size_t pos = (size_t)-1) const;
	size_t rfind (const WChar_t* str, size_t pos = (size_t)-1) const;
	size_t rfind (const IN_STD string& str, size_t pos = (size_t)-1) const;
	size_t rfind (const char* str, size_t pos = (size_t)-1) const;
	size_t rfind (WChar_t wc, size_t pos = (size_t)-1) const;
	size_t rfind (char c, size_t pos = (size_t)-1) const;

	/**
	 * @param		Premier caractère de la chaine à extraire.
	 * @param		Dernier caractère de la chaine à extraire. Transmettre
	 *				(size_t)-1 pour que ce soit le dernier caractère de la
	 *				chaine.
	 * @return		La chaine de caractères comprise entre les positions
	 *				first et last.
	 * @exception	Si une des deux positions est invalide.
	 */
	UnicodeString substring (size_t first, size_t last = (size_t)-1) const;

	/**
	 * Opérateurs de concaténation. Ajoutent l'argument reçu en fin de chaine.
	 * @param		Elément à ajouter
	 * @return		Une référence sur cette chaine.
	 */
	UnicodeString& operator += (const UnicodeString& str);
	UnicodeString& operator += (const WChar_t* str);
	UnicodeString& operator += (const IN_STD string& str);
	UnicodeString& operator += (const char* str);
	UnicodeString& operator += (WChar_t ws);
	UnicodeString& operator += (char c);
	UnicodeString& operator += (short s);
	UnicodeString& operator += (long l);
	UnicodeString& operator += (unsigned long ul);
	UnicodeString& operator += (long long l);
	UnicodeString& operator += (unsigned long long ul);
	UnicodeString& operator += (float f);
	UnicodeString& operator += (double d);
	UnicodeString& operator += (long double d);
	UnicodeString& operator << (const UnicodeString& str);
	UnicodeString& operator << (const WChar_t* str);
	UnicodeString& operator << (const IN_STD string& str);
	UnicodeString& operator << (const char* str);
	UnicodeString& operator << (WChar_t ws);
	UnicodeString& operator << (char c);
	UnicodeString& operator << (short s);
	UnicodeString& operator << (long l);
	UnicodeString& operator << (unsigned long ul);
	UnicodeString& operator << (long long l);
	UnicodeString& operator << (unsigned long long ul);
	UnicodeString& operator << (float f);
	UnicodeString& operator << (double d);
	UnicodeString& operator << (long double d);

	/**
	 * Formate la chaine de caractères sur le nombre de colonne renseigné en
	 * argument. La césure se fait au niveau des espaces.
	 * @return		La chaine de caractères formatée
	 * @exception	Si columnNum est nul.
	 * @see			trim
	 */
	virtual UnicodeString format (size_t columnNum) const;

	/**
	 * @return		La chaine de caractères dépourvue des espaces et sauts de
	 *				ligne en début et fin de chaine. Si removeTabs vaut true
	 *				enlève également les tabulations.
	 * @see			format
	 */
	virtual UnicodeString trim (bool removeTabs = true) const;

	/**
	 * Le caractère espace.
	 */
	static const WChar_t		unicodeWhiteSpace;

	/**
	 * Le caractère saut à la ligne.
	 */
	static const WChar_t		unicodeLineFeed;


	protected :

	/**
	 * Copie l'instance reçue en argument.
	 */
	virtual void copy (const UnicodeString& str);

	/**
	 * Copie la chaine reçue en argument.
	 */
	virtual void copy (const WChar_t* str);

	/**
	 * Copie la chaine reçue en argument.
	 */
	virtual void copy (const IN_STD string& str);

	/**
	 * @return		La longueur de la chaine unicode tranmise en arguement,
	 *				caractère de terminaison non compris.
	 */
	static size_t stringLength (const WChar_t* str);

	/**
	 * Le plus grand caractère ASCII supporté. Le jeu de caractères ASCII va de
	 * 0 à maxASCIIChar.
	 */
	static const unsigned char			maxASCIIChar;

	/**
	 * Le plus grand caractère ISO supporté. Le jeu de caractères ISO 8859 va de
	 * 0 à maxISOChar.
	 */
	static const unsigned char			maxISOChar;


	private :

	/**
	 * La chaine de caractères.
	 */
	WChar_t*		_string;

	/**
	 * La longueur de la chaine de caractères, caractère 0 de terminaison non
	 * compris.
	 */
	size_t			_length;

	/**
	 * La largeur des champs numériques.
	 * @warning		Ne concerne que les opérations de streaming (opérateur <<).
	 */
	int				_numericFieldWidth;

	/**
	 * Le nombre de décimale lors de la conversion de réels.
	 * @warning		Ne concerne que les opérations de streaming (opérateur <<).
	 */
	int				_decimalNumber;

	/**
	 * Choix de l'utilisation (true) ou non (false) de la notation scientifique
	 * (avec exposant) ou non lors du formattage de réels en chaines.
	 * @warning		Ne concerne que les opérations de streaming (opérateur <<).
	 */
	bool			_scientific;
};	// UnicodeString


/**
 * Largeur minimum de formatage des flux entiers.
 * @warning		Ne concerne que les opérations de streaming (opérateur <<).
 */
UnicodeString& operator << (UnicodeString& ustr, setw width);

/**
 * Nombre de décimales de formatage des flux réels.
 * @warning		Ne concerne que les opérations de streaming (opérateur <<).
 */
UnicodeString& operator << (UnicodeString& ustr, setprecision prec);

/**
 * Flags de formattage des nombres. Valeurs prises en compte : fixed et
 * scientific.
 * @warning		Ne concerne que les opérations de streaming (opérateur <<).
 */
UnicodeString& operator << (UnicodeString& ustr, IN_STD ios_base::fmtflags);

/**
 * Injecte str.iso ( ) dans os puis retourne os.
 */
IN_STD ostream& operator << (IN_STD ostream& os, const UnicodeString& str);

/**
 * @return		La concaténation des deux chaines reçues en argument.
 * @warning		Dans les cas std::string et char*, le jeu de caractères utilisé
 * 				est celui par défaut.
 */
UnicodeString operator + (const UnicodeString& us1, const UnicodeString& us2);
UnicodeString operator + (const UnicodeString& us, const IN_STD string& str);
UnicodeString operator + (const UnicodeString& us, const char* str);
UnicodeString operator + (const UnicodeString& us, char c);
UnicodeString operator + (const UnicodeString& us, short s);
UnicodeString operator + (const UnicodeString& us, long l);
UnicodeString operator + (const UnicodeString& us, unsigned long ul);
UnicodeString operator + (const UnicodeString& us, long long l);
UnicodeString operator + (const UnicodeString& us, unsigned long long ul);
UnicodeString operator + (const UnicodeString& us, float f);
UnicodeString operator + (const UnicodeString& us, double d);
UnicodeString operator + (const UnicodeString& us, long double d);

END_NAMESPACE_UTIL

#endif	// UNICODE_STRING_H

/**
 * Classe utilitaire permettant de faire des conversions chaîness de
 * caractères -> nombres.
 * @warning		Classe non validée
 * @todo		Nombreuses fonctions de conversion
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */

#include <TkUtil/util_config.h>

#include <string>


#ifndef NUMERIC_CONVERSIONS_H
#define NUMERIC_CONVERSIONS_H


BEGIN_NAMESPACE_UTIL

class NumericConversions
{
	public :

	/**
	 * @param		Chaîne à convertir
	 * @return		Entier long correspondant
	 * @exception	Une exception de type UTIL::Exception est levée en cas de
	 *				conversion impossible.
	 */
	static long strToLong (const IN_STD string& str);

	/**
	 * @param		Chaîne à convertir
	 * @return		Entier long non signé correspondant
	 * @exception	Une exception de type UTIL::Exception est levée en cas de
	 *				conversion impossible.
	 * @warning		Contrairement à la fonction C <I>strtoul</I>, lève une
	 *				exception si <I>str</I> commence par un signe "-".
	 */
	static unsigned long strToULong (const IN_STD string& str);

	/**
	 * @param		Chaîne à convertir
	 * @return		Entier long correspondant
	 * @exception	Une exception de type UTIL::Exception est levée en cas de
	 *				conversion impossible.
	 */
	static long long strToLongLong (const IN_STD string& str);

	/**
	 * @param		Chaîne à convertir
	 * @return		Entier long non signé correspondant
	 * @exception	Une exception de type UTIL::Exception est levée en cas de
	 *				conversion impossible.
	 * @warning		Contrairement à la fonction C <I>strtoull</I>, lève une
	 *				exception si <I>str</I> commence par un signe "-".
	 */
	static unsigned long long strToULongLong (const IN_STD string& str);

	/**
	 * @param		Chaîne à convertir
	 * @return		Réel double précision correspondant
	 * @exception	Une exception de type UTIL::Exception est levée en cas de
	 *				conversion impossible.
	 */
	static double strToDouble (const IN_STD string& str);

	/**
	 * @param		Chaîne à convertir
	 * @return		Réel long correspondant
	 * @exception	Une exception de type UTIL::Exception est levée en cas de
	 *				conversion impossible.
	 */
	static long double strToLongDouble (const IN_STD string& str);

	/**
	 * @param		Nombre à convertir en chaine de caractères.	
	 * @param		width est le nombre de caractères utilisés au minimum lors 
	 *				du formatage. L'éventuel remplissage est effectué avec des 
	 *				0. Ca n'est pas forcément le nombre de chiffres
	 *				significatifs car en mode scientific le nombre de 
	 *				caractères inclue également le e+-XXX).
	 * @param		precision est le nombre de décimales lors du formatage 
	 *				des nombres réels (et non le nombre de chiffres
	 *				significatifs).
	 * @param		Si scientific est true la notation utilisée 
	 * @see			userRepresentation
	 * @see			shortestRepresentation
	 */
	static IN_STD string toStr (short s);
	static IN_STD string toStr (short s, int width);
	static IN_STD string toStr (unsigned short s);
	static IN_STD string toStr (unsigned short s, int width);
	static IN_STD string toStr (long l);
	static IN_STD string toStr (long l, int width);
	static IN_STD string toStr (unsigned long ul);
	static IN_STD string toStr (unsigned long ul, int width);
	static IN_STD string toStr (long long l);
	static IN_STD string toStr (long long l, int width);
	static IN_STD string toStr (unsigned long long ul);
	static IN_STD string toStr (unsigned long long ul, int width);
	static IN_STD string toStr (float f);
	static IN_STD string toStr (float f, int precision, bool scientific = true);
	static IN_STD string toStr (
				float f, int precision, int width, bool scientific =true);
	static IN_STD string toStr (double d);
	static IN_STD string toStr (double d, int precision, bool scientific =true);
	static IN_STD string toStr (
				double d, int precision, int width, bool scientific =true);
	static IN_STD string toStr (long double d);
	static IN_STD string toStr (
				long double d, int precision, bool scientific =true);
	static IN_STD string toStr (
				long double d, int precision, int width, bool scientific =true);

	/**
	 * <P>La représentation <I>user friendly</I> d'un nombre décimale est
	 * définie comme suit :<BR>
	 * <OL>
	 * <LI>- Notation fixe si elle fait moins de <I>integerPartNum</I>
	 * caractères (séparateur décimale excepté).
	 * <LI>- Notation scientifique si la notation fixe n'est pas possible.
	 * </OL>
	 * </P>
	 * <P>Le nombre de chiffres significatifs est d'au plus <I>precision</I>
	 * caractères. L'objectif est ici d'avoir la représentation la plus courte
	 * possible.
	 * </P>
	 * <P>Un autre objectif de cet algorithme est d'éviter qu'un nombre "simple"
	 * en entrée devienne un nombre irrationnel (ex : 12.3 -> 12.99999).
	 * </P>
	 * @see	shortestRepresentation
	 */
	static std::string userRepresentation (
		double d, unsigned short precision, unsigned short integerPartNum = 1);

	/**
	 * <P>La représentation <I>user friendly</I> la plus courte d'un nombre
	 * décimale est définie comme suit :<BR>
	 * <OL>
	 * <LI>- Notation fixe si elle fait moins de <I>integerPartNum</I>
	 * caractères (séparateur décimale excepté).
	 * <LI>- Notation scientifique si la notation fixe n'est pas possible.
	 * </OL>
	 * </P>
	 * <P>Le nombre de chiffres significatifs est d'au plus <I>precision</I>
	 * caractères. L'objectif est ici d'avoir la représentation la plus courte
	 * possible. Les 0 inutiles sont supprimés.
	 * </P>
	 * <P>Un autre objectif de cet algorithme est d'éviter qu'un nombre "simple"
	 * en entrée devienne un nombre irrationnel (ex : 12.3 -> 12.99999).
	 * </P>
	 * @see	userRepresentation
	 */
	static std::string shortestRepresentation (
		double d, unsigned short precision, unsigned short integerPartNum = 1);


	private :

	/**
	 * Constructeurs, opérateur = et destructeur : interdits.
	 */
	NumericConversions ( );
	NumericConversions (const NumericConversions&);
	NumericConversions& operator = (const NumericConversions&);
	~NumericConversions ( );
};	// class NumericConversions

END_NAMESPACE_UTIL


#endif	// NUMERIC_CONVERSIONS_H

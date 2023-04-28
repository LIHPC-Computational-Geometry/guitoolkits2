/**
 * \file		NumericServices.h
 * \author		Charles PIGNEROL
 * \date		13/09/2013
 */

#include <TkUtil/util_config.h>


#ifndef NUMERIC_H
#define NUMERIC_H

#include <limits>
#include <numeric>
#include <string>


BEGIN_NAMESPACE_UTIL

/**
 * \brief	Gestion des constantes numériques du projet, aussi bien pour
 * 			les aspects calculs que pour ceux liés à l'IHM.
 */
class NumericServices
{
	public :


	/**
	 * Les <I>double</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (double)</I>).
	 */
	static const size_t			doubleSize;

	/**
	 * Epsilon machine.
	 */
	static double doubleMachEpsilon ( )
	{ return std::numeric_limits<double>::epsilon ( ); }

	/** Min/Max machine.
	 * \warning	<B>ATTENTION :</B> la valeur minimale est la plus petite valeur
	 *			positive possible.
	 */
	static double doubleMachMin ( )
	{ return std::numeric_limits<double>::min ( ); }
	static double doubleMachMax ( )
	{ return std::numeric_limits<double>::max ( ); }

	/** L'infini. */
	static double doubleMachInfinity ( )
	{ return std::numeric_limits<double>::infinity ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * double garantie). */
	static const size_t			doubleDigitsMax;

	/**
	 * \return		<I>true</I> si l'argument est valide (au sens machine),
	 * 				<I>false</I> dans le cas contraire.
	 * \see			isValidAndNotMax
	 */
	static bool isValid (double);

	/**
	 * \return		<I>true</I> si l'argument est valide (au sens machine)
	 * 				et non égal à une valeur extrèmale,
	 * 				<I>false</I> dans le cas contraire.
	 * \see			isValid
	 * \see			doubleMachMin
	 * \see			doubleMachMax
	 */
	static bool isValidAndNotMax (double);

	/**
	 * Le nombre maximum de caractères d'un double en notation scientifique.
	 * Conversion chaine vers double non garantie.
	 */
	static const size_t			doubleScientificNotationCharMax;	

	//@}	//  Les double


	/**
	 * Les <I>long double</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (long double)</I>).
	 */
	static const size_t			longDoubleSize;

	/**
	 * Epsilon machine.
	 */
	static long double longDoubleMachEpsilon ( )
	{ return std::numeric_limits<long double>::epsilon ( ); }

	/** Min/Max machine.
	 * \warning	<B>ATTENTION :</B> la valeur minimale est la plus petite valeur
	 *			positive possible.
	 */
	static long double longDoubleMachMin ( )
	{ return std::numeric_limits<long double>::min ( ); }
	static long double longDoubleMachMax ( )
	{ return std::numeric_limits<long double>::max ( ); }

	/** L'infini. */
	static long double longDoubleMachInfinity ( )
	{ return std::numeric_limits<long double>::infinity ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers long
	 * double garantie). */
	static const size_t			longDoubleDigitsMax;

	/**
	 * \return		<I>true</I> si l'argument est valide (au sens machine),
	 * 				<I>false</I> dans le cas contraire.
	 * \see			isValidAndNotMax
	 */
	static bool isValid (long double);

	/**
	 * \return		<I>true</I> si l'argument est valide (au sens machine)
	 * 				et non égal à une valeur extrèmale,
	 * 				<I>false</I> dans le cas contraire.
	 * \see			isValid
	 * \see			longDoubleMachMin
	 * \see			longDoubleMachMax
	 */
	static bool isValidAndNotMax (long double);

	/**
	 * Le nombre maximum de caractères d'un double en notation scientifique.
	 * Conversion chaine vers long double non garantie.
	 */
	static const size_t			longDoubleScientificNotationCharMax;	

	//@}	//  Les long double


	/**
	 * Les <I>float</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (float)</I>).
	 */
	static const size_t			floatSize;

	/**
	 * Epsilon machine.
	 */
	static float floatMachEpsilon ( )
	{ return std::numeric_limits<float>::epsilon ( ); }

	/** Min/Max machine.
	 * \warning	<B>ATTENTION :</B> la valeur minimale est la plus petite valeur
	 *			positive possible.
	 */
	static float floatMachMin ( )
	{ return std::numeric_limits<float>::min ( ); }
	static float floatMachMax ( )
	{ return std::numeric_limits<float>::max ( ); }

	/** L'infini. */
	static float floatMachInfinity ( )
	{ return std::numeric_limits<float>::infinity ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * float garantie). */
	static const size_t			floatDigitsMax;

	/**
	 * \return		<I>true</I> si l'argument est valide (au sens machine),
	 * 				<I>false</I> dans le cas contraire.
	 * \see			isValidAndNotMax
	 */
	static bool isValid (float);

	/**
	 * \return		<I>true</I> si l'argument est valide (au sens machine)
	 * 				et non égal à une valeur extrèmale,
	 * 				<I>false</I> dans le cas contraire.
	 * \see			isValid
	 * \see			floatMachMin
	 * \see			floatMachMax
	 */
	static bool isValidAndNotMax (float);

	/**
	 * Le nombre maximum de caractères d'un float en notation scientifique.
	 * Conversion chaine vers float non garantie.
	 */
	static const size_t			floatScientificNotationCharMax;	

	//@}	//  Les float


	/**
	 * Les <I>char</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (char)</I>).
	 */
	static const size_t			charSize;

	/** Min/Max machine. */
	static char charMachMin ( )
	{ return std::numeric_limits<char>::min ( ); }
	static char charMachMax ( )
	{ return std::numeric_limits<char>::max ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * char garantie). */
	static const size_t			charDigitsMax;

	//@}	//  Les char

	/**
	 * Les <I>unsigned char</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (unsigned char)</I>).
	 */
	static const size_t			unsignedCharSize;

	/** Min/Max machine. */
	static unsigned char unsignedCharMachMin ( )
	{ return std::numeric_limits<unsigned char>::min ( ); }
	static unsigned char unsignedCharMachMax ( )
	{ return std::numeric_limits<unsigned char>::max ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * unsigned char garantie). */
	static const size_t			unsignedCharDigitsMax;

	//@}	//  Les unsigned char


	/**
	 * Les <I>wchar_t</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (wchar_t)</I>).
	 */
	static const size_t			wcharSize;

	/** Min/Max machine. */
	static wchar_t wcharMachMin ( )
	{ return std::numeric_limits<wchar_t>::min ( ); }
	static wchar_t wcharMachMax ( )
	{ return std::numeric_limits<wchar_t>::max ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * wchar_t garantie). */
	static const size_t			wcharDigitsMax;

	//@}	//  Les wchar_t


	/**
	 * Les <I>short</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (short)</I>).
	 */
	static const size_t			shortSize;

	/** Min/Max machine. */
	static short shortMachMin ( )
	{ return std::numeric_limits<short>::min ( ); }
	static short shortMachMax ( )
	{ return std::numeric_limits<short>::max ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * short garantie). */
	static const size_t			shortDigitsMax;

	//@}	//  Les short

	/**
	 * Les <I>unsigned short</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (unsigned short)</I>).
	 */
	static const size_t			unsignedShortSize;

	/** Min/Max machine. */
	static unsigned short unsignedShortMachMin ( )
	{ return std::numeric_limits<unsigned short>::min ( ); }
	static unsigned short unsignedShortMachMax ( )
	{ return std::numeric_limits<unsigned short>::max ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * unsigned short garantie). */
	static const size_t			unsignedShortDigitsMax;

	//@}	//  Les unsigned short


	/**
	 * Les <I>int</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (int)</I>).
	 */
	static const size_t			intSize;

	/** Min/Max machine. */
	static int intMachMin ( )
	{ return std::numeric_limits<int>::min ( ); }
	static int intMachMax ( )
	{ return std::numeric_limits<int>::max ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * int garantie). */
	static const size_t			intDigitsMax;

	//@}	//  Les int

	/**
	 * Les <I>unsigned int</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (unsigned int)</I>).
	 */
	static const size_t			unsignedIntSize;

	/** Min/Max machine. */
	static unsigned int unsignedIntMachMin ( )
	{ return std::numeric_limits<unsigned int>::min ( ); }
	static unsigned int unsignedIntMachMax ( )
	{ return std::numeric_limits<unsigned int>::max ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * unsigned int garantie). */
	static const size_t			unsignedIntDigitsMax;

	//@}	//  Les unsigned int


	/**
	 * Les <I>long</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (long/I>).
	 */
	static const size_t			longSize;

	/** Min/Max machine. */
	static long longMachMin ( )
	{ return std::numeric_limits<long>::min ( ); }
	static long longMachMax ( )
	{ return std::numeric_limits<long>::max ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * long garantie). */
	static const size_t			longDigitsMax;

	//@}	//  Les long

	/**
	 * Les <I>unsigned long</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (unsigned long)</I>).
	 */
	static const size_t			unsignedLongSize;

	/** Min/Max machine. */
	static unsigned long unsignedLongMachMin ( )
	{ return std::numeric_limits<unsigned long>::min ( ); }
	static unsigned long unsignedLongMachMax ( )
	{ return std::numeric_limits<unsigned long>::max ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * unsigned long garantie). */
	static const size_t			unsignedLongDigitsMax;

	//@}	//  Les unsigned long


	/**
	 * Les <I>long long</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (long long)</I>).
	 */
	static const size_t			longLongSize;

	/** Min/Max machine. */
	static long long longLongMachMin ( )
	{ return std::numeric_limits<long long>::min ( ); }
	static long long longLongMachMax ( )
	{ return std::numeric_limits<long long>::max ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * long long garantie). */
	static const size_t			longLongDigitsMax;

	//@}	//  Les long long

	/**
	 * Les <I>unsigned long long</I>
	 */
	//@{

	/**
	 * La taille en octets (<I>sizeof (unsigned long long)</I>).
	 */
	static const size_t			unsignedLongLongSize;

	/** Min/Max machine. */
	static unsigned long long	unsignedLongLongMachMin ( )
	{ return std::numeric_limits<unsigned long long>::min ( ); }
	static unsigned long long	unsignedLongLongMachMax ( )
	{ return std::numeric_limits<unsigned long long>::max ( ); }

	/** Le nombre max de chiffres significatifs (conversion chaine vers
	 * unsigned long long garantie). */
	static const size_t			unsignedLongLongDigitsMax;

	//@}	//  Les unsigned long long

};	// class NumericServices

END_NAMESPACE_UTIL


#endif	// NUMERIC_H

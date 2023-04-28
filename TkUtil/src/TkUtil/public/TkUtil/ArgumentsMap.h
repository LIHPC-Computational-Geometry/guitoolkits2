#ifndef ARGUMENTS_MAP_H
#define ARGUMENTS_MAP_H


#include <map>
#include <set>
#include <string>
#include <vector>


namespace TkUtil
{

/**
 * <P>
 * Classe récupérant et stockant les arguments (et leur valeur optionnelle) du main.
 * </P>
 * <P><OL>Un argument avec valeur est du type :<BR>
 * <LI>[-]name=value<BR>
 * ou
 * <LI>-[-]name value
 * </OL>
 * <P>Les arguments sont, dans l'ensemble, multi ou mono-instanciables. Dans le cas
 * où </P>
 */
class ArgumentsMap
{
	public :

	/**
	 * Constructeurs.
	 * @param	<I>multi</I> doit valoir <I>true</I> si un même argument peut avoir plusieurs valeurs,
	 * 		<I>false</I> s'ils doivent être uniques, auquel cas la dernière valeur écrase la précédente.
	 */
	ArgumentsMap (bool multi = true);
	ArgumentsMap (int argc, const char* argv[], bool multi = true);
	ArgumentsMap (const ArgumentsMap&);
	ArgumentsMap& operator = (const ArgumentsMap&);
	virtual ~ArgumentsMap ( );

	/**
	 * Ajoute à la liste des arguments autorisés ceux transmis en arguments.
	 * @see		parseArgs
	 */
	virtual void allowsArgs (const std::vector<std::string>& args);

	/**
	 * Récupération des arguments transmis en arguments et ajout à ceux déjà
	 * recensés.
	 * @param	Nombre d'arguments
	 * @param	Arguments
	 * @param	si <I>true</I> lève une exception si un ou plusieurs arguments
	 *			ne sont pas dans la liste des arguments autorisés.
	 * @see		allowsArgs
	 */
	virtual void parseArgs (int argc, const char* argv [], bool raises = false);

	/**
	 * @return	<I>multi</I> doit valoir <I>true</I> si un même argument peut avoir plusieurs valeurs,
	 * 		<I>false</I> s'ils doivent être uniques, auquel cas la dernière valeur écrase la précédente.
	 */
	virtual bool allowMultipleTimes ( ) const
	{ return _multi; }

	/**
	 * @return	<I>true</I> si l'argument transmis est référencé, <I>false</I> dans le cas contraire.
	 */
	virtual bool hasArg (const std::string& arg) const;

	/**
	 * Fonctions de récupération d'arguments.
	 * @param	<I>arg</I> est le nom de l'argument (ex : -opt)
	 * @param	<I>def</I> est la valeur par défaut (quand l'argument n'est pas recensé, c.a.d. transmis à <I>parseArgs</I>)
	 * @param	<I>raises</I> doit valoir <I>true</I> pour qu'une exception soit levée si l'argument n'est pas recensé,
	 *		<I>false</I> si l'on veut qu'en l'absence de recensement ce soit la valeur par défaut qui soit retournée.
	 */
	virtual std::string getArgValue (const std::string& arg, const std::string def = std::string (""), bool raises = true) const;
	virtual std::vector<std::string> getArgValues (const std::string& arg, bool raises = true) const;
	virtual long getArgLongValue (const std::string& arg, const std::string def = std::string (""), bool raises = true) const;
	virtual std::vector<long> getArgLongValues (const std::string& arg, bool raises = true) const;
	virtual unsigned long getArgULongValue (const std::string& arg, const std::string def = std::string (""), bool raises = true) const;
	virtual std::vector<unsigned long> getArgULongValues (const std::string& arg, bool raises = true) const;
	virtual double getArgDoubleValue (const std::string& arg, const std::string def = std::string (""), bool raises = true) const;
	virtual std::vector<double> getArgDoubleValues (const std::string& arg, bool raises = true) const;


	private :

	std::multimap< std::string, std::string >	_arguments;
	std::set<std::string>						_allowedArgs;
	bool										_multi;
};	// class ArgumentsMap

}	// namespace TkUtil


#endif	// ARGUMENTS_MAP_H

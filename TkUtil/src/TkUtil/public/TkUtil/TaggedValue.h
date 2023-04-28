#ifndef TAGGED_VALUE_H
#define TAGGED_VALUE_H

#include <TkUtil/util_config.h>


#include <string>
#include <iostream>
#include <vector>


BEGIN_NAMESPACE_UTIL


/**
 * <P>Cette classe représente une valeur identifiée par une balise (<I>tag</I>)
 * et un nom. Les instances de cette classe peuvent être caractérisées par des
 * attributs optionnels et de type indéfini : c'est à l'application que revient
 * en charge la gestion du typage.
 * </P>
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class TaggedValue
{
	public :

	/**
	 * Constructeur.
	 * @param		balise de l'instance
	 * @param		nom de l'instance
	 * @exception	Une exception est levée si la balise ou le nom est nul.
	 * @warning		Les valeurs tranmises par défaut sont là pour être agréables
	 *				à <I>swig</I> qui utilise après l'opérateur =.
	 */
	TaggedValue (const IN_STD string& tag = "Invalid tag",
	             const IN_STD string& name = "Invalid name");

	/**
	 * Constructeur.
	 * @param		flux entrant au format texte dans lequel sont lus la balise,
	 *				le nom de l'instance et les éventuels attributs.
	 * @exception	Une exception est levée si l'instance ne peut pas lire la
	 *				balise ou le nom de l'instance.
	 */
	TaggedValue (IN_STD istream& stream);

	/**
	 * Constructeurs de copie et opérateur = : RAS.	
	 */
	TaggedValue (const TaggedValue&);
	TaggedValue& operator = (const TaggedValue&);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~TaggedValue ( );

	/**
	 * @return		La balise associée à l'instance.
	 */
	virtual IN_STD string getTag ( ) const
	{ return _tag; }

	/**
	 * @return		Le nom de l'instance.
	 */
	virtual IN_STD string getName ( ) const
	{ return _name; }

	/**
	 * @return		<I>true</I> si des attributs sont associés à l'instance,
	 *				<I>false</I> dans le cas contraire.
	 */
	virtual bool hasAttibutes ( ) const
	{ return 0 == _attributes.size ( ) ? false : true; }

	/**
	 * @return		Le nombre d'attributs associés à l'instance.
	 */
	virtual size_t getAttibuteCount ( ) const
	{ return _attributes.size ( ); }

	/**
	 * @return		La liste des éventuels attributs associés à l'instance.
	 */
	virtual const IN_STD vector < IN_STD string >& getAttributes ( ) const
	{ return _attributes; }

	/**
	 * @param		Attribut à ajouter à la liste des attributs.
	 */
	virtual void addAttribute (const IN_STD string& attr);

	/**
	 * @return		La valeur balisée sous forme de chaine de caractère
	 *				<I>balise nom [attribut_1 ... attribut_n]</I>.
	 */
	virtual IN_STD string toString ( ) const;

	/**
	 * @return		<I>true</I> si les deux instances sont égales, <I>false</I>
	 *				dans le cas contraire.
	 */
	virtual bool operator == (const TaggedValue& tv);

	/**
	 * @return		<I>true</I> si les deux instances sont différentes,
	 *				<I>false</I> dans le cas contraire.
	 */
	virtual bool operator != (const TaggedValue& tv)
	{ return !(*this == (tv)); }


	private :

	/** La balise et le nom de l'instance. */
	IN_STD	string						_tag, _name;

	/** Les éventuels attributs associés à l'instance. */
	IN_STD vector < IN_STD string >		_attributes;
};	// class TaggedValue


END_NAMESPACE_UTIL


#endif	// TAGGED_VALUE_H

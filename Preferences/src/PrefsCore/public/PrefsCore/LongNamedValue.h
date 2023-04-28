#ifndef LONG_NAMED_VALUE_H
#define LONG_NAMED_VALUE_H

#include <PrefsCore/NamedValue.h>



BEGIN_NAMESPACE_PREFS


/**
 * Classe représentant une valeur nommée de type long.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class LongNamedValue : public NamedValue 
{
	public :

	/**
	 * Constructeur.
	 * @param		Nom de l'élément.
	 * @param		Valeur de l'élément.
	 * @param		Eventuel commentaire associé à la valeur nommée.
	 * @param		Caractère surchargeable de l'élément.
	 * @param		Caractère enregistrable de l'élément.
	 */
	LongNamedValue (const IN_UTIL UTF8String name, long value = 0, const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ),
	                bool overloadable = true, bool safeguardable = true)
		: NamedValue (name, comment, overloadable, safeguardable), _value (value)
	{ }

	/**
	 * Constructeur de copie.
	 * @param		Elément copié.
	 */
	LongNamedValue (const LongNamedValue& namedValue)
		: NamedValue (namedValue), _value (namedValue._value)
	{ }

	/**
	 * Opérateur =.
	 */
	LongNamedValue& operator = (const LongNamedValue& namedValue);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~LongNamedValue ( )
	{ }

	/**
	 * @return		Une copie de l'instance.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	virtual Element* clone ( ) const;

	/**
	 * @return		La valeur associée sous forme de chaine de caractères.
	 */
	virtual IN_STD string getStrValue ( ) const;

	/**
	 * @param		Nouvelle valeur, sous forme de chaine de caractères.
	 * @exception	Lève une exception si la valeur est incorrecte ou non surchargeable.
	 */
	virtual void setStrValue (const IN_STD string& value);

	/**
	 * @return		"long".
	 */
	virtual IN_STD string getType ( ) const
	{ return typeName; }

	/**
	 * @return		La valeur associée
	 */
	virtual long getValue ( ) const
	{ return _value; }
	virtual operator long ( ) const
	{ return _value; }

	/**
	 * @param		Nouvelle valeur associée
	 * @exception	Lève une exception si la valeur est incorrecte ou non surchargeable.
	 */
	virtual void setValue (long value);

	/**
	 * Le nom du type de valeur nommée.
	 */
	static const IN_STD string		typeName;


	protected :

	/**
	 * Effectue une copie de l'element reçu en argument.
	 * @param		Element à copier.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	void copy (const LongNamedValue& value);


	private :

	/** La valeur associée. */
	long			_value;
};	// class LongNamedValue


END_NAMESPACE_PREFS

#endif	// LONG_NAMED_VALUE_H

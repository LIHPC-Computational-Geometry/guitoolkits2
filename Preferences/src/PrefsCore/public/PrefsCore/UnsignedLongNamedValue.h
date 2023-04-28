#ifndef UNSIGNED_LONG_NAMED_VALUE_H
#define UNSIGNED_LONG_NAMED_VALUE_H

#include <PrefsCore/NamedValue.h>



BEGIN_NAMESPACE_PREFS


/**
 * Classe représentant une valeur nommée de type unsigned long.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class UnsignedLongNamedValue : public NamedValue 
{
	public :

	/**
	 * Constructeur.
	 * @param		Nom de l'élément.
	 * @param		Valeur de l'élément.
	 * @param		Eventuel commentaire associé à la valeur nommée.
	 */
	UnsignedLongNamedValue (const IN_UTIL UTF8String& name, unsigned long value = 0,const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ),
	                        bool overloadable = true, bool safeguardable = true)
		: NamedValue (name, comment, overloadable, safeguardable), _value (value)
	{ }

	/**
	 * Constructeur de copie.
	 * @param		Elément copié.
	 */
	UnsignedLongNamedValue (const UnsignedLongNamedValue& namedValue)
		: NamedValue (namedValue), _value (namedValue._value)
	{ }

	/**
	 * Opérateur =.
	 */
	UnsignedLongNamedValue& operator = (const UnsignedLongNamedValue& value);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~UnsignedLongNamedValue ( )
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
	 * @return		"unsigned long".
	 */
	virtual IN_STD string getType ( ) const
	{ return typeName; }

	/**
	 * @return		La valeur associée
	 */
	virtual unsigned long getValue ( ) const
	{ return _value; }
	virtual operator unsigned long ( ) const
	{ return _value; }

	/**
	 * @param		Nouvelle valeur associée
	 * @exception	Lève une exception si la valeur est incorrecte ou non surchargeable.
	 */
	virtual void setValue (unsigned long value);

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
	void copy (const UnsignedLongNamedValue& elem);


	private :

	/** La valeur associée. */
	unsigned long			_value;
};	// class UnsignedLongNamedValue


END_NAMESPACE_PREFS

#endif	// UNSIGNED_LONG_NAMED_VALUE_H

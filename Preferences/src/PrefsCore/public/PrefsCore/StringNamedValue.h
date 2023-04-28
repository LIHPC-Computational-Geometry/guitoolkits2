#ifndef STRING_NAMED_VALUE_H
#define STRING_NAMED_VALUE_H

#include <PrefsCore/NamedValue.h>



BEGIN_NAMESPACE_PREFS


/**
 * Classe représentant une valeur nommée de type chaine de caractères.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class StringNamedValue : public NamedValue 
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
	StringNamedValue (const IN_UTIL UTF8String& name, const IN_UTIL UTF8String& value = IN_UTIL UTF8String ( ),
			const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true)
		: NamedValue (name, comment, overloadable, safeguardable), _value (value)
	{ }

	/**
	 * Constructeur de copie.
	 * @param		Elément copié.
	 */
	StringNamedValue (const StringNamedValue& namedValue)
		: NamedValue (namedValue), _value (namedValue._value)
	{ }

	/**
	 * Opérateur =.
	 */
	StringNamedValue& operator = (const StringNamedValue& namedValue);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~StringNamedValue ( )
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
	 * @exception	Lève une exception si la valeur n'est pas surchargeable.
	 */
	virtual void setStrValue (const IN_STD string& value);

	/**
	 * @return		"string".
	 */
	virtual IN_STD string getType ( ) const
	{ return typeName; }

	/**
	 * @return		La valeur associée
	 */
	virtual const IN_UTIL UTF8String& getValue ( ) const
	{ return _value; }

	/**
	 * @param		Nouvelle valeur associée
	 * @exception	Lève une exception si la valeur n'est pas surchargeable.
	 */
	virtual void setValue (const IN_UTIL UTF8String& value);

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
	void copy (const StringNamedValue& value);


	private :

	/** La valeur associée. */
	IN_UTIL UTF8String			_value;
};	// class StringNamedValue


END_NAMESPACE_PREFS

#endif	// STRING_NAMED_VALUE_H

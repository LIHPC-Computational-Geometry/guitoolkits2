#ifndef BOOL_NAMED_VALUE_H
#define BOOL_NAMED_VALUE_H

#include <PrefsCore/NamedValue.h>



BEGIN_NAMESPACE_PREFS


/**
 * Classe représentant une valeur nommée de type bool.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class BoolNamedValue : public NamedValue 
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
	BoolNamedValue (const IN_UTIL UTF8String& name, bool value = false, const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ),
	                bool overloadable = true, bool safeguardable = true)
		: NamedValue (name, comment, overloadable, safeguardable), _value (value)
	{ }

	/**
	 * Constructeur de copie.
	 * @param		Elément copié.
	 */
	BoolNamedValue (const BoolNamedValue& namedValue)
		: NamedValue (namedValue), _value (namedValue._value)
	{ }

	/**
	 * Opérateur =.
	 */
	BoolNamedValue& operator = (const BoolNamedValue& namedValue);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~BoolNamedValue ( )
	{ }

	/**
	 * @return		Une copie de l'instance.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	virtual Element* clone ( ) const;

	/**
	 * @return		La valeur associée sous forme de chaine de caractères
	 *				(trueStr ou falseStr).
	 */
	virtual IN_STD string getStrValue ( ) const;

	/**
	 * @param		Nouvelle valeur, sous forme de chaine de caractères.
	 * @exception	Lève une exception si la valeur est incorrecte ou non surchargeable.
	 */
	virtual void setStrValue (const IN_STD string& value);

	/**
	 * @return		"bool".
	 */
	virtual IN_STD string getType ( ) const
	{ return typeName; }

	/**
	 * @return		La valeur associée
	 */
	virtual bool getValue ( ) const
	{ return _value; }
	virtual operator bool ( ) const
	{ return _value; }

	/**
	 * @param		Nouvelle valeur associée
	 * @exception	Lève une exception si la valeur est incorrecte ou non surchargeable.
	 */
	virtual void setValue (bool value);

	/**
	 * Le nom du type de valeur nommée.
	 */
	static const IN_STD string		typeName;

	/**
	 * Les valeurs true et false, sous forme de chaine de caractères.
	 */
	static const IN_STD string		trueStr, falseStr;


	protected :

	/**
	 * Effectue une copie de l'element reçu en argument.
	 * @param		Element à copier.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	void copy (const BoolNamedValue& elem);


	private :

	/** La valeur associée. */
	bool			_value;
};	// class BoolNamedValue


END_NAMESPACE_PREFS

#endif	// BOOL_NAMED_VALUE_H

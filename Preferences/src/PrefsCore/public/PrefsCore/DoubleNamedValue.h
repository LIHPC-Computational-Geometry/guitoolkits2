#ifndef DOUBLE_NAMED_VALUE_H
#define DOUBLE_NAMED_VALUE_H

#include <PrefsCore/NamedValue.h>



BEGIN_NAMESPACE_PREFS


/**
 * Classe représentant une valeur nommée de type double.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class DoubleNamedValue : public NamedValue 
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
	DoubleNamedValue (const IN_UTIL UTF8String& name, double value = 0., const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ),
	                  bool overloadable = true, bool safeguardable = true)
		: NamedValue (name, comment, overloadable, safeguardable), _value (value)
	{ }

	/**
	 * Constructeur de copie.
	 * @param		Elément copié.
	 */
	DoubleNamedValue (const DoubleNamedValue& namedValue)
		: NamedValue (namedValue), _value (namedValue._value)
	{ }

	/**
	 * Opérateur =.
	 */
	DoubleNamedValue& operator = (const DoubleNamedValue& namedValue);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~DoubleNamedValue ( )
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
	 * @exception	Lève une exception si la valeur est incorrecte ou si l'élément n'est pas modifiable.
	 */
	virtual void setStrValue (const IN_STD string& value);

	/**
	 * @return		"double".
	 */
	virtual IN_STD string getType ( ) const
	{ return typeName; }

	/**
	 * @return		La valeur associée
	 */
	virtual double getValue ( ) const
	{ return _value; }
	virtual operator double ( ) const
	{ return _value; }

	/**
	 * @param		Nouvelle valeur associée
	 * @exception	Lève une exception si la valeur est incorrecte ou non surchargeable.
	 */
	virtual void setValue (double value);

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
	void copy (const DoubleNamedValue& dValue);


	private :

	/** La valeur associée. */
	double			_value;
};	// class DoubleNamedValue


END_NAMESPACE_PREFS

#endif	// DOUBLE_NAMED_VALUE_H

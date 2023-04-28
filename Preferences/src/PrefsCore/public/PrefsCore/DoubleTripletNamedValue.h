#ifndef DOUBLE_TRIPLET_NAMED_VALUE_H
#define DOUBLE_TRIPLET_NAMED_VALUE_H

#include <PrefsCore/NamedValue.h>



BEGIN_NAMESPACE_PREFS


/**
 * Classe représentant une valeur nommée de type triplet de double.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class DoubleTripletNamedValue : public NamedValue 
{
	public :

	/**
	 * Constructeur.
	 * @param		Nom de l'élément.
	 * @param		Composantes de la valeur de l'élément.
	 * @param		Eventuel commentaire associé à la valeur nommée.
	 * @param		Caractère surchargeable de l'élément.
	 * @param		Caractère enregistrable de l'élément.
	 */
	DoubleTripletNamedValue (const IN_UTIL UTF8String& name,  double x = 0., double y = 0., double z = 0.,
			const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true)
		: NamedValue (name, comment, overloadable, safeguardable), _x (x), _y (y), _z (z)
	{ }

	/**
	 * Constructeur de copie.
	 * @param		Elément copié.
	 */
	DoubleTripletNamedValue (const DoubleTripletNamedValue& namedValue)
		: NamedValue (namedValue), _x (namedValue._x), _y (namedValue._y), _z (namedValue._z)
	{ }

	/**
	 * Opérateur =.
	 */
	DoubleTripletNamedValue& operator = (const DoubleTripletNamedValue& namedValue);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~DoubleTripletNamedValue ( )
	{ }

	/**
	 * @return		Une copie de l'instance.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	virtual Element* clone ( ) const;

	/**
	 * @return		La valeur associée sous forme de chaine de caractères "(x, y, z)".
	 */
	virtual IN_STD string getStrValue ( ) const;

	/**
	 * @param		Nouvelle valeur, sous forme de chaine de caractères "(x, y, z)".
	 * @exception	Lève une exception si la valeur est incorrecte ou non surchargeable.
	 */
	virtual void setStrValue (const IN_STD string& value);

	/**
	 * @return		"doubleTriplet".
	 */
	virtual IN_STD string getType ( ) const
	{ return typeName; }

	/**
	 * @return		Les composantes de la valeur associée
	 */
	virtual double getX ( ) const
	{ return _x; }
	virtual double getY ( ) const
	{ return _y; }
	virtual double getZ ( ) const
	{ return _z; }

	/**
	 * @param		Nouvelle valeur associée
	 * @exception	Lève une exception si la valeur est incorrecte ou non surchargeable.
	 */
	virtual void setValue (double x, double y, double z);

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
	void copy (const DoubleTripletNamedValue& dtnv);


	private :

	/** La valeur associée. */
	double			_x, _y, _z;
};	// class DoubleTripletNamedValue


END_NAMESPACE_PREFS

#endif	// DOUBLE_TRIPLET_NAMED_VALUE_H

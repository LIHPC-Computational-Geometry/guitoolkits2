#ifndef COLOR_NAMED_VALUE_H
#define COLOR_NAMED_VALUE_H

#include <PrefsCore/DoubleTripletNamedValue.h>



BEGIN_NAMESPACE_PREFS

/*
 * Classe représentant une valeur nommée de type couleur dont les trois
 * composantes RGB sont des doubles compris entre 0 et 1.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class ColorNamedValue : public DoubleTripletNamedValue 
{
	public :

	/**
	 * Constructeur.
	 * @param		Nom de l'élément.
	 * @param		Composantes RGB de la couleur. Ces composantes doivent être comprises entre 0 et 1.
	 * @param		Eventuel commentaire associé à la valeur nommée.
	 * @param		Caractère surchargeable de l'élément.
	 * @param		Caractère enregistrable de l'élément.
	 * @exception	Une exception est levée si une composante au moins n'est pas comprise entre 0 et 1.
	 */
	ColorNamedValue (const IN_UTIL UTF8String& name, double red = 0., double green = 0., double blue = 0.,
	                 const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true);

	/**
	 * Constructeur de copie.
	 * @param		Elément copié.
	 */
	ColorNamedValue (const ColorNamedValue& color)
		: DoubleTripletNamedValue (color)
	{ }

	/**
	 * Opérateur =.
	 */
	ColorNamedValue& operator = (const ColorNamedValue& color);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~ColorNamedValue ( )
	{ }

	/**
	 * @return		Une copie de l'instance.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	virtual Element* clone ( ) const;

	/**
	 * @param		Nouvelle couleur, sous forme de chaine de caractères "(r, g, b)".
	 * @exception	Lève une exception si la valeur est incorrecte ou non surchargeable.
	 */
	virtual void setStrValue (const IN_STD string& value);

	/**
	 * @return		"color".
	 */
	virtual IN_STD string getType ( ) const
	{ return typeName; }

	/**
	 * @return		Les composantes RGB de la valeur associée
	 */
	virtual double getRed ( ) const
	{ return getX ( ); }
	virtual double getGreen ( ) const
	{ return getY ( ); }
	virtual double getBlue ( ) const
	{ return getZ ( ); }

	/**
	 * @param		Nouvelle valeur associée
	 * @exception	Lève une exception si la valeur est incorrecte ou non surchargeable.
	 */
	virtual void setValue (double red, double green, double blue);

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
	void copy (const ColorNamedValue& color);

	/**
	 * @param			Composante à évaluer
	 * @param			Nom de la composante à évaluer
	 * @exception		Lève une exception si la composante RGB reçue en  argument n'est pas comprise entre 0 et 1.
	 */
	virtual void evaluateValue (double value, const IN_STD string& name);
};	// class ColorNamedValue


END_NAMESPACE_PREFS

#endif	// COLOR_NAMED_VALUE_H

#ifndef NAMED_VALUE_H
#define NAMED_VALUE_H

#include <PrefsCore/Element.h>



BEGIN_NAMESPACE_PREFS


/**
 * Classe abstraite représentant une valeur nommée de configuration.
 * Une valeur nommée est un couple nom/valeur.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class NamedValue : public Element 
{
	public :

	/**
	 * Destructeur : RAS.
	 */
	virtual ~NamedValue ( )
	{ }

	/**
	 * @return		La valeur associée sous forme de chaine de caractères.
	 */
	virtual IN_STD string getStrValue ( ) const = 0;

	/**
	 * @param		Nouvelle valeur, sous forme de chaine de caractères.
	 * @exception	Lève une exception si la valeur est incorrecte ou non surchargeable.
	 * @see			isOverloadable
	 */
	virtual void setStrValue (const IN_STD string& value) = 0;

	/**
	 * Affichage dans le flux reçu en argument de la valeur nommée.
	 * @param		Flux utilisé
	 * @param		Niveau d'indentation initial
	 */
	virtual void print (IN_STD ostream& stream, size_t indent = 0) const;


	protected :

	/**
	 * Constructeur.
	 * @param		Nom de l'élément.
	 * @param		Eventuel commentaire associé à la valeur nommée.
	 * @param		Caractère surchargeable de l'élément.
	 * @param		Caractère enregistrable de l'élément.
	 */
	NamedValue (const IN_UTIL UTF8String& name, const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true)
		: Element (name, comment, overloadable, safeguardable)
	{ }

	/**
	 * Constructeur de copie.
	 * @param		Elément copié.
	 */
	NamedValue (const NamedValue& namedValue)
		: Element (namedValue)
	{ }

	/**
	 * Opérateur =.
	 */
	NamedValue& operator = (const NamedValue& namedValue);

	/**
	 * Effectue une copie de l'element reçu en argument.
	 * @param		Element à copier.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	void copy (const NamedValue& value);
};	// class NamedValue


END_NAMESPACE_PREFS

#endif	// NAMED_VALUE_H

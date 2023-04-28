#ifndef SECTION_H
#define SECTION_H

#include <PrefsCore/prefs_config.h>
#include <PrefsCore/Element.h>
#include <PrefsCore/NamedValue.h>

#include <string>
#include <vector>


BEGIN_NAMESPACE_PREFS


/**
 * Classe abstraite représentant une section de la configuration.
 * Une section est composée d'éléments, pouvant être des sous-sections ou des valeurs nommees.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class Section : public Element
{
	public :

	/**
	 * Constructeur.
	 * @param		Nom de la section.
	 * @param		Eventuel commentaire associé à la section.
	 * @param		Caractère surchargeable de la section.
	 * @param		Caractère enregistrable de la section.
	 */
	Section (const IN_UTIL UTF8String& name, const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true);

	/**
	 * Constructeur de copie. RAS.
	 */
	Section (const Section&);

	/**
	 * Opérateur =.  RAS.
	 */
	Section& operator = (const Section& element);

	/**
	 * Destructeur : détruit ses éléments.
	 */
	virtual ~Section ( );

	/**
	 * @return		Une chaine de caractères décrivant le type l'élément.
	 */
	virtual IN_STD string getType ( ) const
	{ return typeName; }

	/**
	 * @return		true si l'élément est "imprimable" en caractères ASCII, false dans le cas contraire. Descendance prise en compte.
	 */
	virtual bool isAscii ( ) const;

	/**
	 * @return		true si l'élément est "imprimable" en caractères ISO 8859, false dans le cas contraire. Descendance prise en compte.
	 */
	virtual bool isIso ( ) const;

	/**
	 * @return		Une copie de l'instance.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	virtual Element* clone ( ) const;

	/**
	 * @return		true si l'élement est surchargeable, false dans le cas contraire.
	 * @exception	Lève une exception si l'élément n'est pas surchargeable et si raise vaut true.
	 * @see			isOverloadable
	 */
	virtual bool checkForModification (bool raise = true);	// v 5.7.0

	/**
	 * @return		true si l'élement est enregistrable, false dans le cas contraire.
	 * @exception	Lève une exception si l'élément n'est pas enregistrable et si raise vaut true.
	 * @see			isSafeguardable
	 */
	virtual bool checkForSafeguard (bool raise = true);		// v 5.7.0

	/**
	 * @return			true si l'instance a un élément du nom tranmsis en argument, ou false dans le cas contraire.
	 */
	virtual bool hasElement (const IN_UTIL UTF8String& name) const;

	/**
	 * @return			L'élément dont le nom est transmis en argument.
	 * @exception		Une exception est levée si l'instance n'a pas l'élément demandée.
	 */
	virtual Element& getElement (const IN_UTIL UTF8String& name) const;

	/**
	 * @return			true si l'instance a une sous-section du nom tranmsis en argument, ou false dans le cas contraire.
	 */
	virtual bool hasSection (const IN_UTIL UTF8String& name) const;

	/**
	 * Ajoute la section reçue en argument à la liste des sous-sections.
	 * La section ajoutée est adoptée, c'est à dire que cette instance se chargera de sa destruction.
	 * @exception		Une exception est levée si cette instance a déjà une sous-section de ce nom ou si elle n'est pas surchargeable.
	 */
	virtual void addSection (Section* section);

	/**
	 * Supprime la sous-section reçue en argument.
	 * @exception		Une exception est levée si l'instance n'a pas de sous-section à supprimer ou si elle n'est pas surchargeable.
	 */
	virtual void removeSection (const IN_UTIL UTF8String& name);

	/**
	 * @return			La sous-section du nom transmis en argument.
	 * @exception		Une exception est levée si l'instance n'a pas la sous-section demandée.
	 */
	virtual Section& getSection (const IN_UTIL UTF8String& name) const;

	/**
	 * @return		La liste des sous-sections.
	 */
	virtual IN_STD vector<Section*> getSections ( ) const;

	/**
	 * @return			true si l'instance a une valeur nommée du nom transmis en argument, ou false dans le cas contraire.
	 */
	virtual bool hasNamedValue (const IN_UTIL UTF8String& name) const;

	/**
	 * Ajoute la valeur nommée reçue en argument à la liste des valeurs nommées.
	 * La valeur nommée ajoutée est adoptée, c'est à dire que cette instance se chargera de sa destruction.
	 * @exception		Une exception est levée si cette instance a déjà une valeur nommée de ce nom ou si elle n'est pas surchargeable.
	 */
	virtual void addNamedValue (NamedValue* namedValue);

	/**
	 * Supprime la valeur nommée reçue en argument.
	 * @exception		Une exception est levée si l'instance n'a pas de valeur nommée à supprimer ou si elle n'est pas surchargeable.
	 */
	virtual void removeNamedValue (const IN_UTIL UTF8String& name);

	/**
	 * @return			La valeur nommée du nom transmis en argument.
	 * @exception		Une exception est levée si l'instance n'a pas la valeur nommée demandée.
	 */
	virtual NamedValue& getNamedValue (const IN_UTIL UTF8String& name) const;

	/**
	 * @return		La liste des valeurs nommées.
	 */
	virtual IN_STD vector<NamedValue*> getNamedValues ( ) const;

	/**
	 * Renomme un élément.
	 * @param		Nom de l'élément à renommer.
	 * @param		Nouveau nom de l'élément.
	 * @exception	Une exception est levée si l'opération est impossible.
	 */
	virtual void renameElement (const IN_UTIL UTF8String& name, const IN_UTIL UTF8String& newName);

	/**
	 * @param		Nouveau nom de la section.
	 * @exception	Une exception est levée si cette section a un parent (utiliser alors renameElement sur le parent) ou si elle n'est pas surchargeable.
	 */
	virtual void rename (const IN_UTIL UTF8String& name);

	/**
	 * Affichage dans le flux reçu en argument de la section et de sa descendance.
	 * @param		Flux utilisé
	 * @param		Niveau d'indentation initial
	 */
	virtual void print (IN_STD ostream& stream, size_t indent = 0) const;


	protected :

	/**
	 * Effectue une copie de l'element reçu en argument.
	 * @param		Element à copier.
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	void copy (const Section& section);


	private :

	/**
	 * Le nom du type des instances de cette classe.
	 */
	static const IN_STD string		typeName;

	/** Les sous-sections. */
	IN_STD vector<Section*>			_sections;

	/** Les valeurs nommées. */
	IN_STD vector<NamedValue*>		_namedValues;
};	// class Section


END_NAMESPACE_PREFS

#endif	// SECTION_H

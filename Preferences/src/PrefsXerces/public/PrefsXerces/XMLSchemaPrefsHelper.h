#ifndef XML_SCHEMA_PREFS_HELPER_H
#define XML_SCHEMA_PREFS_HELPER_H

#include <PrefsCore/prefs_config.h>
#include <PrefsCore/Element.h>
#include <TkUtil/UTF8String.h>

#include <string>


BEGIN_NAMESPACE_PREFS


/**
 * Classe utilitaire de conversions de types "préférences" vers types XML 
 * schema.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class XMLSchemaPrefsHelper
{
	public :

	/**
	 * @return		Le type XML schema correspondant au type de l'élément reçu 
	 *				en argument. Ce type est destiné à figurer comme valeur
	 *				de l'attribut <I>type</I> de l'élément.
	 * @throw		Une exception si le type XML schema est inconnu.
	 */
	static IN_STD string elementToXMLschemaType (const Element& element);

	/**
	 * @return		Le type XML schema correspondant au type de l'élément reçu
	 *				en argument. Ce type est destiné à nommer le type de chaque
	 *				instance : <Type name="nom" type="string">Valeur</Type>.
	 */
	static IN_UTIL UTF8String elementToXMLschemaTypeName (const Element& element);

	private :

	/**
	 * Constructeurs, opérateur = et destructeur interdits.
	 */
	XMLSchemaPrefsHelper ( );
	XMLSchemaPrefsHelper (const XMLSchemaPrefsHelper&);
	XMLSchemaPrefsHelper& operator = (const XMLSchemaPrefsHelper&);
	~XMLSchemaPrefsHelper ( );
};	// class XMLSchemaPrefsHelper



END_NAMESPACE_PREFS

#endif		// XML_SCHEMA_PREFS_HELPER_H

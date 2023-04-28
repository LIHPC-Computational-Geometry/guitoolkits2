#ifndef PREFERENCES_HELPER_H
#define PREFERENCES_HELPER_H

#include <PrefsCore/prefs_config.h>
#include <PrefsCore/Section.h>
#include <PrefsCore/BoolNamedValue.h>
#include <PrefsCore/StringNamedValue.h>
#include <PrefsCore/ColorNamedValue.h>
#include <PrefsCore/LongNamedValue.h>
#include <PrefsCore/UnsignedLongNamedValue.h>
#include <PrefsCore/DoubleNamedValue.h>
#include <PrefsCore/DoubleTripletNamedValue.h>

#include <string>


BEGIN_NAMESPACE_PREFS


/**
 * Classe utilitaire regroupant certaines actions utiles à la gestion
 * de préférences.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class PreferencesHelper
{
	public :

	/**
	 * @param		Parent de la section recherchée
	 * @param		Nom de la section recherchée
	 * @param		Eventuel commentaire associé à la section recherchée, en cas de création
	 * @param		Caractère surchargeable de la section recherchée, en cas de création
	 * @param		Caractère enregistrable de la section recherchée, en cas de création
	 * @return		la section dont le nom est donné en argument. Crée cette section si nécessaire et l'ajoute à parent.
	 */
	static Section& getSection (Section& parent, const IN_UTIL UTF8String& name, const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ),
						bool overloadable = true, bool safeguardable = true);

	/**
	 * @param		Parent de la valeur nommée recherchée
	 * @param		Nom de la section recherchée
	 * @param		Valeur affectée à la valeur nommée en cas de création
	 * @param		Eventuel commentaire associé à la valeur nommée recherchée, en cas de création
	 * @param		Caractère surchargeable de la valeur nommée recherchée, en cas de création
	 * @param		Caractère enregistrable de la valeur nommée recherchée, en cas de création
	 * @return		le bouléen dont le nom est donné en argument. Crée ce bouléen si nécessaire et l'ajoute à parent.
	 */
	static BoolNamedValue& getBooleanNamedValue (Section& parent, const IN_UTIL UTF8String& name, bool value = false,
						const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true);

	/**
	 * @param		Parent de la valeur nommée recherchée
	 * @param		Nom de la section recherchée
	 * @param		Valeur affectée à la valeur nommée en cas de création
	 * @param		Eventuel commentaire associé à la valeur nommée recherchée, en cas de création
	 * @param		Caractère surchargeable de la valeur nommée recherchée, en cas de création
	 * @param		Caractère enregistrable de la valeur nommée recherchée, en cas de création
	 * @return		la chaine de caractères dont le nom est donné en argument. Crée cette chaine de caractères si 
	 *				nécessaire et l'ajoute à parent.
	 */
	static StringNamedValue& getStringNamedValue (Section& parent, const IN_UTIL UTF8String& name, const IN_UTIL UTF8String& value = IN_UTIL UTF8String ( ),
						const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true);

	/**
	 * @param		Parent de la valeur nommée recherchée
	 * @param		Nom de la section recherchée
	 * @param		Valeur affectée à la valeur nommée en cas de création
	 * @param		Eventuel commentaire associé à la valeur nommée recherchée, en cas de création
	 * @param		Caractère surchargeable de la valeur nommée recherchée, en cas de création
	 * @param		Caractère enregistrable de la valeur nommée recherchée, en cas de création
	 * @return		l'entier long dont le nom est donné en argument. Crée cet entier si nécessaire et l'ajoute à parent.
	 */
	static LongNamedValue& getLongNamedValue (Section& parent, const IN_UTIL UTF8String& name, long value = 0,
						const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true);

	/**
	 * @param		Parent de la valeur nommée recherchée
	 * @param		Nom de la section recherchée
	 * @param		Valeur affectée à la valeur nommée en cas de création
	 * @param		Eventuel commentaire associé à la valeur nommée recherchée, en cas de création
	 * @param		Caractère surchargeable de la valeur nommée recherchée, en cas de création
	 * @param		Caractère enregistrable de la valeur nommée recherchée, en cas de création
	 * @return		l'entier long non signé dont le nom est donné en argument. Crée cet entier si nécessaire et l'ajoute à parent.
	 */
	static UnsignedLongNamedValue& getUnsignedLongNamedValue (Section& parent, const IN_UTIL UTF8String& name, unsigned long value = 0,
						const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true);

	/**
	 * @param		Parent de la valeur nommée recherchée
	 * @param		Nom de la section recherchée
	 * @param		Valeur affectée à la valeur nommée en cas de création
	 * @param		Eventuel commentaire associé à la valeur nommée recherchée, en cas de création
	 * @param		Caractère surchargeable de la valeur nommée recherchée, en cas de création
	 * @param		Caractère enregistrable de la valeur nommée recherchée, en cas de création
	 * @return		le réel double précision dont le nom est donné en argument. Crée ce réel si nécessaire et l'ajoute à parent.
	 */
	static DoubleNamedValue& getDoubleNamedValue (Section& parent, const IN_UTIL UTF8String& name, double value = 0.,
						const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true);

	/**
	 * @param		Parent de la valeur nommée recherchée
	 * @param		Nom de la section recherchée
	 * @param		Valeur affectée à la valeur nommée en cas de création
	 * @param		Eventuel commentaire associé à la valeur nommée recherchée, en cas de création
	 * @param		Caractère surchargeable de la valeur nommée recherchée, en cas de création
	 * @param		Caractère enregistrable de la valeur nommée recherchée, en cas de création
	 * @return		le triplet de réels double précision dont le nom est donné en argument. Crée ce triplet de réels si nécessaire et l'ajoute à parent.
	 */
	static DoubleTripletNamedValue& getDoubleTripletNamedValue (Section& parent, const IN_UTIL UTF8String& name, double x = 0., double y = 0., double z = 0.,
						const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true);

	/**
	 * @param		Parent de la valeur nommée recherchée
	 * @param		Nom de la section recherchée
	 * @param		Valeur affectée à la valeur nommée en cas de création
	 * @param		Eventuel commentaire associé à la valeur nommée recherchée, en cas de création
	 * @param		Caractère surchargeable de la valeur nommée recherchée, en cas de création
	 * @param		Caractère enregistrable de la valeur nommée recherchée, en cas de création
	 * @return		la couleur dont le nom est donné en argument. Créecette couleur si nécessaire et l'ajoute à parent.
	 */
	static ColorNamedValue& getColorNamedValue (Section& parent, const IN_UTIL UTF8String& name, double r = 0., double g = 0., double b = 0.,
						const IN_UTIL UTF8String& comment = IN_UTIL UTF8String ( ), bool overloadable = true, bool safeguardable = true);

	/**
	 * Met à jour le bouléen de nom boolName de la section section avec la valeur reçue en argument si il est modifiable. Créé le bouléen si nécessaire et l'ajoute à section.
	 * @param		section contenant le booléen
	 * @param		nom du bouléen à mettre à jour.
	 * @param		nouvelle valeur du bouléen
	 */
	static void updateBoolean (Section& section, const IN_UTIL UTF8String& boolName, bool value);

	/**
	 * Met à jour le bouléen de la section section avec la valeur reçue en argument si il est modifiable. Créé le bouléen si nécessaire et l'ajoute à section.
	 * @param		section contenant le booléen
	 * @param		nouvelle valeur du bouléen
	 */
	static void updateBoolean (Section& section, const IN_PREFS BoolNamedValue& boolean);

	/**
	 * Met à jour la chaine de caractères de nom strName de la section section avec la valeur reçue en argument si elle est modifiable. Créé la chaine si 
	 * nécessaire et l'ajoute à section.
	 * @param		section contenant la chaine de caractères
	 * @param		nom de la chaine à mettre à jour.
	 * @param		nouvelle valeur de la chaine de caractères
	 */
	static void updateString (Section& section, const IN_UTIL UTF8String& strName, const IN_UTIL UTF8String& value);

	/**
	 * Met à jour la chaine de caractères de la section section avec la valeur reçue en argument si elle est modifiable. Créé la chaine si nécessaire et l'ajoute à section.
	 * @param		section contenant la chaine
	 * @param		nouvelle valeur de la chaine
	 */
	static void updateString (Section& section, const IN_PREFS StringNamedValue& str);

	/**
	 * Met à jour l'entier de nom name de la section section avec la valeur reçue en argument si il est modifiable. Créé l'entier si nécessaire et l'ajoute à section.
	 * @param		section contenant l'entier
	 * @param		nom de l'entier à mettre à jour.
	 * @param		nouvelle valeur de l'entier
	 */
	static void updateLong (Section& section, const IN_UTIL UTF8String& name, long value);

	/**
	 * Met à jour l'entier de la section section avec la valeur reçue en argument si il est modifiable. Créé l'entier si nécessaire et l'ajoute à section.
	 * @param		section contenant l'entier
	 * @param		nouvelle valeur de l'entier
	 */
	static void updateLong (Section& section, const IN_PREFS LongNamedValue& value);

	/**
	 * Met à jour l'entier non signé de nom name de la section section avec la valeur reçue en argument si il est modifiable. Créé l'entier si nécessaire et l'ajoute à section.
	 * @param		section contenant l'entier
	 * @param		nom de l'entier à mettre à jour.
	 * @param		nouvelle valeur de l'entier
	 */
	static void updateUnsignedLong (Section& section, const IN_UTIL UTF8String& name, unsigned long value);

	/**
	 * Met à jour l'entier non signé de la section section avec la valeur reçue en argument si il est modifiable. Créé l'entier si nécessaire et l'ajoute à section.
	 * @param		section contenant l'entier
	 * @param		nouvelle valeur de l'entier
	 */
	static void updateUnsignedLong (Section& section, const IN_PREFS UnsignedLongNamedValue& value);

	/**
	 * Met à jour le réel double précision de nom name de la section section avec la valeur reçue en argument si il est modifiable. Créé le réel si nécessaire et l'ajoute à section.
	 * @param		section contenant le réel
	 * @param		nom du réel à mettre à jour.
	 * @param		nouvelle valeur du réel
	 */
	static void updateDouble (Section& section, const IN_UTIL UTF8String& name, double value);

	/**
	 * Met à jour le réel double précision de la section section avec la valeur reçue en argument si il est modifiable. Créé le réel si nécessaire et l'ajoute à section.
	 * @param		section contenant le réel
	 * @param		nouvelle valeur du réel
	 */
	static void updateDouble (Section& section, const IN_PREFS DoubleNamedValue& value);

	/**
	 * Met à jour le triplet de réels double précision de nom name de la section section avec la valeur reçue en argument si il est modifiable. Créé le 
	 * triplet de réels si nécessaire et l'ajoute à section.
	 * @param		section contenant le triplet de réels
	 * @param		nom du triplet de réels à mettre à jour.
	 * @param		nouvelle valeur du triplet de réels
	 */
	static void updateDoubleTriplet (Section& section, const IN_UTIL UTF8String& name, double x, double y, double z);

	/**
	 * Met à jour le triplet de réels double précision de la section section avec la valeur reçue en argument si il est modifiable. Créé le 
	 * triplet de réels si nécessaire et l'ajoute à section.
	 * @param		section contenant le triplet de réels
	 * @param		nouvelle valeur du triplet de réels
	 */
	static void updateDoubleTriplet (Section& section, const IN_PREFS DoubleTripletNamedValue& triplet);

	/**
	 * Met à jour la couleur de nom colorName de la section section avec les composantes RGB reçues en argument si elle est modifiable. Créé la couleur si 
	 * nécessaire et l'ajoute à section.
	 * @param		section contenant la couleur
	 * @param		nom de la couleur à mettre à jour.
	 * @param		composantes RGB de la couleur.
	 */
	static void updateColor (Section& section, const IN_UTIL UTF8String& colorName, double red, double green, double blue);

	/**
	 * Met à jour la couleur de la section section avec les composantes RGB reçues en argument si elle est modifiable. Créé la couleur si 
	 * nécessaire et l'ajoute à section.
	 * @param		section contenant la couleur
	 * @param		Couleur à mettre à jour
	 */
	static void updateColor (Section& section, const IN_PREFS ColorNamedValue& color);

	/**
	 * Modifie value à l'aide de la valeur name récupérée en configuration dans section.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant le booléen
	 * @param		Nom du booléen
	 * @param		Valeur à modifier
	 */
	static void getBoolean (const Section& section, const IN_UTIL UTF8String& name, bool& value);

	/**
	 * Modifie value à l'aide de la valeur de même nom récupérée en  configuration dans section si elle est modifiable.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant la valeur
	 * @param		Valeur à modifier
	 */
	static void getBoolean (const Section& section, BoolNamedValue& value);

	/**
	 * Modifie value à l'aide de la valeur name récupérée en configuration dans section si elle est modifiable.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant la chaine de caractères
	 * @param		Nom de la chaine de caractères
	 * @param		Valeur à modifier
	 */
	static void getString (const Section& section, const IN_UTIL UTF8String& name, IN_UTIL UTF8String& value);
	static void getString (const Section& section, const IN_UTIL UTF8String& name, IN_STD string& value);
	static void getString (const Section& section, StringNamedValue& value);

	/**
	 * Modifie value à l'aide de la valeur name récupérée en configuration dans section.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant l'entier long
	 * @param		Nom de l'entier long
	 * @param		Valeur à modifier
	 */
	static void getLong (const Section& section, const IN_UTIL UTF8String& name, long& value);

	/**
	 * Modifie value à l'aide de la valeur de même nom récupérée en configuration dans section si elle est modifiable.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant la valeur
	 * @param		Valeur à modifier
	 */
	static void getLong (const Section& section, LongNamedValue& value);

	/**
	 * Modifie value à l'aide de la valeur name récupérée en configuration dans section.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant l'entier long non signé
	 * @param		Nom de l'entier long non signé
	 * @param		Valeur à modifier
	 */
	static void getUnsignedLong (const Section& section, const IN_UTIL UTF8String& name, unsigned long& value);

	/**
	 * Modifie value à l'aide de la valeur de même nom récupérée en configuration dans section si elle est modifiable.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant la valeur
	 * @param		Valeur à modifier
	 */
	static void getUnsignedLong (const Section& section, UnsignedLongNamedValue& value);

	/**
	 * Modifie value à l'aide de la valeur name récupérée en configuration dans section.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant le réel double précision
	 * @param		Nom du réel double précision
	 * @param		Valeur à modifier
	 */
	static void getDouble (const Section& section, const IN_UTIL UTF8String& name, double& value);

	/**
	 * Modifie value à l'aide de la valeur de même nom récupérée en configuration dans section si elle est modifiable.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant la valeur
	 * @param		Valeur à modifier
	 */
	static void getDouble (const Section& section, DoubleNamedValue& value);

	/**
	 * Modifie x, y, z à l'aide de la valeur name récupérée en configuration dans section.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant le triplet de réels double précision
	 * @param		Nom du triplet de réels double précision
	 * @param		Valeur à modifier
	 */
	static void getDoubleTriplet (const Section& section, const IN_UTIL UTF8String& name, double& x, double& y, double& z);

	/**
	 * Modifie triplet à l'aide de la valeur de même nom récupérée en configuration dans section si elle est modifiable.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant la valeur
	 * @param		Valeur à modifier
	 */
	static void getDoubleTriplet (const Section& section, DoubleTripletNamedValue& triplet);

	/**
	 * Modifie r, g et b à l'aide de la valeur name récupérée en configuration dans section.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant la couleur
	 * @param		Nom de la couleur
	 * @param		Composantes RGB à modifier
	 */
	static void getColor (const Section& section, const IN_UTIL UTF8String& name, double& r, double& g, double& b);

	/**
	 * Modifie color à l'aide de la valeur de même nom récupérée en configuration dans section si elle est modifiable.
	 * Affiche un message sur la sortie standard si cette valeur n'est pas en configuration.
	 * @param		Section contenant la couleur
	 * @param		Couleur à modifier
	 */
	static void getColor (const Section& section, ColorNamedValue& color);


	private :

	/**
	 * Constructeur. Interdit.
	 */
	PreferencesHelper ( );

	/**
	 * Destructeur. Interdit.
	 */
	virtual ~PreferencesHelper ( );

	/**
	 * Constructeur de copie. Interdit.
	 */
	PreferencesHelper (const PreferencesHelper&);

	/**
	 * Opérateur =. Interdit.
	 */
	PreferencesHelper& operator = (const PreferencesHelper& element);
};	// class PreferencesHelper


END_NAMESPACE_PREFS

#endif	// PREFERENCES_HELPER_H

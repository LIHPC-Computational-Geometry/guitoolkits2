#ifndef VERSION_H
#define VERSION_H

#include <string>

#include <TkUtil/util_config.h>


//BEGIN_NAMESPACE_UTIL
namespace TkUtil {

  
/**
 * Classe décrivant la version d'un composant logiciel ou autre.
 * Une version est composée d'un nombre majeur, d'un nombre mineur, et
 * d'un numéro de release. Des opérateurs permettent de comparer deux instances.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class Version
{
	public :

	/** Version sous forme majeur.mineur.release */
	Version (const IN_STD string& version);

	/** Constructeur de copie. RAS. */
	Version (const Version&);

	/** Opérateur =. RAS. */
	Version& operator = (const Version&);

	/** Divers accesseurs. */
	unsigned short getMajor ( ) const;
	unsigned short getMinor ( ) const;
	unsigned short getRelease ( ) const;
	IN_STD string getVersion ( ) const;

	/** Opérateurs de comparaison. */
	bool operator == (const Version&) const;
	bool operator != (const Version&) const;
	bool operator > (const Version&) const;
	bool operator < (const Version&) const;
	bool operator >= (const Version&) const;
	bool operator <= (const Version&) const;

	/** Conversion chaine -> majeur/mineur/release. */
	static void stringToValues (const IN_STD string& v, unsigned short& major,
								unsigned short& minor, unsigned short& release);


	private :

	unsigned short		m_major, m_minor, m_release;
};	// class Version


//END_NAMESPACE_UTIL
}


#endif	// VERSION_H


#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <PrefsCore/prefs_config.h>
#include <TkUtil/Version.h>


/*!
 * \mainpage    Page principale de la bibliothèque PrefsCore.
 *
 * \section	presentation	Présentation
 * <P>La bibliothèque <I>prefs</I> contient les différentes classes constituant 
 * un ensemble de <B>préférences utilisateurs</B> pour une application : conteneurs,
 * entier, chaine de caractères, couleur, ... Ces informations sont stockées
 * dans un arbre où chaque ensemble de données, des <I>sections</I>, contient
 * des sous-sections et/ou des données.</P>
 *
 * <P>La persistance des données peut être gérer à l'aide de la bibliothèque
 * PrefsXerces, et la PrefsQt offre des
 * composants <I>Qt</I> permettant la gestion des préférences dans une IHM.</P>
 *
 * \section depend		Dépendances
 * * Cette bibliothèque utilise la bibliothèque TkUtil.
 * Les directives de compilation et d'édition des liens de la bibliothèque
 * <I>tkutil</I> s'appliquent également à celle-ci.
 */

BEGIN_NAMESPACE_PREFS


/**
 * Classe permettant de connaître la version de cette bibliothèque.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
*/
class PreferencesInfos
{
	public :

	/**
	 * @return		La version de cette bibliothèque
	 */
	static const IN_UTIL Version& getVersion ( )
	{ return _version; }


	protected :

	/**
	 * Constructeurs, opérateur = et destructeur : interdits.
	 */
	PreferencesInfos ( );
	PreferencesInfos (const PreferencesInfos& exc);
	PreferencesInfos& operator = (const PreferencesInfos& exc);
	~PreferencesInfos ( );


	private :

	/**
	 * La version de cette bibliothèque.
	 */
	static const IN_UTIL Version		_version;
};	// class PreferencesInfos


END_NAMESPACE_PREFS


#endif	// PREFERENCES_H

#ifndef QT_PREFERENCES_INFOS_H
#define QT_PREFERENCES_INFOS_H

#include <PrefsCore/prefs_config.h>
#include <TkUtil/Version.h>


/*!
 * \mainpage    Page principale de la bibliothèque PrefsQt.
 *
 * \section presentation    Présentation
 * La bibliothèque <I>PrefsQt</I> propose des services <I>Qt</I> de chargement,
 * consultation, modification et enregistrement de préférences utilisateurs.
 * La persistance est confiée aux composants de la bibliothèque PrefsXerces.
 * </P>
 * <P>Cette bibliothèque à pour principales classes :
 * <OL><I>QtPreferencesDialog</I>, boite de dialogue de chargement,
 * consultation, édition et enregistrement de préférences,
 * <OL><I>QtPreferencesPanel</I>, panneau gérant des préférences utilisateurs.
 * </P>
 * <P>Les tests de cette bibliothèque contiennent l'outil <I>CfgEdit</I>,
 * application indépendante de gestion de préférences utilisateurs.</P>
 */


/**
 * Classe permettant de connaître la version de cette bibliothèque.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class QtPreferencesInfos
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
	QtPreferencesInfos ( );
	QtPreferencesInfos (const QtPreferencesInfos& exc);
	QtPreferencesInfos& operator = (const QtPreferencesInfos& exc);
	~QtPreferencesInfos ( );


	private :

	/**
	 * La version de cette bibliothèque.
	 */
	static const IN_UTIL Version		_version;
};	// class QtPreferencesInfos


#endif	// QT_PREFERENCES_INFOS_H

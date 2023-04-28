#ifndef QT_PREFS_UPDATER_H
#define QT_PREFS_UPDATER_H

#include "PrefsCore/Section.h"
#include <PrefsCore/prefs_config.h>

#include <TkUtil/UTF8String.h>


BEGIN_NAMESPACE_PREFS


/** <P>Classe utilitaire de configuration permettant de mettre à jour certaines
 * préférences utilisateur contenues dans un fichier à partir de préférences
 * contenues dans un autre fichier, suite, par exemple, à des changements
 * système, à des changements de configuration d'un logiciel, ...
 * </P>
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 * @see			Section
 */
class QtPrefsUpdater 
{
	public :


	/**
	 * Charge la configuration depuis le fichier <I>config</I>, charge les
	 * correctifs depuis le fichier <I>patchs</I>, et les applique à la
	 * configuration lue après accord de l'utilisateur.
	 * Le correctif n'est appliqué que sur les préférences existantes.
	 * @param		Chemin d'accès complet au fichier de préférences à lire.
	 * @param		Chemin d'accès complet au fichier de correctifs à lire.
	 * @param		Titre de l'application
	 * @param		Chaine décrivant le contexte d'appel de cette fonction
	 * @param		<I>true</I> s'il faut demander l'accord de l'utilisateur,
	 * 				<I>false</I> si les modifications sont à effectuer sans
	 * 				demander l'accord de l'utilisateur.
	 * @param		En retour, liste des modifications effectuées.
	 * @return		Nombre d'actualisations effectuées
	 * @exception	Une exception est levée en cas d'erreur.
	 */
	static size_t update (
		const IN_STD string& config, const IN_STD string& patchs,
		const IN_UTIL UTF8String& appTitle, const IN_UTIL UTF8String& context,
		bool confirm, IN_UTIL UTF8String& modifications);


	protected :

	/**
	 * Met à jour la section <I>section</I> à partir des données de la section
	 * <I>patchs</I>.
	 * @param		Section à actualiser
	 * @param		Section contenant les mises à jour
	 * @param		Titre de l'application
	 * @param		Chaine décrivant le contexte d'appel de cette fonction
	 * @param		in/out, chaine contenant les modifications apportées
	 * 				actualisée
	 * @param		in/out, faut-il demander confirmation à l'utilisateur ?
	 * @param		in/out, faut-il continuer ?
	 * @return		Le nombre de mises à jour effectuées.
	 */
	static size_t update (
		Section& config, Section& patchs,
		const IN_UTIL UTF8String& appTitle, const IN_UTIL UTF8String& context,
		IN_UTIL UTF8String& message,
		bool& confirm, bool& operationStopped);


	private :

	/**
	 * Constructeur. Interdit.
	 */
	QtPrefsUpdater ( );

	/**
	 * Constructeur de copie. Interdit.
	 */
	QtPrefsUpdater (const QtPrefsUpdater&);

	/**
	 * Opérateur =. Interdit.
	 */
	QtPrefsUpdater& operator = (const QtPrefsUpdater&);

	/**
	 * Destructeur : RAS.
	 */
	virtual ~QtPrefsUpdater ( );
};	// class QtPrefsUpdater




END_NAMESPACE_PREFS

#endif	// QT_PREFS_UPDATER_H

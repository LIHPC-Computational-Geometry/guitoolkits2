/**
 * NamingFacilities.h : services pour le service de nommage CORBA.
 * @author	: Charles PIGNEROL, CEA/DAM/DSSI
 */

#ifndef NAMING_FACILITIES_H
#define NAMING_FACILITIES_H

#include <string>
#include <CORBA.h>
#include <TkUtil/Exception.h>


namespace CorbaFacilities
{

/**
 * Classe encapsulant des fonctionnalités du service de nommage CORBA CosNaming.
 * Ce service est utilisable dans le cadre d'applications simples
 * n'utilisant qu'un seul service de nommage sans contexte d'application.
 * @warning		Absence de service de désenregistrement auprès du service de
 *				nommage dans cette version.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 */
class NamingFacilities
{
	public :

	/**
	 * Initialise ce service. Ceci est un préalable avant toute utilisation
	 * d'une autre fonction de cette classe.
	 * @param		ORB utilisé durant la session. Doit être fonctionnel.
	 * @param		Machine hote ou est situé le service de nommage. Si chaine
	 *				non vide, cet hote est utilisé en priorité avec le port 
	 *				transmis en argument suivant. Dans le cas contraire la
	 *				résolution est effectuée avec les paramètres par défaut de
	 *				l'ORB.
	 * @throw		Une exception de type UTIL::Exception est levée en cas 
	 *				d'erreur
	 * @see			finalize
	 */
	static void initialize (CORBA::ORB_ptr orb, 
	                        std::string host = std::string (""), 
	                        size_t port = 2809);

	/**
	 * Libère les ressources utilisées par ce service.
	 * @param		ORB utilisé durant la session. Doit être fonctionnel.
	 * @throw		Une exception de type UTIL::Exception est levée en cas 
	 *				d'erreur
	 * @see			initialize
	 */
	static void finalize ( );

	/**
	 * @return		true si le service est initialisé, sinon false.
	 */
	static bool isInited ( );

	/**
	 * Fonction d'enregistrement d'une instance d'une application auprès du
	 * service de nommage.
	 * @param		Identifiant de l'instance (name[0].id)
	 * @param		Type de l'instance (name[0].kind)
	 * @param		Objet à enregistrer. Doit être valide.
	 * @param		Si true, l'instance est enregistrée à nouveau si son nom est
	 *				déjà enregistré (rebind). Dans le cas contraire, une
	 *				exception est levée si son nom est déjà enregistré.
	 * @throw		Une exception de type UTIL::Exception est levée en cas 
	 *				d'erreur
	 */
	static void bindObjectToName (
					const std::string& objId, const std::string& objKind,
					CORBA::Object_ptr objref, bool rebind
						);

	/**
	 * Fonction dédiée pour retourner un proxy sur une instance d'un serveur.
	 * @param		Identifiant de l'instance (name[0].id)
	 * @param		Type de l'instance (name[0].kind)
	 * @return		Une référence sur l'instance demandée
	 * @throw		Une exception de type UTIL::Exception est levée en cas 
	 *				d'erreur
	 */
	static CORBA::Object_ptr getObjectReference (
					const std::string& objId, const std::string& objKind
						);


	protected :


	/** L'ORB utilisé pendant la session de travail. */
	static CORBA::ORB_ptr					_orb;

	/** Le service de nommage utilisé. */
	static CosNaming::NamingContext_var		_rootContext;
};	// class NamingFacilities


}	// namespace CorbaFacilities



#endif	// NAMING_FACILITIES_H

/**
 * Classe d'utilitaires pour la classe ModelICorba.
 */

#ifndef OCEANE_MODEL_HELPER_
#define OCEANE_MODEL_HELPER_H


#include "ModelICorba.hh"


class OceaneModelHelper
{
	public :

	/**
	 * Initialise à rien la structure tranmise en arguments.
	 */
	static void initializeNodeSurfaces (
							Oceane::ModelICorba::NodeSurfaces& nodeSurfaces);

	/**
	 * Affecte au noeud le numéro de surface transmis en argument.
	 * @param		structure représentant les surfaces auxquelles appartient le
	 *				noeud.
	 * @param		numéro de surface à ajouter aux surfaces auxquelles 
	 *				appartient le noeud.
	 * @exception	Si le numéro de la surface ne peut être enregistré dans la
	 *				structure.
	 */
	static void addSurfaceToNode (
			Oceane::ModelICorba::NodeSurfaces& nodeSurfaces, size_t surface);


	/**
	 * @param		structure représentant les surfaces auxquelles appartient le
	 *				noeud.
	 * @param		numéro de la surface testée.
	 * @return		true si le noeud appartient à la surface dont le numéro est 
	 *				donné en argument, false dans le cas contraire.
	 * @exception	Si le numéro de la surface ne peut être enregistré dans la
	 *				structure.
	 */
	static bool nodeBelongToSurface (
		const Oceane::ModelICorba::NodeSurfaces& nodeSurfaces, size_t surface);


	private :

	/**
	 * Constructeurs : interdits.
	 */
	OceaneModelHelper ( );
	OceaneModelHelper (const OceaneModelHelper&);
	OceaneModelHelper& operator = (const OceaneModelHelper&);

	/**
	 * Destructeur : interdit.
	 */
	~OceaneModelHelper ( );
};	// class OceaneModelHelper

#endif	// OCEANE_MODEL_HELPER_H

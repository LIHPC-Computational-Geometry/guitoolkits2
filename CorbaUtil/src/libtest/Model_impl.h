#ifndef MODEL_IMPL_H
#define MODEL_IMPL_H

#include "ModelICorba.hh"
#include "TkUtil/Log.h"

#include <string>


namespace Oceane
{

	/*
	 * Implementation de l'interface CORBA ModelICorba.
	 */
	class Model_impl : public POA_Oceane::ModelICorba,
	                   public PortableServer::RefCountServantBase
	{
		public :

		/**
		 * Constructeur par défaut. RAS.
		 */
		Model_impl (const std::string& fileName);

		/**
		 * Destructeur. RAS.
		 */
		virtual ~Model_impl ( );

		/**
		 * Projection des noeuds reçus en arguments sur la surface désignée.
		 * Retourne les distances à la surface et les coordonnées 
		 * correspondantes sur la surface.
		 */
		virtual void projectNodes (
			const Oceane::ModelICorba::SurfaceList& surfaces,
			const Oceane::ModelICorba::NodeSurfacesList& nodeSurfaces,
			NodeList& nodes, Oceane::ModelICorba::Distances& dists);

		/**
		 * @return		Le nom du fichier associé.
		 */
		virtual const std::string& getFileName ( ) const
		{ return _fileName; }


		protected :

		/**
		 * Envoie le log dans le flux de messages de la classe ORBFacilities.
		 */
		virtual void log (const IN_UTIL Log& log);


		private :

		/**
		 * Constructeurs de copie et opérateur = : interdits.
		 */
		Model_impl (const Model_impl&);
		Model_impl& operator = (const Model_impl&);

		/** Le nom du fichier associé. */
		std::string			_fileName;
	};	// class Model_impl

};	// namespace Oceane

#endif	// MODEL_IMPL_H

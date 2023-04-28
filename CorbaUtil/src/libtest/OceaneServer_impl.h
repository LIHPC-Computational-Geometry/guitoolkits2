#ifndef OCEANE_SERVER_IMPL_H
#define OCEANE_SERVER_IMPL_H

#include "OceaneServerICorba.hh"
#include "Model_impl.h"
#include <TkUtil/Log.h>

#include <vector>


namespace Oceane
{

	/*
	 * Implementation de l'interface CORBA OceaneServerICorba.
	 */
	class OceaneServer_impl : public POA_Oceane::OceaneServerICorba,
	                          public PortableServer::RefCountServantBase
	{
		public :

		/**
		 * Constructeur par défaut. RAS.
		 */
		OceaneServer_impl ( );

		/**
		 * Destructeur. RAS.
		 */
		virtual ~OceaneServer_impl ( );

		/**
		 * @return		Le nom du serveur.
		 */
		virtual char* getOceaneServerName ( );

		virtual CORBA::ULong getId ( );

		/**
		 * Charge le modèle dont le nom de fichier est transmis en argument et
		 * retourne un proxy sur ce modèle.
		 */
		virtual ModelICorba_ptr loadModel (const char* fileName);

		/**
		 * Ferme le modèle dont un proxy est transmis en argument.
		 */
		virtual void closeModel (ModelICorba_ptr m);

		/**
		 * Met fin au serveur.
		 */
		virtual void shutdown ( );


		protected :

		/**
		 * @return		Le modèle associé au fichier dont le nom est tranmis
		 *				en argument. Instancie le modèle si nécéssaire.
		 */
		virtual Model_impl* getModel (const std::string& fileName);

		/**
		 * Affiche le log reçu en argument dans le flux de logs de la classe
		 * ORBFacilities.
		 */
		virtual void log (const IN_UTIL Log& log);


		private :

		/**
		 * Constructeurs de copie et opérateur = : interdits.
		 */
		OceaneServer_impl (const OceaneServer_impl&);
		OceaneServer_impl& operator = (const OceaneServer_impl&);

		/** Les modèles chargés. */
		std::vector<Model_impl*>		_models;
	};	// class OceaneServer_impl

};	// namespace Oceane

#endif	// OCEANE_SERVER_IMPL_H

#ifndef CONVERTIOR_H
#define CONVERTIOR_H

#include <IIOP.h>

#include <string>

std::string convertior (const std::string& ior, const std::string& newHost);

/**
 * Assez hideux, à finaliser.
 * Utilisé lors de l'encodage d'une IOR. Deux cas de figure :
 * - Des profiles existent : remplace le nom de machine hôte et le socket unix
 * par ceux fournis en arguments.
 * - Absence de profile : créé un profile socket Unix avec les informations
 * transmises en argument.
 * ATTENTION : ne traite (pour le moment) que les profiles de type
 * IOP::TAG_OMNIORB_UNIX_TRANS (socket de domaine Unix), donc ni les profiles
 * TCP/IP, ni les profiles SSL.
 */
void converMultipleComponentProfile (
		IOP::MultipleComponentProfile& components, const std::string& newHost,
		const std::string& unixSocket);

#endif	// CONVERTIOR_H

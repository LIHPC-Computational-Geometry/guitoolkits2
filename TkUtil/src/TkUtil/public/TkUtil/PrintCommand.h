#ifndef PRINT_COMMAND_H
#define PRINT_COMMAND_H

#include <TkUtil/util_config.h>
#include <TkUtil/Process.h>

#include <string>
#include <unistd.h>
#include <signal.h>
#if !defined(__GNUC__) && !defined(__ECC)
#include <siginfo.h>
#endif	// !defined(__GNUC__) && !defined(__ECC)

#include <vector>


BEGIN_NAMESPACE_UTIL


/** Classe réalisant des tâches d'impression dans un autre process.
 * @warning		Attention, cette classe positionne une action sur SIGCHLD.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class PrintCommand : public Process
{
	public :

	/**
	 * Structure contenant les options d'impression. A enrichir !
	 */
	struct CommandOptions
	{
		/** Constructeur et opérateur =. RAS.
		 */
		CommandOptions ( );
		CommandOptions (const CommandOptions& copied);
		CommandOptions& operator = (const CommandOptions&);

		/** Faut-il détruire le fichier en fin d'impression ? */
		bool		eraseFile;

		/** Nombre de copies à imprimer. */
		size_t		copiesNumber;
	};	// struct CommandOptions

	/** Constructeur.
	 * @param		Imprimante cible.
	 * @param		Fichier à imprimer.
	 * @param		Options d'impression.
	 * @warning		Il est indispensable d'utiliser l'opérateur new. 
	 *				L'instance s'autodétruira en fin d'impression.
	 */
	PrintCommand (const IN_STD string& prn, const IN_STD string& file,
	              const CommandOptions& options);

	/**
	 * Effectue l'impression. Lève une exception en cas d'erreur.
	 */
	virtual void execute ( );


	protected :

	/**
	 * Destructeur : appelé par l'instance en fin d'impression.
	 */
	virtual ~PrintCommand ( );


	private :

	/** Constructeur de copie et operateur = : interdits. */
	PrintCommand (const PrintCommand&);
	PrintCommand& operator = (const PrintCommand&);

	/** L'imprimante cible. */
	IN_STD string		_printer;

	/** Le fichier à imprimer. */
	IN_STD string		_file;

	/** Les options d'impression. */
	CommandOptions		_options;
};	// class PrintCommand


END_NAMESPACE_UTIL


#endif	// PRINT_COMMAND_H

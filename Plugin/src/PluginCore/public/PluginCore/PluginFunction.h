#ifndef PLUGIN_FUNCTION_H
#define PLUGIN_FUNCTION_H


/**
 * Structure et fonctions associées de définition et paramétrage d'une fonction
 * plugin.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI, d'après 
 *				Gilles GROSPELLIER et Christian LE BRETON
 */
#ifdef  __cplusplus
extern "C"
{
#endif	/* __cplusplus */

/**
 * structure décrivant une fonction et ses arguments.
 */
struct PluginFunction
{
	/** Le nom de la fonction associée. */
	char*	_name;

	/** Les arguments de la fonction. */
	void*	_args;

	/** Message d'erreur en retour de la fonction plugin. A allouer par malloc.
	 */
	char*	_errorMessage;

	/** Message d'avertissement en retour de la fonction plugin. A allouer par 
	 * malloc. */
	char*	_warningMessage;

#ifdef __cplusplus
	/**
	 * Constructeurs et opérateur de copie. RAS.
	 */
	PluginFunction ( );
	PluginFunction (const PluginFunction& function);
	PluginFunction& operator = (const PluginFunction& function);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~PluginFunction ( );
#endif	/* __cplusplus */
};	/* struct PluginFunction */


/**
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @return		Le nom de la fonction exécutée, ou NULL.
 */
const char* getFunctionName (const struct PluginFunction* function);

/**
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Nouveau nom de la fonction exécutée
 */
void setFunctionName (struct PluginFunction* function, const char* name);

/**
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @return		L'éventuel message d'erreur associé à l'exécution de la 
 *				fonction, ou NULL.
 */
const char* getFunctionErrorMessage (const struct PluginFunction* function);

/**
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Message d'erreur associé à l'exécution de la fonction, ou NULL.
 */
void setFunctionErrorMessage (
				struct PluginFunction* function, const char* message);

/**
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @return		L'éventuel message d'avertissement associé à l'exécution de la 
 *				fonction, ou NULL.
 */
const char* getFunctionWarningMessage (const struct PluginFunction* function);

/**
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Message d'avertissement associé à l'exécution de la fonction, 
 *				ou NULL.
 */
void setFunctionWarningMessage (
				struct PluginFunction* function, const char* message);

/**
 * Retourne la valeur booléenne (0 ou 1) associée à l'argument dont le nom
 * en renseigné en argument.
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Nom de l'argument. Ne doit pas être nul.
 * @param		Valeur de l'argument, en retour : 0 ou 1. Ne doit pas être nul.
 * @return		NULL en cas de succès ou un message d'erreur (chaîne statique
 *				à ne pas modifier).
 */
const char* getBooleanArg (const struct PluginFunction* function, 
                           const char* name, unsigned char* value);

/**
 * Ajoute l'argument donné en argument à la fonction. Modifie la valeur
 * courrante de l'argument si cet argument est déjà recensé.
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Nom de l'argument. Ne doit pas être nul.
 * @param		Valeur de l'argument : 0 ou 1.
 * @return		NULL en cas de succès ou un message d'erreur (chaîne statique
 *				à ne pas modifier).
 */
 const char* setBooleanArg (struct PluginFunction* function, const char* name, 
                            unsigned char value);

/**
 * Retourne la valeur de type chaine de caractères associée à l'argument dont le
 * nom en renseigné en argument.
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Nom de l'argument. Ne doit pas être nul.
 * @param		Valeur de l'argument, en retour. Ne doit pas être nul. 
 *				L'allocation est effectuée par cette fonction, la désallocation
 *				étant à la charge de l'appelant.
 * @return		NULL en cas de succès ou un message d'erreur (chaîne statique
 *				à ne pas modifier).
 */
 const char* getStringArg (const struct PluginFunction* function, 
                           const char* name, char** value);

/**
 * Ajoute l'argument donné en argument à la fonction. Modifie la valeur
 * courrante de l'argument si cet argument est déjà recensé.
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Nom de l'argument. Ne doit pas être nul.
 * @param		Valeur de l'argument. Ne doit pas être nulle.
 * @return		NULL en cas de succès ou un message d'erreur (chaîne statique
 *				à ne pas modifier).
 */
 const char* setStringArg (struct PluginFunction* function, const char* name, 
                           const char* value);

/**
 * Retourne la valeur de type long associée à l'argument dont le nom
 * en renseigné en argument.
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Nom de l'argument. Ne doit pas être nul.
 * @param		Valeur de l'argument, en retour. Ne doit pas être nul.
 * @return		NULL en cas de succès ou un message d'erreur (chaîne statique
 *				à ne pas modifier).
 */
 const char* getLongArg (const struct PluginFunction* function, 
                         const char* name, long* value);

/**
 * Ajoute l'argument donné en argument à la fonction. Modifie la valeur
 * courrante de l'argument si cet argument est déjà recensé.
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Nom de l'argument. Ne doit pas être nul.
 * @param		Valeur de l'argument.
 * @return		NULL en cas de succès ou un message d'erreur (chaîne statique
 *				à ne pas modifier).
 */
 const char* setLongArg (struct PluginFunction* function, const char* name, 
                         long value);

/**
 * Retourne la valeur de type unsigned long associée à l'argument dont le nom
 * en renseigné en argument.
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Nom de l'argument. Ne doit pas être nul.
 * @param		Valeur de l'argument, en retour. Ne doit pas être nul.
 * @return		NULL en cas de succès ou un message d'erreur (chaîne statique
 *				à ne pas modifier).
 */
 const char* getULongArg (const struct PluginFunction* function, 
                          const char* name, unsigned long* value);

/**
 * Ajoute l'argument donné en argument à la fonction. Modifie la valeur
 * courrante de l'argument si cet argument est déjà recensé.
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Nom de l'argument. Ne doit pas être nul.
 * @param		Valeur de l'argument.
 * @return		NULL en cas de succès ou un message d'erreur (chaîne statique
 *				à ne pas modifier).
 */
 const char* setULongArg (struct PluginFunction* function, const char* name, 
                          unsigned long value);

/**
 * Retourne la valeur de type double associée à l'argument dont le nom
 * en renseigné en argument.
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Nom de l'argument. Ne doit pas être nul.
 * @param		Valeur de l'argument, en retour. Ne doit pas être nul.
 * @return		NULL en cas de succès ou un message d'erreur (chaîne statique
 *				à ne pas modifier).
 */
 const char* getDoubleArg (const struct PluginFunction* function, 
                           const char* name, double* value);

/**
 * Ajoute l'argument donné en argument à la fonction. Modifie la valeur
 * courrante de l'argument si cet argument est déjà recensé.
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @param		Nom de l'argument. Ne doit pas être nul.
 * @param		Valeur de l'argument.
 * @return		NULL en cas de succès ou un message d'erreur (chaîne statique
 *				à ne pas modifier).
 */
 const char* setDoubleArg (struct PluginFunction* function, const char* name, 
                           double value);

/**
 * Vide la liste des arguments de la fonction.
 * @param		Structure définissant la fonction. Ne doit pas être nulle.
 * @return		NULL en cas de succès ou un message d'erreur (chaîne statique
 *				à ne pas modifier).
 */
 const char* freeArgList (struct PluginFunction* function);

#ifdef  __cplusplus
};	/* extern "C" */
#endif	/* __cplusplus */


#endif	/* PLUGIN_FUNCTION_H */

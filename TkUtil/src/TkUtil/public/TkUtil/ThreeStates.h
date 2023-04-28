#ifndef THREE_STATES_H
#define THREE_STATES_H

#include <TkUtil/util_config.h>

#include <assert.h>


BEGIN_NAMESPACE_UTIL


/** Enuméré décrivant trois états possibles : ON, OFF, indéterminé.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
enum THREESTATES {OFF = 0, UNDETERMINED = 2, ON = 1};


/**
 * @return		le nouvel état d'un ensemble de booléens prenant en 
 *				compte le booléen à y ajouter. true vaut ON, false OFF.
 */
inline THREESTATES newState (THREESTATES current, bool newValue)
{
	if (UNDETERMINED == current)
		return UNDETERMINED;

	if (((OFF == current) && (true == newValue))    ||
	    ((ON == current) && (false == newValue)))
		return UNDETERMINED;

	return current;
}	// stateWith


/**
 * @return		le nouvel état d'un ensemble de booléens prenant en 
 *				compte l'entier à y ajouter. 1 vaut ON, 0 OFF.
 */
inline THREESTATES newState (THREESTATES current, int newValue)
{
	assert ((0 <= newValue) && (1 >= newValue) && "newState : invalid parameter.");
	if (UNDETERMINED == current)
		return UNDETERMINED;

	if (((OFF == current) && (1 == newValue))    ||
	    ((ON == current) && (0 == newValue)))
		return UNDETERMINED;

	return current;
}	// stateWith


inline THREESTATES operator ! (THREESTATES state)
{
	return ON == state ? OFF : (OFF == state ? ON : UNDETERMINED);
}	// operator !


END_NAMESPACE_UTIL


#endif	// THREE_STATES_H

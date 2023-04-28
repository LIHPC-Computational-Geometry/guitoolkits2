#ifndef TIMER_H
#define TIMER_H

#include <TkUtil/util_config.h>

#include <time.h>
#include <sys/time.h>

#include "TkUtil/NumericConversions.h"
#include "TkUtil/UnicodeString.h"

#include <string>


BEGIN_NAMESPACE_UTIL


/**
 * Cette classe permet de mesurer des intervalles de temps (en seconde,
 * microsecondes ou temps CPU).
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class Timer
{
	public :

	/**
	 * Constructeurs, opérateur = et destructeur. RAS.
	 * @param		Si <I>started</I> vaut true le chronomètre est mis en
	 *				marche.
	 */
	Timer (bool started = false);
	Timer (const Timer&);
	Timer& operator = (const Timer&);
	~Timer ( )
	{ }

	/**
	 * Réinitialisation du chronomètre.
	 */
	void reset ( );

	/**
	 * Mise en marche du chronomètre.
	 * @warning		N'appelle pas reset.
	 */
	void start ( );

	/**
	 * Arrêt du chronomètre.
	 */
	void stop ( );

	/**
	 * @return		La date de mise en marche du chronomètre.
	 */
	time_t initialTime ( ) const
	{ return _initialTime; }

	/**
	 * @return		La date d'arrêt du chronomètre.
	 */
	time_t completedTime ( ) const
	{ return _completedTime; }

	/**
	 * @return		La date, en temps CPU, de mise en marche du chronomètre.
	 */
	clock_t initialClock ( ) const
	{ return _initialClock; }

	/**
	 * @return		La date, en temps CPU, d'arrêt du chronomètre.
	 */
	clock_t completedClock ( ) const
	{ return _completedClock; }

	/**
	 * @return		Le temps écoulé, en secondes, entre la mise en marche et
	 *				l'arrêt du chronomètre.
	 * @see			microduration
	 */
	time_t duration ( ) const
	{ return _completedTime - _initialTime; }

	/**
	 * @return		Le temps écoulé, en microsecondes, entre la mise en marche
	 * 				et l'arrêt du chronomètre.
	 * @see			duration
	 */
	unsigned long microduration ( ) const
	{
		timeval	duration;
		timersub (&_completedTimeval, &_initialTimeval, &duration);

		const unsigned long	microseconds	=
						1e6 * duration.tv_sec + duration.tv_usec;

		return microseconds;
	}	// microduration

	/**
	 * @return		Une chaine de caractères relatant le temps échu, avec
	 *				l'unité la plus adaptée
	 *				(secondes/millisecondes/microsecondes).
	 */
	std::string strDuration ( ) const;

	/**
	 * @return		Le temps écoulé, en unités de temps CPU, entre la mise en 
	 *				marche et l'arrêt du chronomètre.
	 */
	clock_t cpuDuration ( ) const
	{ return _completedClock - _initialClock; }

	/**
	 * @return		Le nombre d'instruction machine par seconde.
	 */
	static unsigned long cpuPerSecond ( )
	{ return CLOCKS_PER_SEC; }


	private :

	time_t	_initialTime, _completedTime;
	clock_t	_initialClock, _completedClock;
	timeval	_initialTimeval, _completedTimeval;
};	// class Timer


inline Timer::Timer (bool started)
	: _initialTime (0),  _completedTime (0), 
	  _initialClock (0), _completedClock (0),
	  _initialTimeval ( ), _completedTimeval ( )
{
	if (true == started)
		start ( );
}	// Timer::Timer ( )


inline Timer::Timer (const Timer& timer)
	: _initialTime (timer._initialTime),  
	  _completedTime (timer._completedTime), 
	  _initialClock (timer._initialClock), 
	  _completedClock (timer._completedClock),
	  _initialTimeval (timer._initialTimeval),
	  _completedTimeval (timer._completedTimeval)
{
}	// Timer::Timer (const Timer& timer)


inline Timer& Timer::operator = (const Timer& timer)
{
	if (this != &timer)
	{
		_initialTime		= timer._initialTime;
		_completedTime		= timer._completedTime;
		_initialClock		= timer._initialClock;
		_completedClock		= timer._completedClock;
		_initialTimeval		= timer._initialTimeval;
		_completedTimeval	= timer._completedTimeval;
	}	// if (this != &timer)

	return *this;
}	// Timer::operator = (const Timer& timer)


inline void Timer::reset ( )
{
	_initialTime		= 0;
	_completedTime		= 0;
	_initialClock		= 0;
	_completedClock		= 0;
}	// Timer::reset


inline void Timer::start ( )
{
	_initialClock	= clock ( );
	_initialTime	= time (NULL);
	gettimeofday (&_initialTimeval, NULL);
}	// Timer::start


inline void Timer::stop ( )
{
	_completedClock	= clock ( );
	gettimeofday (&_completedTimeval, NULL);
	_completedTime	= time (NULL);
}	// Timer::stop


inline std::string Timer::strDuration ( ) const
{
	TkUtil::UnicodeString	str;

	if (10 < duration ( ))
		str << (unsigned long)duration ( ) << " secondes";
	else if (1e6 < microduration ( ))	// 1 sec < d < 10 sec
	{
		double	d	= (double)microduration ( ) / 1.e6;
		str << TkUtil::NumericConversions::shortestRepresentation (d, 3, 3)
		    << " secondes";
	}
	else if (1000 < microduration ( ))	// 1e3 à 1e6 microsecondes
	{
		double	d	= (double)microduration ( ) / 1.e3;
		str << TkUtil::NumericConversions::shortestRepresentation (d, 3, 3)
		    << " millisecondes";
	}
	else	// < 1e3 microsecondes
		str << microduration ( ) << " microsecondes";

	return str.iso ( );
}	// Timer::strDuration


END_NAMESPACE_UTIL


#endif	// TIMER_H

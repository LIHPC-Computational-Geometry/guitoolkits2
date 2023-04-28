#ifndef DATE_H
#define DATE_H

#include <TkUtil/util_config.h>


#include <time.h>

#include <string>


BEGIN_NAMESPACE_UTIL


/**
 * Cette classe permet de représenter des dates. Ces dates sont exprimées
 * en dates locales.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class Date
{
	public :

	/**
	 * Constructeurs, opérateur = et destructeur. Initialisés à la date
	 * courante ou à celle transmise en argument.
	 */
	Date ( );
	Date (const Date&);
	Date& operator = (const Date&);

	/**
	 * Destructeur. RAS.
	 */
	virtual ~Date ( );

	/**
	 * @return		La date au format <I>jour/mois/année</I>.
	 */
	virtual IN_STD string getDate ( ) const;

	/**
	 * @return		L'heure au format <I>heure:minutes:secondes</I>.
	 */
	virtual IN_STD string getTime ( ) const;

	/**
	 * Réinitialisation à la date courante.
	 */
	virtual void reset ( );

	/**
	 * @return		L'année.
	 */
	virtual long getYear ( ) const
	{ return (unsigned long)(_date.tm_year + 1900); }

	/**
	 * @return		Le mois.
	 */
	virtual unsigned char getMonth ( ) const
	{ return (unsigned char)(_date.tm_mon + 1); }

	/**
	 * @return		Le jour.
	 */
	virtual unsigned char getDay ( ) const
	{ return (unsigned char)_date.tm_mday; }

	/**
	 * @return		L'heure.
	 */
	virtual unsigned char getHour ( ) const
	{ return (unsigned char)_date.tm_hour; }

	/**
	 * @return		Les minutes.
	 */
	virtual unsigned char getMinutes ( ) const
	{ return (unsigned char)_date.tm_min; }

	/**
	 * @return		Les secondes.
	 */
	virtual unsigned char getSeconds ( ) const
	{ return (unsigned char)_date.tm_sec; }


	private :

	struct	tm				_date;
};	// class Date


END_NAMESPACE_UTIL


#endif	// DATE_H

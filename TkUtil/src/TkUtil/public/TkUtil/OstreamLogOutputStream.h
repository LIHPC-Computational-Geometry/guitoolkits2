#ifndef OSTREAM_LOG_OUTPUT_STREAM_H
#define OSTREAM_LOG_OUTPUT_STREAM_H

#include <TkUtil/util_config.h>
#include <TkUtil/DefaultLogOutputStream.h>
#include <string>

#include <iostream>


BEGIN_NAMESPACE_UTIL


/**
 * Cette classe représente un flux sortant de logs dirigé dans flux
 * de type ostream type cout ou cerr.
 * @author	Charles PIGNEROL, CEA/DAM/DSSI
 */
class OstreamLogOutputStream : public DefaultLogOutputStream
{
	public :

	/**
	 * Le flux doit etre ouvert, et sa fermeture revient à la charge de
	 * l'appelant.
	 * @param		Flux où seront écrits les logs.
	 * @param		Masque à appliquer
	 * @param		true si les dates doivent être affichées
	 * @param		true si les heures doivent être affichées
	 * @param		true si les IDs de thread doivent être affichés
	 * @param		true si les logs doivent être colorisés selon leur nature
	 */
	OstreamLogOutputStream (
			IN_STD_IO ostream& stream, Log::TYPE mask = Log::PRODUCTION,
			bool enableDate = true, bool enableTime = true,
			bool enableThreadID = true, bool enableColorization = true);

	/**
	 * Le flux doit etre ouvert, et sa fermeture revient à la charge de
	 * l'appelant.
	 * @param		Flux où seront écrits les logs.
	 * @param		Jeu de caractères à utiliser lors des écritures
	 * @param		Masque à appliquer
	 * @param		true si les dates doivent être affichées
	 * @param		true si les heures doivent être affichées
	 * @param		true si les IDs de thread doivent être affichés
	 * @param		true si les logs doivent être colorisés selon leur nature
	 */
	OstreamLogOutputStream (
			IN_STD_IO ostream& stream, const IN_UTIL Charset& charset,
			Log::TYPE mask = Log::PRODUCTION,
			bool enableDate = true, bool enableTime = true,
			bool enableThreadID = true, bool enableColorization = true);

	/**
	 * Destructeur.
	 * @warning		Ne fait rien au niveau du flux associé.
	 */
	virtual ~OstreamLogOutputStream ( );

	/**
	 * @return		true si le flux associé est fonctionnel, sinon false.
	 */
	virtual bool isEnabled ( ) const;

	/**
	 * Ecrit le message dans le flux associé si le masque l'autorise.
	 */
	virtual void log (const Log& log);

	/**
	 * @return		true si les logs doivent être colorisés (selon leur nature),
	 * 				false dans le cas contraire.
	 */
	virtual bool enableColorization ( ) const;

	/**
	 * @param		true si les logs doivent être colorisés (selon leur nature),
	 * 				false dans le cas contraire.
	 */
	virtual void enableColorization (bool enable);


	private :

	/**
	 * Constructeur de copie et opérateur = : insterdits.
	 */
	OstreamLogOutputStream (const OstreamLogOutputStream&);
	OstreamLogOutputStream& operator = (const OstreamLogOutputStream&);

	/** Le flux associé. */
	IN_STD_IO ostream&		_stream;

	/** Faut il coloriser les logs ? */
	bool					_enableColorization;
};	// class OstreamLogOutputStream


END_NAMESPACE_UTIL


#endif	// OSTREAM_LOG_OUTPUT_STREAM_H

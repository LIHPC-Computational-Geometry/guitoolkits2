#ifndef QT_STATUS_LOG_OUTPUT_STREAM_H
#define QT_STATUS_LOG_OUTPUT_STREAM_H

#include <TkUtil/util_config.h>
#include <TkUtil/DefaultLogOutputStream.h>

#ifndef QT_5
#include <QtGui/QStatusBar>
#else	// QT_5
#include <QStatusBar>
#endif	// QT_5



/**
 * Classe assurant le lien entre la barre de status de la classe QMainWindow
 * et un flux sortant d'instances de la classe <I>Log</I>.
 *
 * @warning	<B>Classe non validée sous Qt 3.</B>
 *
 * @see		TkUtil::Log
 * @see		TkUtil::LogOutputStream
 */
class QtStatusLogOutputStream : public QObject, public TkUtil::DefaultLogOutputStream
{
	Q_OBJECT

	public :

	/** Constructeur.
	 * @param		Barre de status dans laquelle seront écrits les logs.
	 * @param		masque à appliquer initialement pour filtrer les logs
	 *				à afficher.
	 * @param		true si les dates doivent être affichées
	 * @param		true si les heures doivent être affichées
	 */
	QtStatusLogOutputStream (QStatusBar* statusBar, IN_UTIL Log::TYPE mask,
	            bool enableDate = false, bool enableTime = true);

	/** Destructeur. */
	virtual ~QtStatusLogOutputStream ( );

	/**
	 * Affiche le log reçu en argument dans la barre de status.
	 */
	virtual void log (const IN_UTIL Log& log);

	/**
	 * @return		Toujours <I>true</I>.
	 */
	virtual bool isEnabled ( ) const
	{ return true; }


	signals :

	/**
	 * Signal émis lorsque <I>log</I> est invoquée. Transmet en argument le
	 * message du log.
	 */
	void logCalled (QString message);


	private :

	/** Constructeur de copie. Interdit. */
	QtStatusLogOutputStream (const QtStatusLogOutputStream&);

	/** Opérateur de copie. Interdit. */
	QtStatusLogOutputStream& operator = (const QtStatusLogOutputStream&);

	/** La barre de status utilisée. */
	QStatusBar*			_statusBar;
};	// class QtStatusLogOutputStream



#endif	// QT_STATUS_LOG_OUTPUT_STREAM_H

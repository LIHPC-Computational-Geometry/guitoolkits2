#ifndef QT_LOGS_VIEW_H
#define QT_LOGS_VIEW_H

#include <TkUtil/util_config.h>
#include <TkUtil/DefaultLogOutputStream.h>

#include <QTextCharFormat>
#ifndef QT_5
#include <QtGui/QTextEdit>
#include <QtGui/QMenu>
#else	// QT_5
#include <QTextEdit>
#include <QMenu>
#endif	// QT_5



/**
 * <P>Panneau Qt affichant des logs, par exemple l'historique d'une session
 * d'une application.
 * </P>
 *
 * @warning		<B>En environnement <I>multithread</I> compiler avec
 * 				l'option <I>-DMULTITHREADED_APPLICATION</I></B>.
 */
class QtLogsView : public QTextEdit
// MOC_SKIP_BEGIN
#ifndef Q_MOC_RUN
                   , public IN_UTIL DefaultLogOutputStream
#endif	// Q_MOC_RUN
// MOC_SKIP_END
{
	Q_OBJECT

	public :

	/** Constructeur. Utilise le mode "Log" de Qt.
	 * @param		widget parent. Ne doit pas être nul.
	 * @param		masque à appliquer initialement pour filtrer les logs
	 *				à afficher.
	 * @param		true si les dates doivent être affichées
	 * @param		true si les heures doivent être affichées
	 */
	QtLogsView (QWidget* parent, IN_UTIL Log::TYPE mask,
	            bool enableDate = false, bool enableTime = true);

	/** Destructeur. */
	virtual ~QtLogsView ( );

	/**
	 * Affiche le log reçu en argument si le masque l'autorise.
	 * Invoque logError, logWarning ou logDefault selon la nature du log.
	 */
	virtual void log (const IN_UTIL Log& log);

	/**
	 * Fonction créée pour des problèmes d'ambiguité de la fonction
	 * setMask héritée par 2 fois de Qt et par DefaultLogOutputStream.
	 * Cette fonction appelle DefaultLogOutputStream::setMask avec l'argument
	 * transmis.
	 */
	virtual void setLogMask (IN_UTIL Log::TYPE mask);

	/**
	 * @return		Toujours true.
	 */
	bool isEnabled ( ) const
	{ return true; }

	/**
	 * @return		Le <I>log</I> courrant, s'il y en a un.
	 * @see			logCalled
	 */
	virtual const IN_UTIL Log* getCurrentLog ( ) const;


	signals :

	/**
	 * Signal émis lorsque <I>log</I> est appelé. Le <I>log</I> tranmis en
	 * argument est accessible via <I>getCurrentLog</I>.
	 * @see		getCurrentLog
	 */
	void logCalled ( );


	public slots :

	/**
	 * Efface les logs.
	 */
	virtual void clearCallback ( );

	/**
	 * Appelé lorsque logCalled est émis en environnement multithread. Affiche
	 * le log courant.
	 * @see		logCalled
	 * @see		getCurrentLog
	 */
	virtual void logCalledCallback ( );


	protected :

	/**
	 * Créé un menu popup pour cette fenêtre contenant l'item "Effacer".
	 */
	virtual QMenu* createStandardContextMenu ( );

	/**
	 * Formate le texte du log reçu en argument pour affichage dans le flux.
	 * cf. définition de base. En plus, formate éventuellement le texte sur un
	 * nombre de colonnes données.
	 * @see		maxColumns
	 */
	virtual IN_UTIL UTF8String format (const IN_UTIL Log& log) const;

	/** Affiche le log reçu en argument avec le style "erreur". */
	virtual void logError (const IN_UTIL Log& log);

	/** Affiche le log reçu en argument avec le style "attention". */
	virtual void logWarning (const IN_UTIL Log& log);

	/** Affiche le log reçu en argument avec le style "processus". */
	virtual void logProcess (const IN_UTIL Log& log);

	/** Affiche le log reçu en argument avec le style "progression". */
	virtual void logProgression (const IN_UTIL Log& log);

	/** Affiche le log reçu en argument avec le style par défaut. */
	virtual void logDefault (const IN_UTIL Log& log);

	/** Le nombre maximum de lignes. Si -1, nombre illimité. */
	static int					maxLines;

	/** Le nombre maximum de colonnes. Si -1 alors absence de césure. */
	static int					maxColumns;


	private :

	/** Constructeur de copie. Interdit. */
	QtLogsView (const QtLogsView&);

	/** Opérateur de copie. Interdit. */
	QtLogsView& operator = (const QtLogsView&);

	/** Les paramètres d'affichage des différents types de log. */
	QTextCharFormat		_defaultFormat, _errorFormat, _warningFormat, 
						_processFormat, _progressionFormat;

	/** Le log en cours (lors d'un appel à <I>log</I>.
	 * @see		logCalled
	 */
	mutable const IN_UTIL Log*		_currentLog;
};	// class QtLogsView



#endif	// QT_LOGS_VIEW_H

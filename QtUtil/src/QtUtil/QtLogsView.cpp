#include "QtUtil/QtLogsView.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/UTF8String.h>
#include <TkUtil/InformationLog.h>
#include <TkUtil/ProcessLog.h>
#include <TkUtil/MemoryError.h>

#ifndef QT_5
#include <QtGui/QApplication>
#else	// QT_5
#include <QApplication>
#endif	// QT_5

#include <assert.h>

USING_UTIL

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


int QtLogsView::maxLines		= -1;
int QtLogsView::maxColumns		= -1;


QtLogsView::QtLogsView (
		QWidget* parent, Log::TYPE mask, bool enableDate, bool enableTime)
	: QTextEdit (parent), DefaultLogOutputStream (mask, enableDate, enableTime),
	  _defaultFormat ( ), _errorFormat ( ), _warningFormat ( ),
	  _processFormat ( ), _progressionFormat ( ),
	  _currentLog (0)
{
	setLogMask (mask);
	setUndoRedoEnabled (false);

	QBrush	brush (Qt::black);
	_defaultFormat.setForeground (brush);
	brush.setColor (Qt::red);
	_errorFormat.setForeground (brush);
	brush.setColor (Qt::blue);
	_warningFormat.setForeground (brush);
	brush.setColor (Qt::darkRed);
	_processFormat.setForeground (brush);
	brush.setColor (Qt::darkGray);
	_progressionFormat.setForeground (brush);

#ifdef MULTITHREADED_APPLICATION
	connect (this, SIGNAL (logCalled ( )), this, SLOT (logCalledCallback ( )));
#endif	// MULTITHREADED_APPLICATION
}	// QtLogsView::QtLogsView


QtLogsView::QtLogsView (const QtLogsView& view)
{
	assert (0 && "QtLogsView::QtLogsView copy constructor is not allowed.");
}	// QtLogsView::QtLogsView (const QtLogsView& view)


QtLogsView& QtLogsView::operator = (const QtLogsView& view)
{
	assert (0 && "QtLogsView::operator = is not allowed.");
	return *this;
}	// QtLogsView::QtLogsView (const QtLogsView& view)


QtLogsView::~QtLogsView ( )
{
}	// QtLogsView::~QtLogsView


void QtLogsView::clearCallback ( )
{
	clear ( );
}	// QtLogsView::clearCallback



void QtLogsView::logCalledCallback ( )
{
#ifdef MULTITHREADED_APPLICATION
	LOCK_LOG_STREAM

	if (0 == _currentLog)
		return;

	if (_currentLog->getType ( ) == Log::ERROR)
		logError (*_currentLog);
	else if (_currentLog->getType ( ) == Log::WARNING)
		logWarning (*_currentLog);
	else if (_currentLog->getType ( ) == Log::PROCESS)
		logProcess (*_currentLog);
	else if (_currentLog->getType ( ) == Log::TASK_PROGRESSION)
		logProgression (*_currentLog);
	else
		logDefault (*_currentLog);

	// QApplication::processEvents nécessaire car dans certains cas la mise à
	// jour du widget ne survient que bien après.
	// C'est par exemple le cas lorsque le code qui suit monopolise pas mal le
	// cpu, et que Qt ne reprend la main qu'après => les logs ne sont plus
	// considérables comme étant en temps réel. append passe t'elle également
	// par les signaux et moc ???
	QApplication::processEvents ( );
#endif	// MULTITHREADED_APPLICATION
}	// QtLogsView::logCalledCallback



QMenu* QtLogsView::createStandardContextMenu ( )
{
	QMenu*	menu	= QTextEdit::createStandardContextMenu ( );
	CHECK_NULL_PTR_ERROR (menu)
	menu->addAction ("Effacer", this, SLOT (clearCallback ( )));

	return menu;
}	// QtLogsView::createStandardContextMenu


void QtLogsView::log (const Log& log)
{
#ifdef MULTITHREADED_APPLICATION
	LOCK_LOG_STREAM
#endif	// MULTITHREADED_APPLICATION

	if (0 == (log.getType ( ) & getMask ( )))
		return;

#ifdef MULTITHREADED_APPLICATION
	_currentLog	= &log;
	emit logCalled ( );
#else
	if (log.getType ( ) == Log::ERROR)
		logError (log);
	else if (log.getType ( ) == Log::WARNING)
		logWarning (log);
	else if (log.getType ( ) == Log::PROCESS)
		logProcess (log);
	else if (log.getType ( ) == Log::TASK_PROGRESSION)
		logProgression (log);
	else
		logDefault (log);
#endif	// MULTITHREADED_APPLICATION

	_currentLog	= 0;
}	// QtLogsView::log


void QtLogsView::setLogMask (IN_UTIL Log::TYPE mask)
{
	DefaultLogOutputStream::setMask (mask);
}	// QtLogsView::setLogMask


const Log* QtLogsView::getCurrentLog ( ) const
{
	return _currentLog;
}	// QtLogsView::getCurrentLog


UTF8String QtLogsView::format (const Log& log) const
{
	UTF8String	formated	= DefaultLogOutputStream::format (log);
	if (maxColumns < 0)
		return formated;

	return formated.format (maxColumns);
}	// QtLogsView::format


void QtLogsView::logError (const Log& log)
{
	setCurrentCharFormat (_errorFormat);
	UTF8String	error	= format (log);
	append (UTF8TOQSTRING (error));
}	// void QtLogsView::logError (const Log& log)


void QtLogsView::logWarning (const Log& log)
{
	setCurrentCharFormat (_warningFormat);
	UTF8String	warning	= format (log);
	append (UTF8TOQSTRING (warning));
}	// void QtLogsView::logWarning (const Log& log)


void QtLogsView::logProcess (const Log& l)
{
	const ProcessLog*	processLog	= dynamic_cast<const ProcessLog*>(&l);
	if (0 != processLog)
	{
		UTF8String	description (charset);
		description << "==> Sorties du processus ou script " 
		            << processLog->getProcessName ( ) << " :";
		log (InformationLog (description));
	}
	setCurrentCharFormat (_processFormat);
	UTF8String	process	= format (l);
	append (UTF8TOQSTRING (process));
	if (0 != processLog)
	{
		UTF8String	description;
		description << "==> Fin des sorties du processus ou script " 
		            << processLog->getProcessName ( ) << " :";
		log (InformationLog (description));
	}
}	// void QtLogsView::logProcess (const Log& l)


void QtLogsView::logProgression (const Log& log)
{
	UTF8String	progression	= format (log);
	setCurrentCharFormat (_progressionFormat);
	append (UTF8TOQSTRING (progression));
}	// void QtLogsView::logProgression (const Log& log)


void QtLogsView::logDefault (const Log& log)
{
	setCurrentCharFormat (_defaultFormat);
//	append (QtUnicodeHelper::unicodeStringToQString (format(log)));
	append (UTF8TOQSTRING (format(log)));
}	// void QtLogsView::logDefault (const Log& log)



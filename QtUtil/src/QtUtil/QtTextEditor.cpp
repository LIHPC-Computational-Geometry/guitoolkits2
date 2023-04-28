#include "QtUtil/QtTextEditor.h"
#include "QtUtil/QtUnicodeHelper.h"

#include <TkUtil/InternalError.h>
#include <TkUtil/MemoryError.h>

#include <QTextBlock>
#include <QPainter>
#ifndef QT_5
#include <QtGui/QAction>
#include <QtGui/QMenu>
#else	// QT_5
#include <QAction>
#include <QMenu>
#endif	// QT_5

#include <iostream>
#include <memory>
#include <assert.h>


using namespace TkUtil;
using namespace std;

static const Charset	charset ("àéèùô");

USE_ENCODING_AUTODETECTION


// =============================================================================
//                LA CLASSE QtTextEditor::QtLineNumberArea
// =============================================================================

QtTextEditor::QtLineNumberArea::QtLineNumberArea (QtTextEditor& editor)
	: QWidget (&editor), _textEditor (&editor)
{
}	// QtLineNumberArea::QtLineNumberArea


QtTextEditor::QtLineNumberArea::QtLineNumberArea (
										const QtTextEditor::QtLineNumberArea&)
	: QWidget (0)
{
	assert (0 && "QtLineNumberArea copy constructor is not allowed.");
}	// QtLineNumberArea::QtLineNumberArea


QtTextEditor::QtLineNumberArea& QtTextEditor::QtLineNumberArea::operator = (
										const QtTextEditor::QtLineNumberArea&)
{
	assert (0 && "QtLineNumberArea assignment operator = is not allowed.");
	return *this;
}	// QtLineNumberArea::operator =


QtTextEditor::QtLineNumberArea::~QtLineNumberArea ( )
{
}	// QtLineNumberArea::~QtLineNumberArea


QSize QtTextEditor::QtLineNumberArea::sizeHint ( ) const
{
	return QSize(getTextEditor ( ).lineNumberAreaWidth ( ), 0);
}	// QtLineNumberArea::sizeHint


const QtTextEditor& QtTextEditor::QtLineNumberArea::getTextEditor ( ) const
{
	if (0 == _textEditor)
	{
		INTERNAL_ERROR (exc, "Editeur nul.", "QtLineNumberArea::getTextEditor")
		throw exc;
	}	// if (0 == _textEditor)

	return *_textEditor;
}	// QtLineNumberArea::getTextEditor


QtTextEditor& QtTextEditor::QtLineNumberArea::getTextEditor ( )
{
	if (0 == _textEditor)
	{
		INTERNAL_ERROR (exc, "Editeur nul.", "QtLineNumberArea::getTextEditor")
		throw exc;
	}	// if (0 == _textEditor)

	return *_textEditor;
}	// QtLineNumberArea::getTextEditor


void QtTextEditor::QtLineNumberArea::paintEvent (QPaintEvent* event)
{
	getTextEditor ( ).drawLinesNumbers (event->rect ( ));
}	// QtLineNumberArea::paintEvent


// =============================================================================
//                            LA CLASSE QtTextEditor
// =============================================================================

QtTextEditor::QtTextEditor (QWidget* parent, bool displayLinesNumbers)
	: QPlainTextEdit (parent),
	  _lineNumberArea (0), _displayLinesNumbers (displayLinesNumbers)
{
	_lineNumberArea	= new QtTextEditor::QtLineNumberArea (*this);

	connect (this, SIGNAL (blockCountChanged (int)), this,
	         SLOT (updateLineNumberAreaWidthCallback (int)));
	connect (this, SIGNAL (updateRequest (QRect, int)), this,
	         SLOT (updateLineNumberAreaWidthCallback (QRect, int)));
	connect (this, SIGNAL (cursorPositionChanged ( )), this,
	         SLOT (cursorPositionCallback ( )));

	updateLineNumberAreaWidthCallback (0);
	cursorPositionCallback ( );
}	// QtTextEditor::QtTextEditor


QtTextEditor::QtTextEditor (const QtTextEditor&)
	: QPlainTextEdit (0), _lineNumberArea (0), _displayLinesNumbers (false)
{
	assert (0 && "QPlainTextEdit copy constructor is not allowed.");
}	// QtTextEditor::QtTextEditor


QtTextEditor& QtTextEditor::operator = (const QtTextEditor&)
{
	assert (0 && "QPlainTextEdit assignment operator = is not allowed.");
	return *this;
}	// QtTextEditor::operator =


QtTextEditor::~QtTextEditor ( )
{
}	// QtTextEditor::~QtTextEditor


bool QtTextEditor::displayLinesNumbers ( ) const
{
	return _displayLinesNumbers;
}	// QtTextEditor::displayLinesNumbers


void QtTextEditor::displayLinesNumbers (bool displayLinesNumbers)
{
	if (displayLinesNumbers != _displayLinesNumbers)
	{
		_displayLinesNumbers	= displayLinesNumbers;
		updateLineNumberAreaWidthCallback (0);
	}	// if (displayLinesNumbers != _displayLinesNumbers)
}	// QtTextEditor::displayLinesNumbers


int QtTextEditor::lineNumberAreaWidth ( ) const
{
	if (false == displayLinesNumbers ( ))
		return 0;

	int	digits	= 1;
	int	max		= qMax (1, blockCount ( ));

	while (max >= 10)
	{
		max		/= 10;
		digits	+= 1;
	}	// while (max >= 10)

	const int space	= 3 + digits * fontMetrics ( ).width (QLatin1Char ('9'));

	return space;
}	// QtTextEditor::lineNumberAreaWidth


void QtTextEditor::drawLinesNumbers (const QRect& rect)
{
	try
	{
		if (false == displayLinesNumbers ( ))
			return;

		// On efface :
		QPainter	painter (&getLineNumberArea ( ));
		painter.fillRect (rect, Qt::lightGray);

		// On affiche les numéros de lignes correspondant au scroller :
		QTextBlock	block	= firstVisibleBlock ( );
		int			number	= block.blockNumber ( );
		int			top		=
			blockBoundingGeometry (block).translated (contentOffset ( )).top( );
		int			bottom	= top + blockBoundingRect (block).height ( );

		while ((block.isValid ( )) && (top <= rect.bottom ( )))
		{
			if ((true == block.isVisible ( )) && (bottom >= rect.top ( )))
			{
				const QString	str	= QString::number (number + 1);
				painter.setPen (Qt::black);
				painter.drawText (0, top, getLineNumberArea ( ).width ( ),
							fontMetrics ( ).height ( ), Qt::AlignRight, str);
			}	// if ((true == block.isVisible ( )) && (bottom >= ...

			block	= block.next ( );
			top		= bottom;
			bottom	= top + blockBoundingRect (block).height ( );
			number	+= 1;
		}	// while ((block.isValid ( )) && (top <= rect.bottom ( )))
	}
	catch (const Exception& exc)
	{
		UTF8String	message (charset);
		message << __FILE__ << ' ' << (unsigned long)__LINE__ << " Exception levée : "
		     << exc.getFullMessage ( );
		ConsoleOutput::cerr ( ) << message << co_endl;
	}
	catch (...)
	{
		UTF8String	message (charset);
		message << __FILE__ << ' ' << (unsigned long)__LINE__ << " Exception non documentée.";
		ConsoleOutput::cerr ( ) << message << co_endl;
	}
}	// QtTextEditor::drawLinesNumbers


void QtTextEditor::contextMenuEvent (QContextMenuEvent * event)
{
	try
	{
		unique_ptr<QMenu>	menu (createPopupMenu ( ));
		menu->exec (event->globalPos ( ));
	}
	catch (...)
	{
	}
}	// QtTextEditor::contextMenuEvent


void QtTextEditor::resizeEvent (QResizeEvent* event)
{
	try
	{
		QPlainTextEdit::resizeEvent (event);

		const QRect	rect	= contentsRect ( );
		getLineNumberArea ( ).setGeometry (QRect (
			rect.left( ), rect.top( ), lineNumberAreaWidth( ), rect.height( )));
	}
	catch (const Exception& exc)
	{
		UTF8String	message (charset);
		message << __FILE__ << ' ' << (unsigned long)__LINE__ << " Exception levée : "
		     << exc.getFullMessage ( );
		ConsoleOutput::cerr ( ) << message << co_endl;
	}
	catch (...)
	{
		UTF8String	message (charset);
		message << __FILE__ << ' ' << (unsigned long)__LINE__ << " Exception non documentée.";
		ConsoleOutput::cerr ( ) << message << co_endl;
	}
}	// QtTextEditor::resizeEvent


const QtTextEditor::QtLineNumberArea& QtTextEditor::getLineNumberArea ( ) const
{
	if (0 == _lineNumberArea)
	{
		INTERNAL_ERROR (exc, "Zone d'affichage des numéros de ligne nulle.",
		                "QtTextEditor::getLineNumberArea")
		throw exc;
	}	// if (0 == _lineNumberArea)

	return *_lineNumberArea;
}	// QtTextEditor::getLineNumberArea


QtTextEditor::QtLineNumberArea& QtTextEditor::getLineNumberArea ( )
{
	if (0 == _lineNumberArea)
	{
		INTERNAL_ERROR (exc, "Zone d'affichage des numéros de ligne nulle.",
		                "QtTextEditor::getLineNumberArea")
		throw exc;
	}	// if (0 == _lineNumberArea)

	return *_lineNumberArea;
}	// QtTextEditor::getLineNumberArea


QMenu* QtTextEditor::createPopupMenu ( )
{
	QMenu*		menu		= createStandardContextMenu ( );

	try
	{
		CHECK_NULL_PTR_ERROR (menu)
		menu->addSeparator ( );
		QAction*	linesAction	=
			new QAction (QSTR ("Afficher les numéros de ligne"), this);
		linesAction->setCheckable (true);
		linesAction->setChecked (
				true == displayLinesNumbers ( ) ? Qt::Checked : Qt::Unchecked);
		connect (linesAction, SIGNAL (toggled (bool)), this,
		         SLOT (displayLinesNumbersCallback ( )));
		menu->addAction (linesAction);
	}
	catch (...)
	{
	}

	return menu;
}	// QtTextEditor::createPopupMenu


void QtTextEditor::updateLineNumberAreaWidthCallback (int /* blockCount*/)
{
	try
	{
		setViewportMargins (lineNumberAreaWidth ( ), 0, 0, 0);
	}
	catch (const Exception& exc)
	{
		UTF8String	message (charset);
		message << __FILE__ << ' ' << (unsigned long)__LINE__ << " Exception levée : "
		     << exc.getFullMessage ( );
		ConsoleOutput::cerr ( ) << message << co_endl;
	}
	catch (...)
	{
		UTF8String	message (charset);
		message << __FILE__ << ' ' << (unsigned long)__LINE__ << " Exception non documentée.";
		ConsoleOutput::cerr ( ) << message << co_endl;
	}
}	// QtTextEditor::updateLineNumberAreaWidthCallback


void QtTextEditor::updateLineNumberAreaWidthCallback (const QRect& rect, int dy)
{
	try
	{
		CHECK_NULL_PTR_ERROR (viewport ( ))

		if (0 != dy)
			getLineNumberArea ( ).scroll (0, dy);
		else
			getLineNumberArea ( ).update (
				0, rect.y ( ), getLineNumberArea( ).width ( ), rect.height ( ));

		if (true == rect.contains (viewport ( )->rect ( )))
			updateLineNumberAreaWidthCallback (0);
	}
	catch (const Exception& exc)
	{
		UTF8String	message (charset);
		message << __FILE__ << ' ' << (unsigned long)__LINE__ << " Exception levée : "
		     << exc.getFullMessage ( );
		ConsoleOutput::cerr ( ) << message << co_endl;
	}
	catch (...)
	{
		UTF8String	message (charset);
		message << __FILE__ << ' ' << (unsigned long)__LINE__ << " Exception non documentée.";
		ConsoleOutput::cerr ( ) << message << co_endl;
	}
}	// QtTextEditor::updateLineNumberAreaWidthCallback


void QtTextEditor::cursorPositionCallback ( )
{
	try
	{
		QList<QTextEdit::ExtraSelection>	extraSelections;
//		if (false == isReadOnly ( ))
		{
			QTextEdit::ExtraSelection	selection;
			const QColor	lineColor	= QColor (Qt::yellow).lighter (160);
			selection.format.setBackground (lineColor);
			selection.format.setProperty (QTextFormat::FullWidthSelection,true);
			selection.cursor	= textCursor ( );
			selection.cursor.clearSelection ( );
			extraSelections.append (selection);
		}	// if (false == isReadOnly ( ))

		setExtraSelections (extraSelections);
	}
	catch (const Exception& exc)
	{
		UTF8String	message (charset);
		message << __FILE__ << ' ' << (unsigned long)__LINE__ << " Exception levée : "
		     << exc.getFullMessage ( );
		ConsoleOutput::cerr ( ) << message << co_endl;
	}
	catch (...)
	{
		UTF8String	message (charset);
		message << __FILE__ << ' ' << (unsigned long)__LINE__ << " Exception non documentée.";
		ConsoleOutput::cerr ( ) << message << co_endl;
	}
}	// QtTextEditor::cursorPositionCallback


void QtTextEditor::displayLinesNumbersCallback ( )
{
	try
	{
		displayLinesNumbers (!displayLinesNumbers ( ));
	}
	catch (...)
	{
	}
}	// QtTextEditor::displayLinesNumbersCallback



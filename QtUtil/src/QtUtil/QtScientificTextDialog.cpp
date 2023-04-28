
#include "QtUtil/QtScientificTextDialog.h"
#include "QtUtil/QtConfiguration.h"
#include "QtUtil/QtStringHelper.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/MemoryError.h>

#include <vector>
#include <assert.h>



USING_STD
USING_UTIL


// ===========================================================================
//                      LA CLASSE QtGreekPushButton
// ===========================================================================

QtGreekPushButton::QtGreekPushButton (
		QWidget* parent, const QString& character, QtScientificTextDialog& dlg)
	: QPushButton (character, parent), _dialog (dlg)
{
	connect (this, SIGNAL (clicked (bool)), this, SLOT (insertCharacter(bool)));
}	// QtGreekPushButton::QtGreekPushButton


void QtGreekPushButton::insertCharacter (bool)
{
	try
	{
		QString			character	= text ( );
		QTextDocument*	doc	= _dialog.getTextEditor ( ).document ( );
		CHECK_NULL_PTR_ERROR (doc)
		size_t	pos	= _dialog.getTextEditor ( ).textCursor ( ).position ( );
		QString	text	= doc->toPlainText ( );
		const bool	shouldBlock	= doc->signalsBlocked ( );
		doc->blockSignals (true);
		text.insert (pos, character);
		doc->setPlainText (text);
		QTextCursor	cursor (doc);
		cursor.setPosition(pos + 1);
		_dialog.getTextEditor ( ).setTextCursor (cursor);
		doc->blockSignals (shouldBlock);
		_dialog.textModified ( );

		// On rend le focus à l'éditeur de texte :
		_dialog.getTextEditor ( ).setFocus (Qt::OtherFocusReason);
	}
	catch (...)
	{
	}
}	// QtGreekPushButton::insertCharacter


template<class PushButton> PushButton* addButton (
		QtScientificTextDialog& dlg, const QString& label, QGridLayout& layout,
		size_t& row, size_t& col)
{
	PushButton*	b	= new PushButton (&dlg, label, dlg);
#ifdef QT_4
	b->setContentsMargins  (0, 0, 0, 0);
#endif	// QT_4
	layout.addWidget (b, row, col++);	

	return b;
}


// ===========================================================================
//                      LA CLASSE QtScientificTextDialog
// ===========================================================================


QtScientificTextDialog::QtScientificTextDialog (
			QWidget* parent, const UTF8String& title, 
			const UTF8String& text, bool editable)
	: QtTextDialog (parent, title, text, editable),
	  _greekPanel (0)
{
	createGui (editable);
}	// QtScientificTextDialog::QtScientificTextDialog


QtScientificTextDialog::QtScientificTextDialog (
			QWidget* parent, const UTF8String& title, 
			const UTF8String& text, bool editable, const QFont& font,
			const QColor& foreground, const QColor& background)
	: QtTextDialog(parent, title, text, editable, font, foreground, background),
	  _greekPanel (0)
{
	createGui (editable);
}	// QtScientificTextDialog::QtScientificTextDialog


void QtScientificTextDialog::createGui (bool editable)
{
	if (true == editable)
	{
		_greekPanel	= new QWidget (this);
		getCentralLayout ( ).addWidget (_greekPanel);
		QGridLayout*	gridLayout	= new QGridLayout (_greekPanel);
		_greekPanel->setLayout (gridLayout);
		gridLayout->setMargin (QtConfiguration::margin);
		gridLayout->setSpacing (QtConfiguration::spacing);
		gridLayout->setSizeConstraint (QLayout::SetFixedSize);

		vector<QtGreekPushButton*>	buttons;
		size_t	row	= 0, col = 0;
		QtGreekPushButton*	b	= 0;
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::alphaMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::betaMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::khiMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::deltaMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::epsilonMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::etaMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::gammaMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::iotaMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::phiMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::kappaMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::lambdaMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::muMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::nuMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::omicronnMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::piMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::thetaMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::roMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::sigmaMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::tauMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::upsilonMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::omegaMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::xsiMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::psiMin ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::dzetaMin ( ), *gridLayout, row, col));
		row++;	col	= 0;

		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::alphaMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::betaMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::khiMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::deltaMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::epsilonMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::etaMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::gammaMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::iotaMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::phiMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::kappaMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::lambdaMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::muMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::nuMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::omicronnMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::piMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::thetaMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::roMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::sigmaMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::tauMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::upsilonMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::omegaMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::xsiMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::psiMaj ( ), *gridLayout, row, col));
		buttons.push_back (addButton<QtGreekPushButton> (
				*this, QtStringHelper::dzetaMaj ( ), *gridLayout, row, col));

		for (vector<QtGreekPushButton*>::iterator it1 = buttons.begin ( );
		     buttons.end ( ) != it1; it1++)
		{
			QSize	min	= (*it1)->minimumSizeHint ( );
			// QSize (height, height) : pour une raison inconnue minimumSizeHint
			// retourne (75, 26), donc une largeur bien trop grande. Valeur min
			// gravée dans le marbre ?
			(*it1)->setFixedSize (min.height ( ), min.height ( ));
		}	// for (vector<QtGreekPushButton*>::iterator it1 = ...
	}	// if (true == editable)
/*
	if (0 != getTextEditor ( ).document ( ))
	{
		connect (getTextEditor ( ).document ( ),
		         SIGNAL (contentsChange (int, int, int)),
		         this, SLOT (contentsChange (int, int, int)));
	}
*/
}	// QtScientificTextDialog::QtScientificTextDialog


QtScientificTextDialog::QtScientificTextDialog (
											const QtScientificTextDialog& dlg)
	: QtTextDialog (0, "", "", false)
{
	assert (0 && "QtScientificTextDialog copy constructor is forbidden.");
}	// QtScientificTextDialog::QtScientificTextDialog (const QtScientificTextDialog&)


QtScientificTextDialog& QtScientificTextDialog::operator = (
												const QtScientificTextDialog&)
{
	assert (0 && "QtScientificTextDialog assignment operator is forbidden.");
	return *this;
}	// QtScientificTextDialog::operator =


QtScientificTextDialog::~QtScientificTextDialog ( )
{
}	// QtScientificTextDialog::~QtScientificTextDialog


void QtScientificTextDialog::contentsChange (
									int pos, int charsRemoved, int charsAdded)
{
/*
	try
	{
		QString	text	= getQText ( );
		QString			added;
		for (size_t i = 0; i < charsAdded; i++)
			added += text [pos + i];
		if (0 != charsAdded)
		{
			QTextDocument*	doc	= getTextEditor ( ).document ( );
			CHECK_NULL_PTR_ERROR (doc)
			const bool	shouldBlock	= doc->signalsBlocked ( );
			doc->blockSignals (true);
			text.replace (
						pos, 1, QtStringHelper::textToUnicodeGreekText (added));
			doc->setPlainText (text);
			QTextCursor	cursor (doc);
			cursor.setPosition(pos + 1);
			getTextEditor ( ).setTextCursor (cursor);
			doc->blockSignals (shouldBlock);
		}
	}
	catch (...)
	{
	}
*/
}	// QtScientificTextDialog::contentsChange


#include "QtUtil/QtValidatedTextField.h"
#include "QtUtil/QtUnicodeHelper.h"
#include "QtUtil/QtGroupBox.h"
#include "QtUtil/QtMessageBox.h"
#include <TkUtil/InternalError.h>

#include <QValidator>
#include <QKeyEvent>
#ifndef QT_5
#include <QtGui/QMessageBox>
#else	// QT_5
#include <QMessageBox>
#endif	// QT_5

#include <assert.h>



USING_UTIL
USING_STD

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


bool	QtValidatedTextField::automaticValidation	= true;
bool	QtValidatedTextField::dialogOnError			= true;
QColor	QtValidatedTextField::errorBackground		= Qt::white;
QColor	QtValidatedTextField::errorForeground		= Qt::red;


QtValidatedTextField::QtValidatedTextField (
					QWidget* parent, bool autoValidation, const char* name)
	: QtTextField (parent, name), _autoValidation (autoValidation)
{
}	// QtValidatedTextField::QtValidatedTextField


QtValidatedTextField::QtValidatedTextField (const QtValidatedTextField&)
	: QtTextField (0, 0), _autoValidation (true)
{
	assert (0 && "QtValidatedTextField::QtValidatedTextField is not allowed.");
}	// QtValidatedTextField::QtValidatedTextField (const QtValidatedTextField&)


QtValidatedTextField& QtValidatedTextField::operator = (
													const QtValidatedTextField&)
{
	assert (0 && "QtValidatedTextField::operator = is not allowed.");
	return *this;
}	// QtValidatedTextField::operator =


QtValidatedTextField::~QtValidatedTextField ( )
{
}	// QtValidatedTextField::~QtValidatedTextField


void QtValidatedTextField::focusInEvent (QFocusEvent* event)
{
	QtTextField::focusInEvent (event);
	setSkin (true);
}	// QtValidatedTextField::focusInEvent


void QtValidatedTextField::focusOutEvent (QFocusEvent* event)
{
	QtTextField::focusOutEvent (event);
	if ((true == automaticValidation) || (true == doAutoValidation ( )))
		validate ( );
}	// QtValidatedTextField::focusOutEvent


void QtValidatedTextField::keyPressEvent (QKeyEvent* event)
{
	assert (0 != event);
	QtTextField::keyPressEvent (event);
	if ((Qt::Key_Return == event->key( )) || (Qt::Key_Enter == event->key( )))
	{
		if ((true == automaticValidation) || (true == doAutoValidation ( )))
			validate ( );
	}
}	// QtValidatedTextField::keyPressEvent


bool QtValidatedTextField::validate ( )
{
	try
	{
		QString				tValue	= text ( );
		int					pos		= 0;
		const QValidator*	v		= validator ( );

		if (0 == v)
		{
			INTERNAL_ERROR (exc, "Absence de validateur.", "QtValidatedTextField::validate")
			throw exc;
		}	// if (0 == v)

		if (QValidator::Acceptable != v->validate (tValue, pos))
		{
			UTF8String	errorMsg (charset);
			errorMsg << "Valeur " << tValue.toStdString ( ) << " incorrecte.\n";
			const QDoubleValidator*	dv	=
									 dynamic_cast<const QDoubleValidator*>(v);
			const QIntValidator*	iv	=
									 dynamic_cast<const QIntValidator*>(v);
			const QRegExpValidator*	rev	=
									 dynamic_cast<const QRegExpValidator*>(v);
			if (0 != dv)
				errorMsg << "La valeur doit être comprise entre "
				         << ios_base::fixed << IN_UTIL setprecision (2)
				         << dv->bottom ( ) << " et " << dv->top ( ) << ".";	
			else if (0 != iv)
				errorMsg << "La valeur doit être comprise entre "
				         << ios_base::fixed << IN_UTIL setprecision (2)
				         << (long)iv->bottom ( ) << " et "
				         << (long)iv->top ( ) << ".";	
			else if (0 != rev)
				errorMsg << "La valeur doit être du type " 
				         << rev->regExp ( ).pattern ( ).toStdString ( ) << ".";
			throw Exception (errorMsg);
		}	// if (QValidator::Acceptable != v->validate (tValue, pos))
		setSkin (true);
	}
	catch (const Exception& exc)
	{
		setSkin (false);
		if (true == dialogOnError)
			QtMessageBox::displayErrorMessage (
							this, "Saisie invalide", exc.getFullMessage ( ));
		return false;
	}
	catch (...)
	{
		setSkin (false);
		if (true == dialogOnError)
			QtMessageBox::displayErrorMessage (
							this, "Saisie invalide", "Erreur non documentée.");
		return false;
	}

	return true;
}	// QtValidatedTextField::validate


void QtValidatedTextField::setSkin (bool normal)
{
	static QColor	defaultBackground, defaultForeground;
	static bool		first	= true;

	QPalette	p	= palette ( );
	if (true == first)
	{
		defaultBackground	= p.base ( ).color ( );
		defaultForeground	= p.text ( ).color ( );
		first	= false;
	}	// if (true == first)

	p.setColor (QPalette::Active,
		QPalette::Base, true == normal ? defaultBackground : errorBackground);
	p.setColor (QPalette::Active,
		QPalette::Text, true == normal ? defaultForeground : errorForeground);
	setPalette (p);
}	// QtValidatedTextField::setSkin



#include "QtUtil/QtRotationDialog.h"
#include "QtUtil/QtConfiguration.h"
#include "QtUtil/QtUnicodeHelper.h"
#include "QtUtil/QtGroupBox.h"
#include "QtUtil/QtMessageBox.h"

#include <TkUtil/Exception.h>

#include <QValidator>
#ifndef QT_5
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QButtonGroup>
#include <QtGui/QMessageBox>
#else	// QT_5
#include <QLabel>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>
#include <QMessageBox>
#endif	// QT_5

#include <assert.h>



USING_STD
USING_UTIL

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtRotationDialog::QtRotationDialog (
			QWidget* parent, const UTF8String& title, 
			const UTF8String& axe1Label,
			const UTF8String& axe2Label,
			const UTF8String& axe3Label,
			unsigned char defaultAxe, double angle,
			const UTF8String& angleUnit,
			double angleMin, double angleMax,
			const string& helpURL, const string& helpTag)
	: QDialog (parent, QtConfiguration::modalDialogWFlags),
	  _axe1Button (0), _axe2Button (0), _axe3Button (0),
	  _angleTextField (0), _closurePanel (0)
{	
	setModal (true);
	setWindowTitle (UTF8TOQSTRING (title));

	// Creation de l'ihm :
	QVBoxLayout*	layout	= new QVBoxLayout (this);
	layout->setMargin (QtConfiguration::margin);
	layout->setSizeConstraint (QLayout::SetMinimumSize);

	// Le panneau :
	QGroupBox*		buttons	= new QGroupBox ("Axes de rotation", this);
	QVBoxLayout*	buttonsLayout	= new QVBoxLayout ( );
	buttons->setLayout (buttonsLayout);
	QButtonGroup*	buttonsGroup	= new QButtonGroup ( );
	buttonsGroup->setExclusive (true);
	buttons->setContentsMargins (
					QtConfiguration::margin, 25 + QtConfiguration::margin,
					QtConfiguration::margin, QtConfiguration::margin);
	layout->addWidget (buttons);
	// Les axes :
	_axe1Button		= new QRadioButton (UTF8TOQSTRING (axe1Label), buttons);
	_axe2Button		= new QRadioButton (UTF8TOQSTRING (axe2Label), buttons);
	_axe3Button		= new QRadioButton (UTF8TOQSTRING (axe3Label), buttons);
	switch (defaultAxe)
	{
		case	1 :	_axe2Button->setChecked (true);	break;
		case	2 : _axe3Button->setChecked (true); break;
		default	  : _axe1Button->setChecked (true);
	}
	buttonsLayout->addWidget (_axe1Button);
	buttonsLayout->addWidget (_axe2Button);
	buttonsLayout->addWidget (_axe3Button);
	buttonsGroup->addButton (_axe1Button);
	buttonsGroup->addButton (_axe2Button);
	buttonsGroup->addButton (_axe3Button);

	// L'angle de rotation :
	layout->addWidget (new QLabel (" ", this));
	QWidget*		hbox		= new QWidget (this);
	QHBoxLayout*	hboxLayout	= new QHBoxLayout ( );
	hbox->setLayout (hboxLayout);
	hboxLayout->setMargin (QtConfiguration::margin);
	hboxLayout->setSpacing (QtConfiguration::spacing);
	layout->addWidget (hbox);
	QLabel*	label	= new QLabel ("Angle : ", hbox);
	_angleTextField	= new QtTextField (UTF8TOQSTRING (UTF8String(angle)), hbox);	
	QDoubleValidator*	validator	= new QDoubleValidator (_angleTextField);
	validator->setRange (angleMin, angleMax);
	validator->setDecimals (8);
	_angleTextField->setValidator (validator);
	QLabel*	label2	= new QLabel (UTF8TOQSTRING (angleUnit), hbox);
	hboxLayout->addWidget (label);
	hboxLayout->addWidget (_angleTextField);
	hboxLayout->addWidget (label2);
	
	// Les boutons :
	layout->addWidget (new QLabel (" ", this));
	_closurePanel	= 
		new QtDlgClosurePanel (this, false, "Appliquer", "", "Annuler", helpURL,
		                       helpTag);
	layout->addWidget (_closurePanel);
	_closurePanel->setMinimumSize (_closurePanel->sizeHint ( ));
	connect (_closurePanel->getApplyButton ( ), SIGNAL(clicked ( )), this,
	         SLOT(accept ( )));
	connect (_closurePanel->getCancelButton ( ), SIGNAL(clicked ( )), this,
	         SLOT(reject ( )));

	// Par defaut le bouton OK est artificellement clique par QDialog quand
	// l'utilisateur fait return dans un champ de texte => on inhibe ce
	// comportement par defaut :
	_closurePanel->getApplyButton ( )->setAutoDefault (false);
	_closurePanel->getApplyButton ( )->setDefault (false);
	_closurePanel->getCancelButton ( )->setAutoDefault (false);
	_closurePanel->getCancelButton ( )->setDefault (false);

	layout->activate ( );
	setMinimumSize (layout->sizeHint ( ));
}	// QtRotationDialog::QtRotationDialog


QtRotationDialog::QtRotationDialog (const QtRotationDialog&)
{
	assert (0 && "QtRotationDialog copy constructor is forbidden.");
}	// QtRotationDialog::QtRotationDialog (const QtRotationDialog&)


QtRotationDialog& QtRotationDialog::operator = (const QtRotationDialog&)
{
	assert (0 && "QtRotationDialog assignment operator is forbidden.");
	return *this;
}	// QtRotationDialog::operator =


QtRotationDialog::~QtRotationDialog ( )
{
}	// QtRotationDialog::~QtRotationDialog


unsigned char QtRotationDialog::getAxe ( ) const
{	
	assert (0 != _axe1Button);
	assert (0 != _axe2Button);
	assert (0 != _axe3Button);

	if (true == _axe1Button->isChecked ( ))
		return 0;
	else if (true == _axe2Button->isChecked ( ))
		return 1;

	return 2;
}	// QtRotationDialog::getAxe 


double QtRotationDialog::getAngle ( ) const
{
	assert (0 != _angleTextField);
	bool	ok		= true;
	QString	text	= _angleTextField->text ( );
	double	angle	= text.toDouble (&ok);
	int		pos		= 0;
	const QValidator*		v			= _angleTextField->validator ( );
	const QDoubleValidator*	validator	=
								dynamic_cast<const QDoubleValidator*>(v);

	if ((false == ok) || ((0 != validator) &&
			(QValidator::Acceptable != validator->validate (text, pos))))
	{
		UTF8String		errorMsg (charset);
		errorMsg << "Valeur incorrecte de l'angle.";
		if (0 != validator)
		{
			errorMsg << "\nL'angle doit être compris entre "
			         << ios_base::fixed << IN_UTIL setprecision (2)
		             << validator->bottom ( ) << " et " << validator->top ( )
			         <<".";
		}	// if (0 != validator)
		throw Exception (errorMsg);
	}	// if (false == ok)

	return angle;
}	// QtRotationDialog::getAngle


void QtRotationDialog::accept ( )
{
	UTF8String	title	= QtUnicodeHelper::qstringToUTF8String (windowTitle( ));
	bool		close	= true;

	try
	{
		if (0. == getAngle ( ))
		{
			UTF8String	message (charset);
			message << "Angle nul. Continuer ?";
			if (0 != QtMessageBox::displayWarningMessage (
							this, title, message, 100,
							"Oui", "Annuler", 0, -1))
/*			if (0 != QMessageBox::warning (
							this, title, message.iso ( ).c_str ( ),
							"Oui", "Annuler", QString::null, 0, -1)) */
				return;
		}	// if (0. == getAngle ( ))
	}
	catch (const Exception& exc)
	{
//		QString	msg	=
//				QtUnicodeHelper::unicodeStringToQString (exc.getFullMessage( ));
		QtMessageBox::displayErrorMessage (
									this, title, exc.getFullMessage ( ));
//		QMessageBox::critical ( this, title, msg);
		close	= false;
	}
	catch (...)
	{
		QtMessageBox::displayErrorMessage (
									this, title, "Erreur non documentée.");
//		QMessageBox::critical ( this, title, "Erreur non documentée.");
		close	= false;
	}

	if (true == close)
		QDialog::accept ( );
}	// QtRotationDialog::accept

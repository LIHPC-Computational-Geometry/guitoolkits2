
#include "QtUtil/QtPenPanel.h"
#include "QtUtil/QtConfiguration.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/Exception.h>
#include <TkUtil/MemoryError.h>

#include <QPen>
#ifndef QT_5
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QColorDialog>
#else	// QT_5
#include <QLabel>
#include <QLayout>
#include <QColorDialog>
#endif	// QT_5

#include <assert.h>


USING_UTIL
USING_STD

static const Charset	charset ("àéèùô");

USE_ENCODING_AUTODETECTION


static QBoxLayout*	createSecondaryLayout (Qt::Orientations orientation)
{
	if (Qt::Horizontal == orientation)
		return new QVBoxLayout ( );

	return new QHBoxLayout ( );
}	// createSecondaryLayout


static QBoxLayout*	createInvertedSecondaryLayout (Qt::Orientations orientation)
{
	if (Qt::Horizontal == orientation)
		return new QHBoxLayout ( );

	return new QVBoxLayout ( );
}	// createInvertedSecondaryLayout


QtPenPanel::QtPenPanel (QWidget* parent, Qt::Orientations orientation,
                        bool undeterminate, const QColor color, uint width)
	: QWidget (parent),
	  _allowsUndeterminate (undeterminate), _color (color),
	  _colorButton (0), _undeterminateColorCheckbox (0), _widthTextField (0),
	  _styleComboBox (0)
{

	QBoxLayout*	mainLayout	= 0;
	if (Qt::Horizontal == orientation)
		mainLayout	= new QHBoxLayout (this);
	else
		mainLayout	= new QVBoxLayout (this);
	mainLayout->setSizeConstraint (QLayout::SetMinimumSize);

	// Couleur :
	QBoxLayout*	vLayout	= createSecondaryLayout (orientation);
	mainLayout->addLayout (vLayout);
	_colorButton	= new QPushButton ("Couleur ...", this);
	vLayout->addWidget (_colorButton);
	connect (_colorButton, SIGNAL (clicked ( )), this,
	         SLOT (editColorCallback ( )));
	if (true == _allowsUndeterminate)
	{
		_undeterminateColorCheckbox	=
							new QCheckBox (QSTR ("Couleur indéterminée"), this);
		vLayout->addWidget (_undeterminateColorCheckbox);
		_undeterminateColorCheckbox->setChecked (!_color.isValid ( ));
		_colorButton->setEnabled (!_undeterminateColorCheckbox->isChecked ( ));
		connect (_undeterminateColorCheckbox, SIGNAL (stateChanged (int)), this,
		         SLOT (undeterminateColorCallback (int)));
	}	// if (true == _allowsUndeterminate)

	// Epaisseur :
	QHBoxLayout*	hLayout	= new QHBoxLayout ( );
	mainLayout->addLayout (hLayout);
	QLabel*	label	= new QLabel ("Epaisseur :", this);
	hLayout->addWidget (label);
	_widthTextField	= new QtIntTextField (width, this);
	_widthTextField->setRange (true == _allowsUndeterminate ? -1 : 0, 1000);
	hLayout->addWidget (_widthTextField);

	// Style :
	hLayout	= new QHBoxLayout ( );
	mainLayout->addLayout (hLayout);
	label	= new QLabel ("Style :");
	hLayout->addWidget (label);
	_styleComboBox	= new QComboBox (this);
	hLayout->addWidget (_styleComboBox);
	for (size_t ps = Qt::NoPen; ps < Qt::CustomDashLine; ps++)
	{
		switch ((Qt::PenStyle)ps)
		{
			case Qt::NoPen			:
				 _styleComboBox->addItem (
					QIcon (":/images/empty_line.png"), "Sans trait");
				break;
			case Qt::SolidLine		:
				 _styleComboBox->addItem (
					QIcon (":/images/solid_line.png"), "Ligne solide");
				break;
			case Qt::DashLine		:
				 _styleComboBox->addItem (
					QIcon (":/images/dash_line.png"), "Tirets");
				break;
			case Qt::DotLine		:
				 _styleComboBox->addItem (
					QIcon (":/images/dot_line.png"), QSTR ("Pointillés"));
				break;
			case Qt::DashDotLine	:
				 _styleComboBox->addItem (
					QIcon (":/images/dash_dot_line.png"), "Tiret-Point");
				break;
			case Qt::DashDotDotLine	:
				 _styleComboBox->addItem (
					QIcon (":/images/dash_dot_dot_line.png"), "Tiret-2 Points");
				break;
		}	// switch ((Qt::PenStyle)ps)
	}	// for (Qt::PenStyle ps = Qt::NoPen; ps < Qt::CustomDashLine; ps++)
	if (true == _allowsUndeterminate)
		_styleComboBox->addItem (QSTR ("Indéterminé"));
	
	setLayout (mainLayout);
}	// QtPenPanel::QtPenPanel


QtPenPanel::QtPenPanel (const QtPenPanel& panel)
	: QWidget (0), _color (panel._color), _widthTextField (0)
{
	assert (0 && "QtPenPanel copy constructor is not allowed.");
}	// QtPenPanel::QtPenPanel (const QtPenPanel& view)


QtPenPanel& QtPenPanel::operator = (const QtPenPanel& view)
{
	assert (0 && "QtPenPanel::operator = is not allowed.");
	return *this;
}	// QtPenPanel::QtPenPanel (const QtPenPanel& view)


QtPenPanel::~QtPenPanel ( )
{
}	// QtPenPanel::~QtPenPanel


QColor QtPenPanel::getColor ( ) const
{
	if ((0 != _undeterminateColorCheckbox) &&
	    (true == _undeterminateColorCheckbox->isChecked ( )))
		return _color.convertTo (QColor::Invalid);

	return _color;
}	// QtPenPanel::getColor


void QtPenPanel::setColor (const QColor& color)
{
	_color	= color;
	if (0 != _undeterminateColorCheckbox)
		_undeterminateColorCheckbox->setChecked (!_color.isValid ( ));
}	// QtPenPanel::setColor


uint QtPenPanel::getWidth ( ) const
{
	assert (0 != _widthTextField);
	return _widthTextField->getValue ( );
}	// QtPenPanel::getWidth


void QtPenPanel::setWidth (uint width) 
{
	assert (0 != _widthTextField);
	_widthTextField->setValue (width);
}	// QtPenPanel::setWidth


Qt::PenStyle QtPenPanel::getStyle ( ) const
{
	assert (0 != _styleComboBox);
	if ((true == allowsUndeterminate ( )) &&
	    (_styleComboBox->currentIndex ( ) == (_styleComboBox->count ( ) - 1)))
		return (Qt::PenStyle)-1;

	return (Qt::PenStyle)_styleComboBox->currentIndex ( );
}	// QtPenPanel::getStyle


void QtPenPanel::setStyle (Qt::PenStyle style) 
{
	assert (0 != _styleComboBox);
	if ((true == allowsUndeterminate ( )) && ((Qt::PenStyle)-1 == style))
		_styleComboBox->setCurrentIndex (_styleComboBox->count ( ) - 1);
	else
		_styleComboBox->setCurrentIndex ((int)style);
}	// QtPenPanel::setStyle


QPen QtPenPanel::getPen ( ) const
{
	QPen	pen;
	if ((Qt::PenStyle)-1 != getStyle ( ))
		pen.setStyle (getStyle ( ));
	QColor	color	= getColor ( );
	if (true == color.isValid ( ))
		pen.setColor (color);
	if (-1 != getWidth ( ))
		pen.setWidth (getWidth ( ));

	return pen;
}	// QtPenPanel::getPen


void QtPenPanel::setPen (const QPen& pen)
{
	setStyle (pen.style ( ));
	setColor (pen.color ( ));
	setWidth (pen.width ( ));
}	// QtPenPanel::setPen


void QtPenPanel::editColorCallback ( )
{
	QColor	color	= QColorDialog::getColor (_color, this);
	if (true == color.isValid ( ))
		_color	= color;
}	// QtPenPanel::editColorCallback


void QtPenPanel::undeterminateColorCallback (int)
{
	assert (0 != _colorButton);
	assert (0 != _undeterminateColorCheckbox);
	_colorButton->setEnabled (!_undeterminateColorCheckbox->isChecked ( ));
}	// QtPenPanel::undeterminateColorCallback



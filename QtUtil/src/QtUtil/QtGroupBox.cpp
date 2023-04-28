#include "QtUtil/QtGroupBox.h"

#include <assert.h>


QtGroupBox::QtGroupBox (QWidget* parent, const char* name)
	: QGroupBox (name, parent),
	   _layout (0)
{
}	// QtGroupBox::QtGroupBox


QtGroupBox::QtGroupBox (const QString& title, QWidget* parent, const char* name)
	: QGroupBox (title, parent),
	  _layout (0)
{
	setWindowTitle (title);
}	// QtGroupBox::QtGroupBox


QtGroupBox::QtGroupBox (const QtGroupBox&)
	: QGroupBox (0, 0)
{
	assert (0 && "QtGroupBox::QtGroupBox is not allowed.");
}	// QtGroupBox::QtGroupBox (const QtGroupBox&)


QtGroupBox& QtGroupBox::operator = (const QtGroupBox&)
{
	assert (0 && "QtGroupBox::operator = is not allowed.");
	return *this;
}	// QtGroupBox::operator =


QtGroupBox::~QtGroupBox ( )
{
}	// QtGroupBox::~QtGroupBox


void QtGroupBox::setLayout (QLayout* layout)
{
	assert ((0 == _layout) && "QtGroupBox::setLayout : layout is already set.");
	_layout	= layout;
	QGroupBox::setLayout (layout);
	setMargin (0);		// Assure une marge de la hauteur de la fonte.
}	// QtGroupBox::setLayout


void QtGroupBox::setMargin (int margin)
{
	if (0 == _layout)
		return;

	// On tient compte de la zone d'ecriture du nom du groupe :
	QFontMetrics	metrics	= fontMetrics ( );
	int		fontHeight	= metrics.ascent ( ) + metrics.descent ( );

	_layout->setMargin (margin + fontHeight);
}	// QtGroupBox::setMargin


void QtGroupBox::setSpacing (int spacing)
{
	if (0 == _layout)
		return;

	_layout->setSpacing (spacing);
}	// QtGroupBox::setSpacing


QSize QtGroupBox::sizeHint ( ) const
{
	if (0 == getLayout ( ))
		return QGroupBox::sizeHint ( );
	else
	{
//		getLayout ( )->activate ( );	// Risque de boucle Qt infinie.
		int	left	= 0, right	= 0, top	= 0, bottom	= 0, framew	= 0;	
		getContentsMargins (&left, &top, &right, &bottom);
		int	marg	= left;

		// On tient compte de la marge et de l'epaisseur du frame :
		int				width	= _layout->sizeHint ( ).width ( ) + 
								  2 * marg + 2 * framew;
		int				height	= _layout->sizeHint ( ).height ( ) +
								  2 * marg + 2 * framew;

		return QSize (width, height);
	}	// else if (0 == getLayout ( ))
}	// QtGroupBox::sizeHint


void QtGroupBox::adjustSize ( )
{
	if (0 != _layout)
	{
		_layout->activate ( );
		_layout->setSizeConstraint (QLayout::SetMinimumSize);
/*
setMinimumSize (_layout->sizeHint ( ));
resize (_layout->sizeHint ( ));
*/
	}
//else
	setMinimumSize (sizeHint ( ));
}	// QtGroupBox::adjustSize



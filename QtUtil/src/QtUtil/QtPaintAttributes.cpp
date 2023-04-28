
#include "QtUtil/QtPaintAttributes.h"

#include <QBrush>


USING_UTIL


QtPaintAttributes::QtPaintAttributes (const PaintAttributes& pa)
	: _pen ( )
{
	switch (pa.getLineStyle ( ))
	{
		case PaintAttributes::NO_LINE				:
			_pen.setStyle (Qt::NoPen);			break;
		case PaintAttributes::SOLID_LINE			:
			_pen.setStyle (Qt::SolidLine);		break;
		case PaintAttributes::DASH_LINE				:
			_pen.setStyle (Qt::DashLine);		break;
		case PaintAttributes::DOT_LINE				:
			_pen.setStyle (Qt::DotLine);		break;
		case PaintAttributes::DASH_DOT_LINE			:
			_pen.setStyle (Qt::DashDotLine);	break;
		case PaintAttributes::DASH_DOT_DOT_LINE		:
			_pen.setStyle (Qt::DashDotDotLine);	break;
	}	// switch (pa.getLineStyle ( ))
	_pen.setWidth (pa.getLineWidth ( ));
	_pen.setColor (QColor (pa.getColor ( ).getRed ( ),
	                       pa.getColor ( ).getGreen ( ),
	                       pa.getColor ( ).getBlue ( )));
	_pen.setBrush (QBrush (QColor (pa.getFillingColor ( ).getRed ( ),
	                               pa.getFillingColor ( ).getGreen ( ),
	                               pa.getFillingColor ( ).getBlue ( ))));
}	// QtPaintAttributes::QtPaintAttribute


QtPaintAttributes::QtPaintAttributes (const QPen& pen)
	: _pen (pen)
{
}	// QtPaintAttributes::QtPaintAttributes


QtPaintAttributes::QtPaintAttributes (const QtPaintAttributes& qpa)
	: _pen (qpa._pen)
{
}	// QtPaintAttributes::QtPaintAttributes


QtPaintAttributes& QtPaintAttributes::operator = (const QtPaintAttributes& qpa)
{
	if (&qpa != this)
	{
		_pen	= qpa._pen;
	}	// if (&qpa != this)

	return *this;
}	// QtPaintAttributes::operator =


QtPaintAttributes::~QtPaintAttributes ( )
{
}	// QtPaintAttributes::~QtPaintAttributes


PaintAttributes QtPaintAttributes::getPaintAttributes ( ) const
{
	const QPen		pen (getPen ( ));
	const QColor	color (pen.color ( ));
	PaintAttributes	pa (Color (color.red ( ), color.green ( ), color.blue ( )));
	switch (pen.style ( ))
	{
		case Qt::NoPen				:
			pa.setLineStyle (PaintAttributes::NO_LINE);				break;
		case Qt::SolidLine				:
			pa.setLineStyle (PaintAttributes::SOLID_LINE);			break;
		case Qt::DashLine				:
			pa.setLineStyle (PaintAttributes::DASH_LINE);			break;
		case Qt::DotLine				:
			pa.setLineStyle (PaintAttributes::DOT_LINE);			break;
		case Qt::DashDotLine				:
			pa.setLineStyle (PaintAttributes::DASH_DOT_LINE);		break;
		case Qt::DashDotDotLine				:
			pa.setLineStyle (PaintAttributes::DASH_DOT_DOT_LINE);	break;
	}	// switch (pen.style ( ))
	pa.setLineWidth (pen.width ( ));
	const QBrush	brush (pen.brush ( ));
	const QColor	filling (brush.color ( ));
	pa.setFillingColor (
				Color (filling.red ( ), filling.green ( ), filling.blue ( )));

	return pa;
}	// QtPaintAttributes::getPaintAttributes



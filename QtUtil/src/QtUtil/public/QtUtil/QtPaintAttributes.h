#ifndef QT_PAINT_ATTRIBUTES_H
#define QT_PAINT_ATTRIBUTES_H

#include <TkUtil/PaintAttributes.h>

#include <QPen>


/**
 * Classe assurant la transposition <I>Qt</I> vers <I>PaintAttributes</I> et
 * <I>PaintAttributes</I> vers <I>Qt</I>.
 */
class QtPaintAttributes
{
	public :

	/**
	 * Constructeurs et opérateur =. RAS.
	 */
	QtPaintAttributes (const IN_UTIL PaintAttributes& pa);
	QtPaintAttributes (const QPen& pen);
	QtPaintAttributes (const QtPaintAttributes& qpa);
	QtPaintAttributes& operator = (const QtPaintAttributes& qpa);

	/**
	 * Destructeur. RAS.
	 */
	~QtPaintAttributes ( );

	/**
	 * @return		Le stylo associé.
	 */
	QPen getPen ( ) const
	{ return _pen; }

	/**
	 * @return		La représentation <I>PaintAttributes</I> de l'instance.
	 */
	IN_UTIL PaintAttributes getPaintAttributes ( ) const;


	private :

	/** Le stylo associé. */
	QPen			_pen;
};	// class QtPaintAttributes

#endif	// QT_PAINT_ATTRIBUTES_H

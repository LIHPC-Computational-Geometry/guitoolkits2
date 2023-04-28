#ifndef PAINT_ATTRIBUTES_H
#define PAINT_ATTRIBUTES_H


#include <TkUtil/util_config.h>

#include <unistd.h>


BEGIN_NAMESPACE_UTIL

/**
 * Classe décrivant une couleur.
 * Cette version ne supporte que le système RGB où chaque composante est codée
 * par un entier compris entre 0 et 255. Accès possibles au format
 * <I>double</I> (composantes comprises entre 0. et 1.), et au format <I>HSV</I>
 * codé sur des <I>double</I>.
 */
class Color
{
	public :

	Color (unsigned char red, unsigned char green, unsigned char blue)
		: _red (red), _green (green), _blue (blue), _valid (true)
	{ }
	Color (const Color& c)
		: _red (c._red), _green (c._green), _blue (c._blue), _valid (true)
	{ }
	Color& operator = (const Color& c)
	{
		if (&c != this)
		{
			_red	= c._red;
			_green	= c._green;
			_blue	= c._blue;
			_valid	= c._valid;
		}

		return *this;
	}	// operator =
	/**
 	 * Création à partir de composantes réelles : les composantes doivent
 	 * être comprises entre 0 et 1 (pas de vérification).
 	 * Rem : pas de constructeur réel (<I>Color (double, double, double)</I>,
 	 * le compilateur hésite entre celui-là et celui <I>unsigned char</I> dans
 	 * le cas <I>Color (0, 0, 0)</I>.
 	 */
	static Color create (double red, double green, double blue);

	bool operator == (const Color& c) const
	{
		if ((getRed ( ) != c.getRed ( )) || (getGreen ( ) != c.getGreen ( )) ||
		    (getBlue ( ) != c.getBlue ( )))
			return false;
		if (isValid ( ) != c.isValid ( ))
			return false;

		return true;
	}	// operator ==

	bool operator != (const Color& c) const
	{ return ! (*this == (c)); }
	/**
	 * @return	<I>true</I> si les 2 couleurs sont comparables, composantes
	 *			<I>RGB</I> par composante, à <I>percent<I> près, où
	 *			<I>percent<I> est comparé à la différence entre composantes et
	 *			255. <I>percent<I> doit être compris entre 0. et 1.
	 * @see	hsvNear
	 */
	bool rgbNear (const Color& color, double percent) const;

	/**
	 * @return	<I>true</I> si les 2 couleurs sont comparables, composantes
	 *			<I>RGB</I> par composante, à <I>percent<I> près, où
	 *			<I>percent<I> est comparé à la différence entre composantes et
	 *			255. <I>percent<I> doit être compris entre 0. et 1.
	 * @see	rgbNear
	 */
	bool hsvNear (const Color& color, double percent) const;


	unsigned char getRed ( ) const
	{ return _red; }
	unsigned char getGreen ( ) const
	{ return _green; }
	unsigned char getBlue ( ) const
	{ return _blue; }
	void getRGB (unsigned char& red, unsigned char& green, unsigned char& blue) const
	{
		red		= _red;
		green	= _green;
		blue	= _blue;
	}	// getRGB
	void setRed (unsigned char red)
	{ _red	= red; }
	void setGreen (unsigned char green)
	{ _green	= green; }
	void setBlue (unsigned char blue)
	{ _blue	= blue; }
	void setRGB (unsigned char red, unsigned char green, unsigned char blue)
	{
		_red	= red;
		_green	= green;
		_blue	= blue;
	}	// setRGB
	void getDRGB (double& red, double& green, double& blue) const;
	void setDRGB (double red, double green, double blue);
	double getDRed ( ) const
	{ return (double)_red / 255.; }
	double getDGreen ( ) const
	{ return (double)_green / 255.; }
	double getDBlue ( ) const
	{ return (double)_blue / 255.; }
	void setDRed (double red)
	{ _red	= (unsigned char)(red * 255.); }
	void setDGreen (double green)
	{ _green	= (unsigned char)(green * 255.); }
	void setDBlue (double blue)
	{ _blue	= (unsigned char)(blue * 255.); }
	/**
	 * @return	Les composantes, au format <I>HSV</I>, avec
	 * 			0 <= <I>hue</I> <= 360., 0. <= <I>saturation</I> <= 1., 
	 * 			<I>0. <= <I>saturation</I> <= 1.
	 */
	void getHSV (double& hue, double& saturation, double& value) const;

	/**
	 * @return	<I>true</I> si la couleur est valide, <I>false</I> dans le cas
	 * 			contraire.
	 */
	bool isValid ( ) const
	{ return _valid; }

	/**
	 * @param	<I>true</I> si la couleur est valide, <I>false</I> dans le cas
	 * 			contraire.
	 */
	void setValid (bool valid)
	{ _valid	= valid; }

	/**
	 * Inverse les composantes <I>RGB</I> de la couleur.
	 */
	void invert ( );

	/**
	 * @return		une couleur, dépendant de la couleur reçue en 
	 *				argument. Ses composantes double RGB sont incrémentées
	 *				avec les 3 derniers arguments, puis normalisées entre
	 *				0 et 1.
	 * @warning		Les 3 derniers arguments doivent être compris entre 0 et 1.
	 */
	static Color getNextColor (const Color& initialColor,
				double redInc, double greenInc, double blueInc);

	/**
	 * @return		une couleur, dépendant de la couleur reçue en 
	 *				argument. Ses composantes RGB sont incrémentées
	 *				avec les 3 derniers arguments.
	 * @warning		Les 3 derniers arguments doivent être compris entre 0
	 *				et 255.
	 */
	static Color getNextColor (const Color& initialColor,
				unsigned char redInc, unsigned char greenInc, unsigned char blueInc);

	/**
	 * Quelques couleurs élémentaires.
	 */
	static const Color			black, white, red, green, blue, cyan, magenta,
								yellow, gray, orange, brown, pink;


	private :

	unsigned char	_red, _green, _blue;
	bool			_valid;
};	// class Color


/**
 * Classe décrivant les paramètres graphiques de représentation d'un objet.
 * Cette classe présente l'intérêt d'être totalement indépendante des APIs
 * graphiques (X11, Motif, Qt, ...).
 */
class PaintAttributes
{
	public :

	/**
	 * Le style de trait.
	 */
	enum LineStyle { NO_LINE, SOLID_LINE, DASH_LINE, DOT_LINE, DASH_DOT_LINE,
	                 DASH_DOT_DOT_LINE };

	/**
	 * Constructeur par défaut.
	 * Initialise les couleurs à celle du trait, l'épaisseur du trait à 1 pixel,
	 * le style de ligne à <I>SOLID_LINE</I>.
	 * @param		Couleur du tracé.
	 */
	PaintAttributes (Color color)
	 : _color (color), _fillingColor (color),
	   _lineWidth (1), _lineStyle (SOLID_LINE)
	{ }

	/**
	 * Constructeur de copie et opérateur = : RAS.
	 */
	PaintAttributes (const PaintAttributes& pa)
		: _color (pa._color), _fillingColor (pa._fillingColor),
		  _lineWidth (pa._lineWidth), _lineStyle (pa._lineStyle)
	{ }	// PaintAttributes (const PaintAttributes& pa)

	PaintAttributes& operator = (const PaintAttributes& pa)
	{	
		if (&pa != this)
		{
			_color			= pa._color;
			_fillingColor	= pa._fillingColor;
			_lineWidth		= pa._lineWidth;
			_lineStyle		= pa._lineStyle;
		}	// if (&pa != this)

		return *this;
	}	// operator =

	/**
	 * Destructeur. RAS.
	 */
	~PaintAttributes ( )
	{ }

	/**
	 * @return		Une instance possible de représentation par défaut.
	 *				Les instances retournées ne diffèrent que par la couleur
	 *				fonction de l'entier tranmis en argument. Les autres
	 *				paramètres sont ceux affectés lors de la construction par
	 *				défaut.
	 */
	static PaintAttributes create (size_t i);

	/**
	 * @return		<I>true</I> si les paramètres des deux instances sont
	 *				identiques, <I>false</I> dans le cas contraire.
	 */
	bool operator == (const PaintAttributes& pa) const
	{
		if ((getColor ( ) != pa.getColor ( )) ||
		    (getFillingColor ( ) != pa.getFillingColor ( )))
			return false;
		if ((getLineWidth ( ) != pa.getLineWidth ( )) ||
		    (getLineStyle ( ) != pa.getLineStyle ( )))
			return false;

		return true;
	}	// operator ==

	/**
	 * @return		<I>true</I> si les paramètres des deux instances sont
	 *				différents, <I>false</I> dans le cas contraire.
	 */
	bool operator != (const PaintAttributes& pa) const
	{ return ! (pa == *this); }

	/**
	 * @return		La couleur de tracé.
	 */
	const Color& getColor ( ) const
	{ return _color; }

	/**
	 * @param		La nouvelle couleur de tracé.
	 */
	void setColor (const Color& color) 
	{ _color	= color; }

	/**
	 * @return		La couleur de remplissage.
	 */
	const Color& getFillingColor ( ) const
	{ return _fillingColor; }

	/**
	 * @param		La nouvelle couleur de remplissage.
	 */
	void setFillingColor (const Color& color) 
	{ _fillingColor	= color; }

	/**
	 * @return		L'épaisseur du trait.
	 */
	unsigned short getLineWidth ( ) const
	{ return _lineWidth; }

	/**
	 * @param		La nouvelle épaisseur du trait.
	 */
	void setLineWidth (unsigned int width)
	{ _lineWidth	= width; }

	/**
	 * @return		Le style du trait.
	 */
	LineStyle getLineStyle ( ) const
	{ return _lineStyle; }

	/**
	 * @param		Le nouveau style du trait.
	 */
	void setLineStyle (LineStyle style)
	{ _lineStyle	= style; }


	private :

	/** La couleur du trait. */
	Color			_color;

	/** La couleur de remplissage. */
	Color			_fillingColor;

	/** L'épaisseur du trait. */
	unsigned int	_lineWidth;

	/** Le style du trait. */
	LineStyle		_lineStyle;
};	// class PaintAttributes


END_NAMESPACE_UTIL


#endif	// PAINT_ATTRIBUTES_H

#include "TkUtil/PaintAttributes.h"
#include "TkUtil/InternalError.h"
#include "TkUtil/UTF8String.h"

#include <algorithm>	// std::min, max
#include <cmath>
#include <assert.h>


BEGIN_NAMESPACE_UTIL


const Color	Color::black (0, 0, 0);
const Color	Color::white (255, 255, 255);
const Color	Color::red (255, 0, 0);
const Color	Color::green (0, 255, 0);
const Color	Color::blue (0, 0, 255);
const Color	Color::cyan (0, 255, 255);
const Color	Color::magenta (255, 0, 255);
const Color	Color::yellow (255, 255, 0);
const Color	Color::gray (128, 128, 128);
const Color	Color::orange (255, 166, 0);
const Color	Color::brown (166, 41, 41);
const Color	Color::pink (255, 191, 204);

static const Charset	charset ("àéèùô");


/**
 * @return      La valeur transmise en argument de manière à ce qu'elle soit
 *              comprise entre 0 et 1.
 */
static double normalize (double value)
{
	assert ((value >= -1.) && "normalize : value < -1");
	assert ((value <= 2.) && "normalize : value > 2");
	if (value < 0.)
		value   += 1;
	else if (value > 1.)
		value   -= 1;

	assert ((value >= 0.) && (value <= 1.) && "normalize failed.");
	return  value;
}   // normalize


PaintAttributes PaintAttributes::create (size_t num)
{
	Color		color (0, 0, 0);
	const int	modNum	= num % 6;
	const int	divNum	= num / 6;
    // Couleur de départ : éviter des (1, 0, 0), (1, 1, 0) et autres qui dès le
    // premier appel à normalize, avec i = 1, deviennent identiques.
	switch (modNum)
	{
		case	0	: color	= Color (225, 0, 125);		break;
		case	1	: color	= Color (125, 0, 225);		break;
		case	2	: color	= Color (225, 125, 0);		break;
		case	3	: color	= Color (0, 225, 160);		break;
		case	4	: color	= Color (160, 0, 225);		break;
		case	5	: color	= Color (200, 160, 125);	break;
// coverity : default ne peut arriver. Programmation défensive, si on remplace 6
		default	 	: color	= Color (75, 160, 210);
	}   // switch (modNum)

	for (int i = 0; i < divNum; i++)
	{
		double	red		= normalize (color.getRed ( ) / 255. + (-.66));
		double	green	= normalize (color.getGreen ( ) / 255. + .33);
		double	blue	= normalize (color.getBlue ( ) / 255. + (-.15));
		color.setRGB (
			(size_t)(red * 255), (size_t)(green * 255), (size_t)(blue * 255));
	}	// for (int i = 0; i < divNum; i++)

	return PaintAttributes (color);
}	// PaintAttributes::create


Color Color::create (double red, double green, double blue)
{
	return Color ((unsigned char)(255. * red), (unsigned char)(255. * green), (unsigned char)(255. * blue));
}	// Color::create


inline  double relativeDistance (double d1, double d2, double max)
{
	return std::fabs ((d2 - d1) / max);
}   // relativeDistance


bool Color::rgbNear (const Color& color, double percent) const
{
	if ((relativeDistance (_red, color.getRed ( ), 255.) > percent) ||
	    (relativeDistance (_green, color.getGreen ( ), 255.) > percent) ||
	    (relativeDistance (_blue, color.getBlue ( ), 255.) > percent))
		return false;

	return true;
}	// Color::rgbNear


bool Color::hsvNear (const Color& color, double percent) const
{
	double	hue1	= 0., sat1	= 0., value1	= 0.;
	double	hue2	= 0., sat2	= 0., value2	= 0.;
	getHSV (hue1, sat1, value1);
	color.getHSV (hue2, sat2, value2);
	if ((relativeDistance (hue1, hue2, 360.) > percent) ||
	    (relativeDistance (sat1, sat2, 1.) > percent) ||
	    (relativeDistance (value1, value2, 1.) > percent))
		return false;

	return true;
}	// Color::hsvNear


void Color::getDRGB (double& red, double& green, double& blue) const
{
	red		= getDRed ( );
	green	= getDGreen ( );
	blue	= getDBlue ( );
}	// Color::getDRGB


void Color::setDRGB (double red, double green, double blue)
{
	setDRed (red);
	setDGreen (green);
	setDBlue (blue);
}	// Color::setDRGB


inline double min3 (double a, double b, double c)
{
	const double	m	= std::min (a, b);

	return std::min (m, c);
}	// min3


inline double max3 (double a, double b, double c)
{
	const double	m	= std::max (a, b);

	return std::max (m, c);
}	// max3


void Color::getHSV (double& hue, double& saturation, double& value) const
{
	const double	epsilon	= 1E-10;
	const double	r		= _red   / 255.;
	const double	g		= _green / 255.;
	const double	b		= _blue  / 255.;
	const double	rgbmin	= min3 (r, g, b);
	const double	rgbmax	= max3 (r, g, b);
	const double	delta	= rgbmax - rgbmin;

	value	= rgbmax;

	if (std::fabs (delta) < epsilon)
	{	// achromatic case => h undefined
		hue			= 0.;	// dixit Qt
		saturation	= 0.;	// dixit Qt
	}	// if (std::fabs (delta) < epsilon)
	else
	{
		// Rem : max ne peut être nul car si c'était le cas on aurait delta nul.
		saturation	= delta / rgbmax;

		if (std::fabs (rgbmax - r) < epsilon)
			hue	= (g - b) / delta;		// Between yellow & magenta
		else if (std::fabs (rgbmax - g) < epsilon)
			hue	= 2 + (b - r) / delta;	// Between cyan & yellow
		else
			hue	= 4 + (r - g) / delta;	// Between magenta & cyan
	
		hue	*= 60;	// => degrees
		if (hue < 0)
			hue	+= 360;
	}	// else if (std::fabs (delta) < epsilon)

	// Post-conditions : 0 <= h <= 360, 0 <= s & v <= 1
	if ((hue < 0) || (hue > 360) || (saturation < 0) || (saturation > 1) ||
	    (value < 0) || (value > 1))
	{
		TkUtil::UTF8String	message (charset);
		message << "Erreur transformation RGB vers HSV de ("
		        << (unsigned long)_red << ", " << (unsigned long)_green << ", "
		        << (unsigned long)_blue << "). H = " << hue
		        << " S = " << saturation << " V = " << value << ".";
		INTERNAL_ERROR (exc, message, "Color::getHSV")
		throw exc;
	}	// if ((hue < 0) || (hue > 360) || ...
}	// Color::getHSV


void Color::invert ( )
{
	const double	red		= normalize (1. - getDRed ( ));
	const double	green	= normalize (1. - getDGreen ( ));
	const double	blue	= normalize (1. - getDBlue ( ));

	setDRGB (red, green, blue);
}	// Color::invert


Color Color::getNextColor (const Color& color, double redInc, double greenInc, double blueInc)
{
	const double	red		= normalize (color.getDRed ( ) + redInc);
	const double	green	= normalize (color.getDGreen ( ) + greenInc);
	const double	blue	= normalize (color.getDBlue ( ) + blueInc);
	return Color::create (red, green, blue);
}	// Color::getNextColor


Color Color::getNextColor (const Color& initialColor, unsigned char redInc, unsigned char greenInc, unsigned char blueInc)
{
	unsigned char		r = 0, g = 0, b = 0;
	initialColor.getRGB (r, g, b);
	r	+= redInc;
	g	+= greenInc;
	b	+= blueInc;

	return Color (r, g, b);
}	// Color::getNextColor


END_NAMESPACE_UTIL

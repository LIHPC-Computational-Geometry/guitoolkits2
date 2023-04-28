//
// Un test des caractères grecs est disponible dans les sources du fichier test
// QtCurvesMainWindow du module Qwt de Toolkits.
//

#include "QtUtil/QtStringHelper.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/UTF8String.h>

#include <QFontDatabase>
#include <QStringList>
#include <QTextDocument>

#include <assert.h>


// Implémentation personnelle de la fonction strcasestr qui n'est pas standard :
static char* tk_strcasestr (const char* str1, const char* str2)
{	// L'usage est ici limité à de petites chaines de caractères (nom des fontes
	// système).
	char	buffer1 [2048]	= {'\0'};
	char	buffer2 [2048]	= {'\0'};
	size_t	length	= strlen (str1), i = 0;
	for (i = 0; i < length; i++)
		buffer1 [i]	= tolower (str1 [i]);
	length	= strlen (str2);	
	for (i = 0; i < length; i++)
		buffer2 [i]	= tolower (str2 [i]);

	char*	pos	= strstr (buffer1, buffer2);
	if (NULL != pos)
		return (char*)str1 + (pos - buffer1);

	return NULL;
}	// tk_strcasestr

//#endif	// __USE_GNU


USING_STD
USING_UTIL

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


// Les positions des caractères grecs dans la table de caractères ISO 8859
// (partie 7). Les majuscules et minuscules sont contigues.
static	const	unsigned int	alphaMajPos	= 913;
static	const	unsigned int	alphaMinPos	= alphaMajPos + 32;

static const QChar 	alphaMinArray [1]		= {alphaMinPos};
static const QChar 	alphaMajArray [1]		= {alphaMajPos};
static const QChar 	betaMinArray [1]		= {alphaMinPos + 1};
static const QChar 	betaMajArray [1]		= {alphaMajPos + 1};
static const QChar 	gammaMinArray [1]		= {alphaMinPos + 2};
static const QChar 	gammaMajArray [1]		= {alphaMajPos + 2};
static const QChar 	deltaMinArray [1]		= {alphaMinPos + 3};
static const QChar 	deltaMajArray [1]		= {alphaMajPos + 3};
static const QChar 	epsilonMinArray [1]		= {alphaMinPos + 4};
static const QChar 	epsilonMajArray [1]		= {alphaMajPos + 4};
static const QChar 	dzetaMinArray [1]		= {alphaMinPos + 5};
static const QChar 	dzetaMajArray [1]		= {alphaMajPos + 5};
static const QChar 	etaMinArray [1]			= {alphaMinPos + 6};
static const QChar 	etaMajArray [1]			= {alphaMajPos + 6};
static const QChar 	thetaMinArray [1]		= {alphaMinPos + 7};
static const QChar 	thetaMajArray [1]		= {alphaMajPos + 7};
static const QChar 	iotaMinArray [1]		= {alphaMinPos + 8};
static const QChar 	iotaMajArray [1]		= {alphaMajPos + 8};
static const QChar 	kappaMinArray [1]		= {alphaMinPos + 9};
static const QChar 	kappaMajArray [1]		= {alphaMajPos + 9};
static const QChar 	lambdaMinArray [1]		= {alphaMinPos + 10};
static const QChar 	lambdaMajArray [1]		= {alphaMajPos + 10};
static const QChar 	muMinArray [1]			= {alphaMinPos + 11};
static const QChar 	muMajArray [1]			= {alphaMajPos + 11};
static const QChar 	nuMinArray [1]			= {alphaMinPos + 12};
static const QChar 	nuMajArray [1]			= {alphaMajPos + 12};
static const QChar 	xsiMinArray [1]			= {alphaMinPos + 13};
static const QChar 	xsiMajArray [1]			= {alphaMajPos + 13};
static const QChar 	omicronnMinArray [1]	= {alphaMinPos + 14};
static const QChar 	omicronnMajArray [1]	= {alphaMajPos + 14};
static const QChar 	piMinArray [1]			= {alphaMinPos + 15};
static const QChar 	piMajArray [1]			= {alphaMajPos + 15};
static const QChar 	roMinArray [1]			= {alphaMinPos + 16};
static const QChar 	roMajArray [1]			= {alphaMajPos + 16};
static const QChar 	sigmaMinArray [1]		= {alphaMinPos + 18};
static const QChar 	sigmaMajArray [1]		= {alphaMajPos + 18};
static const QChar 	tauMinArray [1]			= {alphaMinPos + 19};
static const QChar 	tauMajArray [1]			= {alphaMajPos + 19};
static const QChar 	upsilonMinArray [1]		= {alphaMinPos + 20};
static const QChar 	upsilonMajArray [1]		= {alphaMajPos + 20};
static const QChar 	phiMinArray [1]			= {alphaMinPos + 21};
static const QChar 	phiMajArray [1]			= {alphaMajPos + 21};
static const QChar 	khiMinArray [1]			= {alphaMinPos + 22};
static const QChar 	khiMajArray [1]			= {alphaMajPos + 22};
static const QChar 	psiMinArray [1]			= {alphaMinPos + 23};
static const QChar 	psiMajArray [1]			= {alphaMajPos + 23};
static const QChar 	omegaMinArray [1]		= {alphaMinPos + 24};
static const QChar 	omegaMajArray [1]		= {alphaMajPos + 24};

QString	QtStringHelper::_alphaMin (alphaMinArray, 1);
QString	QtStringHelper::_alphaMaj (alphaMajArray, 1);
QString	QtStringHelper::_betaMin (betaMinArray, 1);
QString	QtStringHelper::_betaMaj (betaMajArray, 1);
QString	QtStringHelper::_khiMin (khiMinArray, 1);
QString	QtStringHelper::_khiMaj (khiMajArray, 1);
QString	QtStringHelper::_deltaMin (deltaMinArray, 1);
QString	QtStringHelper::_deltaMaj (deltaMajArray, 1);
QString	QtStringHelper::_epsilonMin (epsilonMinArray, 1);
QString	QtStringHelper::_epsilonMaj (epsilonMajArray, 1);
QString	QtStringHelper::_gammaMin (gammaMinArray, 1);
QString	QtStringHelper::_gammaMaj (gammaMajArray, 1);
QString	QtStringHelper::_etaMin (etaMinArray, 1);
QString	QtStringHelper::_etaMaj (etaMajArray, 1);
QString	QtStringHelper::_iotaMin (iotaMinArray, 1);
QString	QtStringHelper::_iotaMaj (iotaMajArray, 1);
QString	QtStringHelper::_phiMin (phiMinArray, 1);
QString	QtStringHelper::_phiMaj (phiMajArray, 1);
QString	QtStringHelper::_kappaMin (kappaMinArray, 1);
QString	QtStringHelper::_kappaMaj (kappaMajArray, 1);
QString	QtStringHelper::_lambdaMin (lambdaMinArray, 1);
QString	QtStringHelper::_lambdaMaj (lambdaMajArray, 1);
QString	QtStringHelper::_muMin (muMinArray, 1);
QString	QtStringHelper::_muMaj (muMajArray, 1);
QString	QtStringHelper::_nuMin (nuMinArray, 1);
QString	QtStringHelper::_nuMaj (nuMajArray, 1);
QString	QtStringHelper::_omicronnMin (omicronnMinArray, 1);
QString	QtStringHelper::_omicronnMaj (omicronnMajArray, 1);
QString	QtStringHelper::_piMin (piMinArray, 1);
QString	QtStringHelper::_piMaj (piMajArray, 1);
QString	QtStringHelper::_thetaMin (thetaMinArray, 1);
QString	QtStringHelper::_thetaMaj (thetaMajArray, 1);
QString	QtStringHelper::_roMin (roMinArray, 1);
QString	QtStringHelper::_roMaj (roMajArray, 1);
QString	QtStringHelper::_sigmaMin (sigmaMinArray, 1);
QString	QtStringHelper::_sigmaMaj (sigmaMajArray, 1);
QString	QtStringHelper::_tauMin (tauMinArray, 1);
QString	QtStringHelper::_tauMaj (tauMajArray, 1);
QString	QtStringHelper::_upsilonMin (upsilonMinArray, 1);
QString	QtStringHelper::_upsilonMaj (upsilonMajArray, 1);
QString	QtStringHelper::_omegaMin (omegaMinArray, 1);
QString	QtStringHelper::_omegaMaj (omegaMajArray, 1);
QString	QtStringHelper::_xsiMin (xsiMinArray, 1);
QString	QtStringHelper::_xsiMaj (xsiMajArray, 1);
QString	QtStringHelper::_psiMin (psiMinArray, 1);
QString	QtStringHelper::_psiMaj (psiMajArray, 1);
QString	QtStringHelper::_dzetaMin (dzetaMinArray, 1);
QString	QtStringHelper::_dzetaMaj (dzetaMajArray, 1);

// Les caractères grecs au format "rich text" :
QString	QtStringHelper::_alphaMinRichText;
QString	QtStringHelper::_alphaMajRichText;
QString	QtStringHelper::_betaMinRichText;
QString	QtStringHelper::_betaMajRichText;
QString	QtStringHelper::_khiMinRichText;
QString	QtStringHelper::_khiMajRichText;
QString	QtStringHelper::_deltaMinRichText;
QString	QtStringHelper::_deltaMajRichText;
QString	QtStringHelper::_epsilonMinRichText;
QString	QtStringHelper::_epsilonMajRichText;
QString	QtStringHelper::_gammaMinRichText;
QString	QtStringHelper::_gammaMajRichText;
QString	QtStringHelper::_etaMinRichText;
QString	QtStringHelper::_etaMajRichText;
QString	QtStringHelper::_iotaMinRichText;
QString	QtStringHelper::_iotaMajRichText;
QString	QtStringHelper::_phiMinRichText;
QString	QtStringHelper::_phiMajRichText;
QString	QtStringHelper::_kappaMinRichText;
QString	QtStringHelper::_kappaMajRichText;
QString	QtStringHelper::_lambdaMinRichText;
QString	QtStringHelper::_lambdaMajRichText;
QString	QtStringHelper::_muMinRichText;
QString	QtStringHelper::_muMajRichText;
QString	QtStringHelper::_nuMinRichText;
QString	QtStringHelper::_nuMajRichText;
QString	QtStringHelper::_omicronnMinRichText;
QString	QtStringHelper::_omicronnMajRichText;
QString	QtStringHelper::_piMinRichText;
QString	QtStringHelper::_piMajRichText;
QString	QtStringHelper::_thetaMinRichText;
QString	QtStringHelper::_thetaMajRichText;
QString	QtStringHelper::_roMinRichText;
QString	QtStringHelper::_roMajRichText;
QString	QtStringHelper::_sigmaMinRichText;
QString	QtStringHelper::_sigmaMajRichText;
QString	QtStringHelper::_tauMinRichText;
QString	QtStringHelper::_tauMajRichText;
QString	QtStringHelper::_upsilonMinRichText;
QString	QtStringHelper::_upsilonMajRichText;
QString	QtStringHelper::_omegaMinRichText;
QString	QtStringHelper::_omegaMajRichText;
QString	QtStringHelper::_xsiMinRichText;
QString	QtStringHelper::_xsiMajRichText;
QString	QtStringHelper::_psiMinRichText;
QString	QtStringHelper::_psiMajRichText;
QString	QtStringHelper::_dzetaMinRichText;
QString	QtStringHelper::_dzetaMajRichText;

QFont			QtStringHelper::_greekFont;


QtStringHelper::QtStringHelper ( )
{
	assert (0 && "QtStringHelper::QtStringHelper is not allowed.");
}	// QtStringHelper::QtStringHelper


QtStringHelper::QtStringHelper (const QtStringHelper&)
{
	assert (0 && "QtStringHelper copy constructor is not allowed.");
}	// QtStringHelper::QtStringHelper


QtStringHelper& QtStringHelper::operator = (const QtStringHelper&)
{
	assert (0 && "QtStringHelper::operator = is not allowed.");
}	// QtStringHelper::QtStringHelper


void QtStringHelper::initialize ( )
{
	// Recherche d'une police de caractères comportant les caractères grecs :
	QFontDatabase	database;
	QStringList		families	= database.families (QFontDatabase::Greek);
	QStringList::iterator	it;
	bool					greekFont	= false;
	for (it = families.begin ( ); families.end ( ) != it; it++)
	{
		if ((0 != tk_strcasestr ((*it).toStdString ( ).c_str ( ), "greek")) ||
		    (0 != tk_strcasestr ((*it).toStdString ( ).c_str ( ), "symbol")))
		{
			_greekFont	= QFont (*it);
			greekFont	= true;
			break;
		}	// if ((0 != tk_strcasestr ((*it).toStdString ( ).c_str ( ) ...
	}	// for (it = families.begin ( ); families.end ( ) != it; it++)

#ifdef QT_4
	if (false == greekFont)
	{
		families	= database.families (QFontDatabase::Symbol);
		for (it = families.begin ( ); families.end ( ) != it; it++)
		{
			if ((0 != tk_strcasestr ((*it).toStdString( ).c_str( ), "greek")) ||
			    (0 != tk_strcasestr ((*it).toStdString( ).c_str( ), "symbol")))
			{
				greekFont	= true;
				_greekFont	= QFont (*it);
				break;
			}	// if ((0 != tk_strcasestr ((*it).toStdString ( ).c_str ( ) ...
		}	// for (it = families.begin ( ); families.end ( ) != it; it++)
	}	// if (false == greekFont)
#endif	// QT_4

	if (false == greekFont)
	{
		// #@&*)$@ !
		UTF8String	mess (charset);
		mess << "Police de caractères grecque non trouvée dans la base de "
		     << "données des polices.";
		ConsoleOutput::cerr ( ) << mess << "\a" << co_endl;
	}	// if (false == greekFont)

	const string	greekFamily (_greekFont.family ( ).toStdString ( ));
	_alphaMinRichText		= textToGreekText ("a");
	_alphaMajRichText		= textToGreekText ("A");
	_betaMinRichText		= textToGreekText ("b");
	_betaMajRichText		= textToGreekText ("B");
	_khiMinRichText			= textToGreekText ("c");
	_khiMajRichText			= textToGreekText ("C");
	_deltaMinRichText		= textToGreekText ("d");
	_deltaMajRichText		= textToGreekText ("D");
	_epsilonMinRichText		= textToGreekText ("e");
	_epsilonMajRichText		= textToGreekText ("E");
	_gammaMinRichText		= textToGreekText ("g");
	_gammaMajRichText		= textToGreekText ("G");
	_etaMinRichText			= textToGreekText ("h");
	_etaMajRichText			= textToGreekText ("H");
	_iotaMinRichText		= textToGreekText ("i");
	_iotaMajRichText		= textToGreekText ("I");
	_phiMinRichText			= textToGreekText ("j");
	_phiMajRichText			= textToGreekText ("J");
	_kappaMinRichText		= textToGreekText ("k");
	_kappaMajRichText		= textToGreekText ("K");
	_lambdaMinRichText		= textToGreekText ("l");
	_lambdaMajRichText		= textToGreekText ("L");
	_muMinRichText			= textToGreekText ("m");
	_muMajRichText			= textToGreekText ("M");
	_nuMinRichText			= textToGreekText ("n");
	_nuMajRichText			= textToGreekText ("N");
	_omicronnMinRichText	= textToGreekText ("o");
	_omicronnMajRichText	= textToGreekText ("O");
	_piMinRichText			= textToGreekText ("p");
	_piMajRichText			= textToGreekText ("P");
	_thetaMinRichText		= textToGreekText ("q");
	_thetaMajRichText		= textToGreekText ("Q");
	_roMinRichText			= textToGreekText ("r");
	_roMajRichText			= textToGreekText ("R");
	_sigmaMinRichText		= textToGreekText ("s");
	_sigmaMajRichText		= textToGreekText ("S");
	_tauMinRichText			= textToGreekText ("t");
	_tauMajRichText			= textToGreekText ("T");
	_upsilonMinRichText		= textToGreekText ("u");
	_upsilonMajRichText		= textToGreekText ("U");
	_omegaMinRichText		= textToGreekText ("w");
	_omegaMajRichText		= textToGreekText ("W");
	_xsiMinRichText			= textToGreekText ("x");
	_xsiMajRichText			= textToGreekText ("X");
	_psiMinRichText			= textToGreekText ("y");
	_psiMajRichText			= textToGreekText ("Y");
	_dzetaMinRichText		= textToGreekText ("z");
	_dzetaMajRichText		= textToGreekText ("Z");
}	// QtStringHelper::initialize


void QtStringHelper::finalize ( )
{
	// En prévision de futurs développements.
}	// QtStringHelper::finalize


QString QtStringHelper::textToUnicodeGreekText (const QString& text)
{
	QString	greek (text);
	for (int i = 0; i < text.length ( ); i++)
	{
		const QChar	c	= text [i];
		if (true == c.isLetter ( ))
		{
#	if QT_VERSION >= 0x05000
			switch (c.toLatin1 ( ))
#	else
			switch (c.toAscii ( ))
#endif
			{
				case 'a' : greek [i] = QtStringHelper::alphaMin ( )[0]; break;
				case 'A' : greek [i] = QtStringHelper::alphaMaj ( )[0]; break;
				case 'b' : greek [i] = QtStringHelper::betaMin ( )[0]; break;
				case 'B' : greek [i] = QtStringHelper::betaMaj ( )[0]; break;
				case 'c' : greek [i] = QtStringHelper::khiMin ( )[0]; break;
				case 'C' : greek [i] = QtStringHelper::khiMaj ( )[0]; break;
				case 'd' : greek [i] = QtStringHelper::deltaMin ( )[0]; break;
				case 'D' : greek [i] = QtStringHelper::deltaMaj ( )[0]; break;
				case 'e' : greek [i] = QtStringHelper::epsilonMin ( )[0]; break;
				case 'E' : greek [i] = QtStringHelper::epsilonMaj ( )[0]; break;
				// f -> phi : non officiel (j, J)
				case 'f' : greek [i] = QtStringHelper::phiMin ( )[0]; break;
				case 'F' : greek [i] = QtStringHelper::phiMaj ( )[0]; break;
				case 'g' : greek [i] = QtStringHelper::gammaMin ( )[0]; break;
				case 'G' : greek [i] = QtStringHelper::gammaMaj ( )[0]; break;
				case 'h' : greek [i] = QtStringHelper::etaMin ( )[0]; break;
				case 'H' : greek [i] = QtStringHelper::etaMaj ( )[0]; break;
				case 'i' : greek [i] = QtStringHelper::iotaMin ( )[0]; break;
				case 'I' : greek [i] = QtStringHelper::iotaMaj ( )[0]; break;
				case 'j' : greek [i] = QtStringHelper::phiMin ( )[0]; break;
				case 'J' : greek [i] = QtStringHelper::phiMaj ( )[0]; break;
				case 'k' : greek [i] = QtStringHelper::kappaMin ( )[0]; break;
				case 'K' : greek [i] = QtStringHelper::kappaMaj ( )[0]; break;
				case 'l' : greek [i] = QtStringHelper::lambdaMin ( )[0]; break;
				case 'L' : greek [i] = QtStringHelper::lambdaMaj ( )[0]; break;
				case 'm' : greek [i] = QtStringHelper::muMin ( )[0]; break;
				case 'M' : greek [i] = QtStringHelper::muMaj ( )[0]; break;
				case 'n' : greek [i] = QtStringHelper::nuMin ( )[0]; break;
				case 'N' : greek [i] = QtStringHelper::nuMaj ( )[0]; break;
				case 'o' : greek [i] = QtStringHelper::omicronnMin ( )[0];break;
				case 'O' : greek [i] = QtStringHelper::omicronnMaj ( )[0];break;
				case 'p' : greek [i] = QtStringHelper::piMin ( )[0]; break;
				case 'P' : greek [i] = QtStringHelper::piMaj ( )[0]; break;
				case 'q' : greek [i] = QtStringHelper::thetaMin ( )[0]; break;
				case 'Q' : greek [i] = QtStringHelper::thetaMaj ( )[0]; break;
				case 'r' : greek [i] = QtStringHelper::roMin ( )[0]; break;
				case 'R' : greek [i] = QtStringHelper::roMaj ( )[0]; break;
				case 's' : greek [i] = QtStringHelper::sigmaMin ( )[0]; break;
				case 'S' : greek [i] = QtStringHelper::sigmaMaj ( )[0]; break;
				case 't' : greek [i] = QtStringHelper::tauMin ( )[0]; break;
				case 'T' : greek [i] = QtStringHelper::tauMaj ( )[0]; break;
				case 'u' : greek [i] = QtStringHelper::upsilonMin ( )[0]; break;
				case 'U' : greek [i] = QtStringHelper::upsilonMaj ( )[0]; break;
				// v -> nu : non officiel (n, N)
				case 'v' : greek [i] = QtStringHelper::nuMin ( )[0]; break;
				case 'V' : greek [i] = QtStringHelper::nuMaj ( )[0]; break;
				case 'w' : greek [i] = QtStringHelper::omegaMin ( )[0]; break;
				case 'W' : greek [i] = QtStringHelper::omegaMaj ( )[0]; break;
				case 'x' : greek [i] = QtStringHelper::xsiMin ( )[0]; break;
				case 'X' : greek [i] = QtStringHelper::xsiMaj ( )[0]; break;
				case 'y' : greek [i] = QtStringHelper::psiMin ( )[0]; break;
				case 'Y' : greek [i] = QtStringHelper::psiMaj ( )[0]; break;
				case 'z' : greek [i] = QtStringHelper::dzetaMin ( )[0]; break;
				case 'Z' : greek [i] = QtStringHelper::dzetaMaj ( )[0]; break;
			}	// switch (c.toAscii ( ))
		}	// if (true == c.isLetter ( ))
	}	// for (int i = 0; i < text.length ( ); i++)

	return greek;
}	// QtStringHelper::textToUnicodeGreekText


QString QtStringHelper::textToRichText (const string& text, const string& font)
{
	UTF8String	str (charset);
	str << "<FONT face=\"" << font << "\">" << text << "</FONT>";

	return UTF8TOQSTRING (str);
}	// QtStringHelper::textToRichText


QString QtStringHelper::textToGreekText (const string& text)
{
	const string	greekFamily (_greekFont.family ( ).toStdString ( ));

	return textToRichText (text, greekFamily);
}	// QtStringHelper::textToGreekText


const QFont& QtStringHelper::greekFont ( )
{
	return _greekFont;
}	// QtStringHelper::greekFont


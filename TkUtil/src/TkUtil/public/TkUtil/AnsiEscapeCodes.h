/**
 * @author	Charles PIGNEROL, sur une idée de Guénolé HAREL
 *
 * Classe contenant les séquences ESC ANSI pour interagir avec le terminal
 * (colorisation, déplacement, effacement, ...)
 *
 * @see	https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
 *
 * @date	29/10/2018
 *
 */
#ifndef ANSI_ESCAPE_CODES_H
#define ANSI_ESCAPE_CODES_H

#include "TkUtil/util_config.h"
#include <iostream>

BEGIN_NAMESPACE_UTIL

/**
 * <P>
 * Utilitaires de mise en évidence et suppression de texte dans un terminal.
 * L'encodage repose sur les séquences ESC 3/4 bits et CSI.
 * </P>
 * <P>Exemple d'utilisation :</P>
 * <CODE>
 * static const char*  asciiLine   = "La ligne test avec encodage  ASCII.";
 * cout << AnsiEscapeCodes::reset;
 * cout << "On efface complement l'ecran => cette ligne doit disparaitre."
 *      << endl;
 * cout << AnsiEscapeCodes::clearScreen;
 * cout << "Ligne avec jeu de caractères par défaut." << endl;
 * cout << asciiLine << endl;
 * cout << AnsiEscapeCodes::bold << asciiLine << AnsiEscapeCodes::boldOff
 *      << endl;
 * </CODE>
 */
class AnsiEscapeCodes
{
	public :

	static const char*	reset;		// All attributes off
	static const char*	bold;		// Bold or increased intensity
	static const char*	faint;		// Faint (decreased intensity)
	static const char*	italic;		// Not widely supported. Sometimes treated as inverse.
	static const char*	underline;	// Underline
	static const char*	slowBlink;	// Less than 150 per minute
	static const char*	rapidBlink;	// 150+ per minute; not widely supported
	static const char*	reverseVideo;// Swap foreground and background colors
	static const char*	conceal;	// Not widely supported.
	static const char*	crossedOut;	// Characters legible, but marked for deletion.
	static const char*	defaultFont;
	static const char*	font1;		// Alternative font (9 possibilities)
	static const char*	font2;		// Alternative font (9 possibilities)
	static const char*	font3;		// Alternative font (9 possibilities)
	static const char*	font4;		// Alternative font (9 possibilities)
	static const char*	font5;		// Alternative font (9 possibilities)
	static const char*	font6;		// Alternative font (9 possibilities)
	static const char*	font7;		// Alternative font (9 possibilities)
	static const char*	font8;		// Alternative font (9 possibilities)
	static const char*	font9;		// Alternative font (9 possibilities)
	static const char*	boldOff;	// bold and faint off
	static const char*	normalColor;// Normal color or intensity, neither bold nor faint
	static const char*	italicOff;
	static const char*	underlineOff;
	static const char*	blinkOff;
	static const char*	inverseOff;
	static const char*	reveal;		// Conceal off
	static const char*	crossedOutOff;
	static const char*	blackFg;	// Set foreground color
	static const char*	redFg;		// Set foreground color
	static const char*	greenFg;	// Set foreground color
	static const char*	yellowFg;	// Set foreground color
	static const char*	blueFg;		// Set foreground color
	static const char*	magentaFg;	// Set foreground color
	static const char*	cyanFg;		// Set foreground color
	static const char*	whiteFg;	// Set foreground color
	static const char*	brownFg;	// Set foreground color
	static const char*	pinkFg;		// Set foreground color
	static const char*	orangeFg;	// Set foreground color
	static const char*	grayFg;		// Set foreground color
	static const char*	darkGrayFg;	// Set foreground color
	static const char*	lightGrayFg;// Set foreground color
	static const char*	brightBlackFg;// Set foreground color
	static const char*	brightRedFg;// Set foreground color
	static const char*	brightGreenFg;// Set foreground color
	static const char*	brightYellowFg;// Set foreground color
	static const char*	brightBlueFg;// Set foreground color
	static const char*	brightMagentaFg;// Set foreground color
	static const char*	brightCyanFg;// Set foreground color
	static const char*	brightWhiteFg;// Set foreground color
	static const char*	defaultFg;	// Set default foreground color
	static const char*	blackBg;	// Set background color
	static const char*	redBg;		// Set background color
	static const char*	greenBg;	// Set background color
	static const char*	yellowBg;	// Set background color
	static const char*	blueBg;		// Set background color
	static const char*	magentaBg;	// Set background color
	static const char*	cyanBg;		// Set background color
	static const char*	whiteBg;	// Set background color
	static const char*	brownBg;	// Set background color
	static const char*	pinkBg;		// Set background color
	static const char*	orangeBg;	// Set background color
	static const char*	grayBg;		// Set background color
	static const char*	darkGrayBg;	// Set background color
	static const char*	lightGrayBg;// Set background color
	static const char*	brightBlackBg;// Set background color
	static const char*	brightRedBg;// Set background color
	static const char*	brightGreenBg;// Set background color
	static const char*	brightYellowBg;// Set background color
	static const char*	brightBlueBg;// Set background color
	static const char*	brightMagentaBg;// Set background color
	static const char*	brightCyanBg;// Set background color
	static const char*	brightWhiteBg;// Set background color
	static const char*	defaultBg;	// Set default background color
	static const char*	framed;
	static const char*	encircled;
	static const char*	overlined;
	static const char*	framedOff;
	static const char*	encircledOff;
	static const char*	overlinedOff;
	static const char*	clearScreen;	// Clears entire screen
	static const char*	clearScreenBeg;	// Clears screen from beginning to cursor
	static const char*	clearScreenEnd;	// Clears screen from cursor to end


	private :

	AnsiEscapeCodes ( )
	{ }
	AnsiEscapeCodes (const AnsiEscapeCodes&)
	{ }
	AnsiEscapeCodes& operator = (const AnsiEscapeCodes&)
	{ return *this; }
	~AnsiEscapeCodes ( )
	{ }
};	// AnsiEscapeCodes


/**
 * <P>
 * Formatage du texte dans un terminal avec suspension automatique du
 * formatage à la destruction.
 * </P>
 * <P>Exemple :</P>
 * <CODE>
 * static const char*  asciiLine   = "La ligne test avec encodage  ASCII.";
 * cout << "Encodage italique (pas toujours supporte) : ";
 * { TermAutoStyle (cout, AnsiEscapeCodes::italic); cout << asciiLine << endl; }
 * cout << "Encodage bleu sur fond jaune souligne : ";
 * { TermAutoStyle as1 (cout, AnsiEscapeCodes::underline);
 *   TermAutoStyle as2 (cout, AnsiEscapeCodes::blueFg);
 *   TermAutoStyle as3 (cout, AnsiEscapeCodes::yellowBg);
 *   cout << asciiLine; }
 * cout << endl;   // maintenant car Bg.
 * cout << "Encodage video inverse clignotant fonte 5 :";
 * { TermAutoStyle as1 (cout, AnsiEscapeCodes::reverseVideo);
 *   TermAutoStyle as2 (cout, AnsiEscapeCodes::slowBlink);
 *   TermAutoStyle as3 (cout, AnsiEscapeCodes::font5);
 *   cout << asciiLine << endl; }
 * </CODE>
 */
class TermAutoStyle
{
	public :

	/**
	 * <P>Constructeurs et destructeurs.</P>
	 *
	 * <P>Dans le cas du constructeur par défaut il convient d'appeler la
	 * méthode <I>setAttributes</I> pour appliquer un style à un flux.</P>
	 *
	 * <P>Le flux (ex : cout, cerr) transmis en argument doit être fonctionnel 
	 * lors de la destruction de l'instance.</P>
	 *
	 * <P>Le caractère <I>on</I> (séquence appliquée à la construction) n'est 
	 * pas copié, seule l'est le pointeur, et il doit être disponible jusqu'à la
	 * destruction.</P>
	 * @warning	<B>ATTENTION :</B> pour le background il faut que la destruction
	 * 			ait lieu avant le passage à la ligne suivante.
	 */
	TermAutoStyle ( );
	TermAutoStyle (std::ostream&, const char* on);
	TermAutoStyle (const TermAutoStyle&);
	TermAutoStyle& operator = (const TermAutoStyle&);
	~TermAutoStyle ( );

	/**
	 * Affecte à l'instance le flux et le style à lui appliquer. Fonctionne avec
	 * le constructeur par défaut.
	 */
	void setAttributes (std::ostream&, const char* on);


	private :

	std::ostream	*_stream;
	const char		*_on;
};	// class TermAutoStyle


/**
 * Classe de services de positionnement du curseur (clavier) dans un terminal.
 */
class TermCursorPos
{
	public :

	enum DIR { LEFT, RIGHT, UP, DOWN };

	/**
	 * Déplace le curseur du flux transmis en argument de <I>num</I> caractères
	 * dans le sens <I>dir</I>.
	 */
	static void move (std::ostream& stream, DIR dir, size_t num);

	/**
	 * Déplace le curseur du flux transmis en argument à la position absolue
	 * transmise en argument.
	 */
	static void moveTo (std::ostream& stream, size_t row, size_t col);


	private :

	TermCursorPos ( )
	{ }
	TermCursorPos (const TermCursorPos&)
	{ }
	TermCursorPos& operator = (const TermCursorPos&)
	{ return *this; }
	~TermCursorPos ( )
	{ }
};	// class TermCursorPos

END_NAMESPACE_UTIL

#endif	// ANSI_ESCAPE_CODES_H

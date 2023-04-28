/**
 * @author	Charles PIGNEROL, sur une idée de Guénolé HAREL
 *
 * Classe contenant les séquences ESC ANSI pour interagir avec le terminal
 * (colorisation, déplacement, effacement, ...)
 *
 * @see	https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
 *
 * @date	29/10/2018
 */

#include "TkUtil/AnsiEscapeCodes.h"

#include <stdio.h>

USING_STD


BEGIN_NAMESPACE_UTIL



const char*	AnsiEscapeCodes::reset					= "\033[0m";
const char*	AnsiEscapeCodes::bold					= "\033[1m";
const char*	AnsiEscapeCodes::faint					= "\033[2m";
const char*	AnsiEscapeCodes::italic					= "\033[3m";
const char*	AnsiEscapeCodes::underline				= "\033[4m";
const char*	AnsiEscapeCodes::slowBlink				= "\033[5m";
const char*	AnsiEscapeCodes::rapidBlink				= "\033[6m";
const char*	AnsiEscapeCodes::reverseVideo			= "\033[7m";
const char*	AnsiEscapeCodes::conceal				= "\033[8m";
const char*	AnsiEscapeCodes::crossedOut				= "\033[9m";
const char*	AnsiEscapeCodes::defaultFont			= "\033[10m";
const char*	AnsiEscapeCodes::font1					= "\033[11m";
const char*	AnsiEscapeCodes::font2					= "\033[12m";
const char*	AnsiEscapeCodes::font3					= "\033[13m";
const char*	AnsiEscapeCodes::font4					= "\033[14m";
const char*	AnsiEscapeCodes::font5					= "\033[15m";
const char*	AnsiEscapeCodes::font6					= "\033[16m";
const char*	AnsiEscapeCodes::font7					= "\033[17m";
const char*	AnsiEscapeCodes::font8					= "\033[18m";
const char*	AnsiEscapeCodes::font9					= "\033[19m";
const char*	AnsiEscapeCodes::boldOff				= "\033[22m";
const char*	AnsiEscapeCodes::normalColor			= "\033[22m";
const char*	AnsiEscapeCodes::italicOff				= "\033[23m";
const char*	AnsiEscapeCodes::underlineOff			= "\033[24m";
const char*	AnsiEscapeCodes::blinkOff				= "\033[25m";
const char*	AnsiEscapeCodes::inverseOff				= "\033[27m";
const char*	AnsiEscapeCodes::reveal					= "\033[28m";
const char*	AnsiEscapeCodes::crossedOutOff			= "\033[29m";
const char*	AnsiEscapeCodes::blackFg				= "\033[30m";
const char*	AnsiEscapeCodes::redFg					= "\033[31m";
const char*	AnsiEscapeCodes::greenFg				= "\033[32m";
const char*	AnsiEscapeCodes::yellowFg				= "\033[33m";
const char*	AnsiEscapeCodes::blueFg					= "\033[34m";
const char*	AnsiEscapeCodes::magentaFg				= "\033[35m";
const char*	AnsiEscapeCodes::cyanFg					= "\033[36m";
const char*	AnsiEscapeCodes::whiteFg				= "\033[37m";
//
// Codage 8 bits de certaines couleurs :
// 16-231: 6 × 6 × 6 cube (216 colors):
// 16 + 36 × r + 6 × g + b (0 # r, g, b # 5)
// c.a.d. couleurs t.q. red multiple de 36, g multiple de 6, b < 5, et <= 231
// Ex :
// brown	= 16 + 36 * 3 + 6 * 0 + 0	= 124
// orange	= 16 + 36 * 5 + 6 * 3 + 0	= 214
// pink		= 16 + 36 * 5 + 6 * 0 + 5	= 201
const char*	AnsiEscapeCodes::brownFg				= "\033[38;5;124m";
const char*	AnsiEscapeCodes::pinkFg					= "\033[38;5;201m";
const char*	AnsiEscapeCodes::orangeFg				= "\033[38;5;214m";
const char*	AnsiEscapeCodes::grayFg					= "\033[38;5;243m";
const char*	AnsiEscapeCodes::darkGrayFg				= "\033[38;5;238m";
const char*	AnsiEscapeCodes::lightGrayFg			= "\033[38;5;248m";
const char*	AnsiEscapeCodes::brightBlackFg			= "\033[90m";
const char*	AnsiEscapeCodes::brightRedFg			= "\033[91m";
const char*	AnsiEscapeCodes::brightGreenFg			= "\033[92m";
const char*	AnsiEscapeCodes::brightYellowFg			= "\033[93m";
const char*	AnsiEscapeCodes::brightBlueFg			= "\033[94m";
const char*	AnsiEscapeCodes::brightMagentaFg		= "\033[95m";
const char*	AnsiEscapeCodes::brightCyanFg			= "\033[96m";
const char*	AnsiEscapeCodes::brightWhiteFg			= "\033[97m";
const char*	AnsiEscapeCodes::defaultFg				= "\033[39m";
const char*	AnsiEscapeCodes::blackBg				= "\033[40m";
const char*	AnsiEscapeCodes::redBg					= "\033[41m";
const char*	AnsiEscapeCodes::greenBg				= "\033[42m";
const char*	AnsiEscapeCodes::yellowBg				= "\033[43m";
const char*	AnsiEscapeCodes::blueBg					= "\033[44m";
const char*	AnsiEscapeCodes::magentaBg				= "\033[45m";
const char*	AnsiEscapeCodes::cyanBg					= "\033[46m";
const char*	AnsiEscapeCodes::whiteBg				= "\033[47m";
const char*	AnsiEscapeCodes::brownBg				= "\033[48;5;124m";
const char*	AnsiEscapeCodes::pinkBg					= "\033[48;5;201m";
const char*	AnsiEscapeCodes::orangeBg				= "\033[48;5;214m";
const char*	AnsiEscapeCodes::grayBg					= "\033[48;5;243m";
const char*	AnsiEscapeCodes::darkGrayBg				= "\033[48;5;238m";
const char*	AnsiEscapeCodes::lightGrayBg			= "\033[48;5;248m";
const char*	AnsiEscapeCodes::brightBlackBg			= "\033[100m";
const char*	AnsiEscapeCodes::brightRedBg			= "\033[101m";
const char*	AnsiEscapeCodes::brightGreenBg			= "\033[102m";
const char*	AnsiEscapeCodes::brightYellowBg			= "\033[103m";
const char*	AnsiEscapeCodes::brightBlueBg			= "\033[104m";
const char*	AnsiEscapeCodes::brightMagentaBg		= "\033[105m";
const char*	AnsiEscapeCodes::brightCyanBg			= "\033[106m";
const char*	AnsiEscapeCodes::brightWhiteBg			= "\033[107m";
const char*	AnsiEscapeCodes::defaultBg				= "\033[49m";
const char*	AnsiEscapeCodes::framed					= "\033[51m";
const char*	AnsiEscapeCodes::encircled				= "\033[52m";
const char*	AnsiEscapeCodes::overlined				= "\033[53m";
const char*	AnsiEscapeCodes::framedOff				= "\033[54m";
const char*	AnsiEscapeCodes::encircledOff			= "\033[54m";
const char*	AnsiEscapeCodes::overlinedOff			= "\033[55m";
const char* AnsiEscapeCodes::clearScreen			= "\033[2J";
const char* AnsiEscapeCodes::clearScreenBeg			= "\033[1J";
const char* AnsiEscapeCodes::clearScreenEnd			= "\033[0J";


// =============================================================================
//                           LA CLASSE TermAutoStyle
// =============================================================================

TermAutoStyle::TermAutoStyle ( )
	: _stream (0), _on (0)
{
}	// TermAutoStyle::TermAutoStyle

TermAutoStyle::TermAutoStyle (ostream& stream, const char* on)
	: _stream (0), _on (0)
{
	setAttributes (stream, on);
}	// TermAutoStyle::TermAutoStyle


TermAutoStyle::TermAutoStyle (const TermAutoStyle& aec)
	: _stream (aec._stream), _on (aec._on)
{
	setAttributes (*aec._stream, aec._on);
}	// TermAutoStyle::TermAutoStyle


TermAutoStyle& TermAutoStyle::operator = (const TermAutoStyle& aec)
{
	if (this != &aec)
	{
		_stream	= aec._stream;
		_on		= aec._on;
	}	// if (this != &aec)

	return *this;
}	// TermAutoStyle::operator =


TermAutoStyle::~TermAutoStyle ( )
{
	char*	off	= "";

	if (AnsiEscapeCodes::bold == _on)
		off	= (char*)AnsiEscapeCodes::boldOff;
	else if (AnsiEscapeCodes::faint == _on)
		off	= (char*)AnsiEscapeCodes::boldOff;
	else if (AnsiEscapeCodes::italic == _on)
		off	= (char*)AnsiEscapeCodes::italicOff;
	else if (AnsiEscapeCodes::underline	 == _on)
		off	= (char*)AnsiEscapeCodes::underlineOff;
	else if (AnsiEscapeCodes::slowBlink	 == _on)
		off	= (char*)AnsiEscapeCodes::blinkOff;
	else if (AnsiEscapeCodes::rapidBlink == _on)
		off	= (char*)AnsiEscapeCodes::blinkOff;
	else if (AnsiEscapeCodes::reverseVideo == _on)
		off	= (char*)AnsiEscapeCodes::inverseOff;
	else if (AnsiEscapeCodes::conceal	 == _on)
		off	= (char*)AnsiEscapeCodes::reveal;
	else if (AnsiEscapeCodes::crossedOut == _on)
		off	= (char*)AnsiEscapeCodes::crossedOutOff;
	else if ((AnsiEscapeCodes::font1 == _on) ||
	         (AnsiEscapeCodes::font2 == _on) ||
	         (AnsiEscapeCodes::font3 == _on) ||
	         (AnsiEscapeCodes::font4 == _on) ||
	         (AnsiEscapeCodes::font5 == _on) ||
	         (AnsiEscapeCodes::font6 == _on) ||
	         (AnsiEscapeCodes::font7 == _on) ||
	         (AnsiEscapeCodes::font8 == _on) ||
	         (AnsiEscapeCodes::font9 == _on))
		off	= (char*)AnsiEscapeCodes::defaultFont;
	else if ((AnsiEscapeCodes::blackFg == _on)   ||
	         (AnsiEscapeCodes::redFg == _on)     ||
	         (AnsiEscapeCodes::greenFg == _on)   ||
	         (AnsiEscapeCodes::yellowFg == _on)  ||
	         (AnsiEscapeCodes::blueFg == _on)    ||
	         (AnsiEscapeCodes::magentaFg == _on) ||
	         (AnsiEscapeCodes::cyanFg == _on)    ||
	         (AnsiEscapeCodes::whiteFg == _on)   ||
	         (AnsiEscapeCodes::brownFg == _on)   ||
	         (AnsiEscapeCodes::pinkFg == _on)    ||
	         (AnsiEscapeCodes::orangeFg == _on)  ||
	         (AnsiEscapeCodes::grayFg == _on)    ||
	         (AnsiEscapeCodes::darkGrayFg == _on)||
	         (AnsiEscapeCodes::lightGrayFg == _on))
		off	= (char*)AnsiEscapeCodes::defaultFg;
	else if ((AnsiEscapeCodes::blackBg == _on)   ||
	         (AnsiEscapeCodes::redBg == _on)     ||
	         (AnsiEscapeCodes::greenBg == _on)   ||
	         (AnsiEscapeCodes::yellowBg == _on)  ||
	         (AnsiEscapeCodes::blueBg == _on)    ||
	         (AnsiEscapeCodes::magentaBg == _on) ||
	         (AnsiEscapeCodes::cyanBg == _on)    ||
	         (AnsiEscapeCodes::whiteBg == _on)   ||
	         (AnsiEscapeCodes::brownBg == _on)   ||
	         (AnsiEscapeCodes::pinkBg == _on)    ||
	         (AnsiEscapeCodes::orangeBg == _on)  ||
	         (AnsiEscapeCodes::grayBg == _on)    ||
	         (AnsiEscapeCodes::darkGrayBg == _on)||
	         (AnsiEscapeCodes::lightGrayBg == _on))
		off	= (char*)AnsiEscapeCodes::defaultBg;
	else if (AnsiEscapeCodes::framed	 == _on)
		off	= (char*)AnsiEscapeCodes::framedOff;
	else if (AnsiEscapeCodes::encircled	 == _on)
		off	= (char*)AnsiEscapeCodes::encircledOff;
	else if (AnsiEscapeCodes::overlined	 == _on)
		off	= (char*)AnsiEscapeCodes::overlinedOff;
	else
		return;

	*_stream << off;
}	// TermAutoStyle::~TermAutoStyle


void TermAutoStyle::setAttributes (std::ostream& stream, const char* on)
{
	_stream	= &stream;
	_on		= on;

	if (0 != on)
		stream << on;
}	// TermAutoStyle::setAttributes


// =============================================================================
//                           LA CLASSE TermCursorPos
// =============================================================================

void TermCursorPos::move (ostream& stream, TermCursorPos::DIR dir, size_t num)
{
	static char	buffer [101] = { '\0' };
	switch (dir)
	{
		case TermCursorPos::LEFT	: sprintf (buffer, "\033[%luD", num);	break;
		case TermCursorPos::RIGHT	: sprintf (buffer, "\033[%luC", num);	break;
		case TermCursorPos::UP		: sprintf (buffer, "\033[%luA", num);	break;
		case TermCursorPos::DOWN	: sprintf (buffer, "\033[%luB", num);	break;
		default					: return;
	}	// switch (dir)

	stream << buffer;
}	// TermCursorPos::move


void TermCursorPos::moveTo (ostream& stream, size_t row, size_t col)
{
	static char	buffer [101] = { '\0' };
	sprintf (buffer, "\033[%lu;%luH", row, col);

	stream << buffer;
}	// TermCursorPos::moveTo


END_NAMESPACE_UTIL


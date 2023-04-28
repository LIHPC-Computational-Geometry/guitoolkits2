#include "UTF_8.h"

#include "TkUtil/UTF8String.h"
#include <iostream>

using namespace TkUtil;
using namespace std;

static const Charset	charset ("àéèùô");
static const char*	text	=
	"Chaîne de caractères issu d'un fichier encodé en UTF-8";


std::string utf8Str ( )
{
	cout << "Chaine \"" << text << "\" issue d'un fichier encode avec le jeu "
	     << "de caracteres " << charset.name ( ) << "." << endl;

	return text;
}	// utf8Str


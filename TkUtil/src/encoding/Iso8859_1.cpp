#include "Iso8859_1.h"

#include "TkUtil/UTF8String.h"
#include <iostream>

using namespace TkUtil;
using namespace std;


static const Charset	charset ("�����");
static const char*	text	=
			"Cha�ne de caract�res issue d'un fichier encod� en ISO8859-1";


std::string iso8859_1Str ( )
{
	cout << "Chaine \"" << text << "\" issue d'un fichier encode avec le jeu "
	     << "de caracteres " << charset.name ( ) << "." << endl;

	return text;
}	// iso8859_1Str


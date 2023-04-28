#include "QtUtil/QtFileDialogUtilities.h"
#include <assert.h>

using namespace std;


QtFileDialogUtilities::QtFileDialogUtilities ( )
{
	assert (0 && "QtFileDialogUtilities constructor is not allowed.");
}	// QtFileDialogUtilities::QtFileDialogUtilities


QtFileDialogUtilities::QtFileDialogUtilities (const QtFileDialogUtilities&)
{
	assert (0 && "QtFileDialogUtilities constructor is not allowed.");
}	// QtFileDialogUtilities::QtFileDialogUtilities


QtFileDialogUtilities& QtFileDialogUtilities::operator = (const QtFileDialogUtilities&)
{
	assert (0 && "QtFileDialogUtilities assignment operator is not allowed.");
	return *this;
}	// QtFileDialogUtilities::operator =


QtFileDialogUtilities::~QtFileDialogUtilities ( )
{
	assert (0 && "QtFileDialogUtilities destructor is not allowed.");
}	// QtFileDialogUtilities::~QtFileDialogUtilities


string QtFileDialogUtilities::extensionTail (const string& extension)
{
	QString	work	= QString (extension.c_str ( )).trimmed ( );
	if (0 == work.indexOf ("*."))
		return work.toStdString ( ).substr (2);
	if (0 == work.indexOf ("."))
		return work.toStdString ( ).substr (1);
		
	return extension;
}	// QtFileDialogUtilities::extensionTail


vector<string> QtFileDialogUtilities::nameFilterToExtensions (const QString& filters)
{
	vector<string>	extensions;
	
	int start	= filters.lastIndexOf ("(");
	start	= -1 == start ? 0 : start + 1;
	int end	= filters.indexOf (")", start);
	end		= -1 == end ? filters.length ( ) : end - 1;
	string	extensionsList	= QString (filters.toStdString ( ).substr (start, end - start + 1).c_str ( )).trimmed ( ).toStdString ( );
	
	size_t	pos	= 0;
	while (string::npos != (pos = extensionsList.find (' ')))
	{
		const string	extension	= extensionsList.substr (0, pos);
		extensions.push_back (extensionTail (extension));
		extensionsList.erase (0, pos + 1);
	}	// while (string::npos != (pos = extensionsList.find (' ')))
	if (0 != extensionsList.length ( ))
	{
		extensions.push_back (extensionTail (extensionsList));
	}
		
	return extensions;
}	// QtFileDialogUtilities::nameFilterToExtensions


bool QtFileDialogUtilities::extensionMatches (const string& extension, const vector<string>& extensions)
{
	const QString	ext (extension.c_str ( ));
	for (vector<string>::const_iterator it = extensions.begin ( ); extensions.end ( ) != it; it++)
		if (0 == QString::compare (ext, QString ((*it).c_str ( )), Qt::CaseInsensitive))
			return true;
			
	return false;
}	// QtFileDialogUtilities::extensionMatches

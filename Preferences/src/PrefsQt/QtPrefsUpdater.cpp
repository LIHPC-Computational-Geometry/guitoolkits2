#include "PrefsQt/QtPrefsUpdater.h"
#include "PrefsCore/BoolNamedValue.h"
#include "PrefsCore/ColorNamedValue.h"
#include "PrefsCore/LongNamedValue.h"
#include "PrefsCore/UnsignedLongNamedValue.h"
#include "PrefsCore/DoubleNamedValue.h"
#include "PrefsCore/DoubleTripletNamedValue.h"
#include "PrefsCore/StringNamedValue.h"
#include "PrefsXerces/XMLLoader.h"
#include <QtUtil/QtExtMessageBox.h>
#include <TkUtil/Date.h>
#include <TkUtil/File.h>
#include <TkUtil/Exception.h>
#include <TkUtil/InternalError.h>
#include <TkUtil/NumericConversions.h>
#include <TkUtil/Process.h>
#include <TkUtil/UTF8String.h>

#include <assert.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

USING_STD
USING_UTIL

BEGIN_NAMESPACE_PREFS

static const Charset	charset ("àéèùô");


static string getFullName (const Element& element)
{
	UTF8String	name (charset);

	if (true == element.hasParent ( ))
	{
		Section*	parent	= &element.getParent ( );
		while ((0 != parent) && (true == parent->hasParent ( )))
		{
			const string	old	= name.iso ( );
			name.clear ( );
			name << old << parent->getName ( ) << ".";
			parent	= &parent->getParent ( );
		}	// while ((0 != parent) && (true == parent->hasParent ( )))
	}	// if (true == element.hasParent ( ))
	name << element.getName ( );

	return name.iso ( );
}	// getFullName


static string createBackupFileName (const File& file)
{
	Date			date;
	UTF8String		name (charset);
	name << file.getFullFileName ( ) << "."
	     << NumericConversions::toStr ((unsigned short)date.getDay ( ), 2)
	     << NumericConversions::toStr ((unsigned short)date.getMonth ( ), 2)
	     << NumericConversions::toStr (date.getYear ( ), 4);

	File			backup (name);
	if (false == backup.exists ( ))
	{
		if (true == backup.isWritable ( ))
			return name.iso ( );

		UTF8String	error (charset);
		error << "Impossibilité de créer le fichier de sauvegarde "
		      << name << ".";
		throw Exception (error);
	}	// if (false == backup.exists ( ))

	bool	ok	= false;
	size_t	i	= 0;
	while (false == ok)
	{
		UTF8String	iname (charset);
		iname << name << "_" << i;
		backup.setFullFileName (iname);
		if (false == backup.exists ( ))
		{
			if (false == backup.isWritable ( ))
			{
				UTF8String	error (charset);
				error << "Impossibilité de créer le fichier de sauvegarde "
				      << iname << ".";
				throw Exception (error);
			}	// if (false == backup.isWritable ( ))

			name	= iname;
			break;
		}	// if (false == backup.exists ( ))

		i++;
	}	// while (false == ok)

	return name.iso ( );
}	// createBackupFileName



// ===========================================================================
//                            LA CLASSE QtPrefsUpdater
// ===========================================================================


QtPrefsUpdater::QtPrefsUpdater ( )
{
	assert (0 && "QtPrefsUpdater constructor is not allowed.");
}	// QtPrefsUpdater::QtPrefsUpdater


QtPrefsUpdater::QtPrefsUpdater (const QtPrefsUpdater&)
{
	assert (0 && "QtPrefsUpdater copy constructor is not allowed.");
}	// QtPrefsUpdater::QtPrefsUpdater


QtPrefsUpdater& QtPrefsUpdater::operator = (const QtPrefsUpdater&)
{
	assert (0 && "QtPrefsUpdater assignment operator is not allowed.");
	return *this;
}	// QtPrefsUpdater::::operator =


QtPrefsUpdater::~QtPrefsUpdater ( )
{
	assert (0 && "QtPrefsUpdater destructor is not allowed.");
}	// QtPrefsUpdater::~QtPrefsUpdater


size_t QtPrefsUpdater::update (const string& config, const string& patchs, const UTF8String& appTitle, const UTF8String& context,
			bool confirm, UTF8String& modifications)
{
	size_t			count	= 0;
	UTF8String		message (charset);

	modifications.clear ( );

	try
	{
		unique_ptr<Section>	modifiedCfg (XMLLoader::load (config.c_str ( )));
		unique_ptr<Section>	updates (XMLLoader::load (patchs.c_str ( )));
		if (0 == modifiedCfg.get ( ))
		{
			modifications << "Absence de préférences dans le fichier " << config
			        << " => absence d'application de correctif.";
			return count;
		}	// if (0 == modifiedCfg.get ( ))
		if (0 == updates.get ( ))
		{
			modifications << "Absence de préférences dans le fichier de "
					<< "correctifs " << patchs
			        << " => absence d'application de correctif.";
			return count;
		}	// if (0 == updates.get ( ))

		// On boucle sur les correctifs à appliquer :
		bool	operationStopped	= false;
		count	= update (*modifiedCfg.get ( ), *updates.get ( ), appTitle, context, modifications, confirm, operationStopped);

		XMLLoader::ENCODING	encoding	= XMLLoader::UTF_8;	// v 5.6.1
/*		XMLLoader::ENCODING	encoding	= XMLLoader::UTF_16;
		if (true == modifiedCfg->isIso ( ))
			encoding	= XMLLoader::ISO_8859;
		else if (true == modifiedCfg->isAscii ( ))
			encoding	= XMLLoader::UTF_8;	*/
		if (0 != count)
		{
			// On effectue une copie de sauvegarde du fichier d'origine :
			File	originalFile (config);
			File	backupFile (createBackupFileName (originalFile));
			unique_ptr<Process>	copy (new Process ("cp"));
			copy->getOptions ( ).addOption (originalFile.getFullFileName ( ));
			copy->getOptions ( ).addOption (backupFile.getFullFileName ( ));
			copy->execute (false);
			copy->wait ( );
			modifications << "\n"
				<< "Fichier de préférences " << originalFile.getFullFileName ( )
				<< " sauvegardé dans " << backupFile.getFullFileName ( ) << ".";
			
			// On écrase le fichier d'origine avec la nouvelle configuration :
			XMLLoader::save (*modifiedCfg.get ( ), config, encoding);
		}	// if (0 != count)
	}
	catch (const Exception& exc)
	{
		UTF8String	error (charset);
		error << "Erreur lors de l'application des correctifs au fichier "
		      << config << " contenus dans le fichier " << patchs
		      << " :\n" << exc.getFullMessage ( );
		throw Exception (error);
	}
	catch (const exception& e)
	{
		UTF8String	error (charset);
		error << "Erreur lors de l'application des correctifs au fichier "
		      << config << " contenus dans le fichier " << patchs << " :\n"
		      << e.what ( );
		throw Exception (error);
	}
	catch (...)
	{
		UTF8String	error (charset);
		error << "Erreur lors de l'application des correctifs au fichier "
		      << config << " contenus dans le fichier " << patchs << " :\n" 
		      << "Erreur non documentée.";
		throw Exception (error);
	}

	return count;
}	// QtPrefsUpdater::update


size_t QtPrefsUpdater::update (
	Section& config, Section& patchs,
	const UTF8String& appTitle, const UTF8String& context,
	UTF8String& message, bool& confirm, bool& operationStopped)
{
	size_t	count	= 0;

	vector<Section*>	sections	= patchs.getSections ( );
	for (vector<Section*>::const_iterator its	= sections.begin ( );
	     (false == operationStopped) && (sections.end ( ) != its); its++)
	{
		bool	hasSection	= false;
		try
		{
//cout << "Actualisation des préférences de la section " << (*its)->getName ( ) << endl;
			Section&	section	= config.getSection ((*its)->getName ( ));
			hasSection	= true;
			count	+= update (
						section, **its, appTitle, context, message, confirm,
						operationStopped);
//cout << "Préférences de la section " << (*its)->getName ( ) << " actualisées." << endl;
		}
		catch (...)
		{
			if (true == hasSection)
				throw;
		}
	}	// for (vector<Section*>::const_iterator its   = ...

	vector<NamedValue*>	elements	= patchs.getNamedValues ( );
	for (vector<NamedValue*>::const_iterator ite	= elements.begin ( );
	     (false == operationStopped) && (elements.end ( ) != ite); ite++)
	{
		if (false == config.hasElement ((*ite)->getName ( )))
			continue;

		NamedValue&	value	= config.getNamedValue ((*ite)->getName ( ));
		if ((*ite)->getStrValue ( ) != value.getStrValue ( ))
		{
//cout << "Actualisation de la préférence " << (*ite)->getName ( ) << endl;
			const UTF8String	oldValue	= value.getStrValue ( );
			if (true == confirm)
			{
				UTF8String	question (charset);
				if (0 == count)
					question << context << "\n";
				question << "Acceptez-vous de remplacer la préférence "
				         << getFullName (**ite) << " de valeur "
				         << oldValue << " par " << (*ite)->getStrValue ( )
				         << " ?";
				QtExtMessageBox	dialog (
						UTF8String (appTitle), 0, question,
						UTF8String ("Oui", charset),UTF8String ("Non", charset),
						UTF8String ("Annuler", charset),
						UTF8String ("Ne plus poser cette question", charset),
						QMessageBox::Question, 2, false);
				switch (dialog.exec ( ))
				{
					case	1	:
						if (true == dialog.getCheckBoxValue ( ))
							confirm	= false;
						break;
					case	2	:
						if (true == dialog.getCheckBoxValue ( ))
						{
							confirm	= false;
							operationStopped	= true;
							return count;
						}
						continue;
					case	3	:
						throw Exception(UTF8String (
							"Opération annulée par l'utilisateur.", charset));
				}	// switch (dialog.exec ( ))

			}	// if (true == confirm)
			value.setStrValue ((*ite)->getStrValue ( ));
			value.setComment ((*ite)->getComment ( ));
			message << "\n" << "Actualisation de la préférence "
			        << getFullName (**ite) << " : " 
			        << oldValue << " -> " << (*ite)->getStrValue ( ) << ".";
			count++;
//cout << "Préférence " << (*ite)->getName ( ) << " actualisée." << endl;
		}	// if ((*ite)->getStrValue ( ) != value.getStrValue ( ))
	}	// for (vector<NamedValue*>::const_iterator ite   = ...

	return count;
}	// QtPrefsUpdater::update


END_NAMESPACE_PREFS

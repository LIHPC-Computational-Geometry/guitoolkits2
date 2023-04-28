#include "QtPlugin/QtPluginRunnerDialog.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/InternalError.h>
#include <TkUtil/MemoryError.h>
#include <PrefsCore/BoolNamedValue.h>
#include <PrefsCore/StringNamedValue.h>
#include <PrefsCore/LongNamedValue.h>
#include <PrefsCore/UnsignedLongNamedValue.h>
#include <PrefsCore/DoubleNamedValue.h>

#include <assert.h>

USING_STD
USING_UTIL
USING_PREFS
using namespace PLUGIN;

static const Charset	charset ("àéèùô");
USE_ENCODING_AUTODETECTION


const Version QtPluginRunnerDialog::_version (PLUGIN_VERSION);


QtPluginRunnerDialog::QtPluginRunnerDialog (
				QWidget* parent, Section* section, Plugin* plugin,
                const string& fromFile, bool withApply, bool modal, 
                const string& dlgTitle, const string& appTitle)
	: QtPreferencesDialog (parent, section, fromFile, true /* editable */,
	                       withApply, modal, dlgTitle, appTitle),
	  _plugin (plugin), _pluginParentSection (0), _safeGuardSection (0)
{
	if (0 == section)
		throw Exception (UTF8STR ("Boite de dialogue d'édition des plugins : absence de paramètres de plugin."));
	if (0 == plugin)
		throw Exception (UTF8STR ("Boite de dialogue d'édition des plugins : absence de paramètres plugin."));

	QMenuBar*	menu	= getMenuBar ( );
	if (0 != menu)
	{
		// Le menu Plugin :
#ifdef QT_3
		QPopupMenu*	pluginMenu	= new QPopupMenu (this);
		menu->insertItem ("&Plugin", pluginMenu);

		// Les items :
		int	id	= pluginMenu->insertItem ("&Exécuter", this,
										SLOT(runPluginCallback ( )));
		pluginMenu->setWhatsThis (id, "Affiche une boite de dialogue de lancement de plugins.");
#else	// => !QT_3
		QMenu*		pluginMenu	= new QMenu ("&Plugin", this);
		menu->addMenu (pluginMenu);
		QAction*	action		= pluginMenu->addAction ("&Exécuter ...", this, SLOT(runPluginCallback ( )));
		action->setWhatsThis ("Affiche une boite de dialogue de lancement de plugins.");
#endif	// else QT_3
	}	// if (0 != menu)
}	// QtPluginRunnerDialog::QtPluginRunnerDialog


QtPluginRunnerDialog::QtPluginRunnerDialog (const QtPluginRunnerDialog&)
	: QtPreferencesDialog (0, 0, "", false, false, false, "", ""),
	  _plugin (0), _pluginParentSection (0), _safeGuardSection (0)
{
	assert (0 && "QtPluginRunnerDialog copy constructor is not allowed.");
}	// QtPluginRunnerDialog::QtPluginRunnerDialog (const QtPluginRunnerDialog&)


QtPluginRunnerDialog& QtPluginRunnerDialog::operator = (
												const QtPluginRunnerDialog&)
{
	assert (0 && "QtPluginRunnerDialog::operator = is not allowed.");
	return *this;
}	// QtPluginRunnerDialog::operator =


QtPluginRunnerDialog::~QtPluginRunnerDialog ( )
{
	delete _pluginParentSection;
	delete _safeGuardSection;
}	// QtPluginRunnerDialog::~QtPluginRunnerDialog


const Section& QtPluginRunnerDialog::getSafeguardedSection ( )
{
	delete _safeGuardSection;	_safeGuardSection	= 0;
	Section*	root	= 0 == getPluginParentSection ( ) ?
			  0 : dynamic_cast<Section*>(getPluginParentSection ( )->clone ( ));
	Section*	parent	= root;

	if (0 != parent)
		while (0 != parent->getSections ( ).size ( ))
			parent	= parent->getSections ( ) [0];

	Section*	copy	= 
		dynamic_cast<Section*>(QtPreferencesDialog::getSection ( ).clone ( ));
	if (0 != parent)
		parent->addSection (copy);

	_safeGuardSection	= 0 == root ? copy : root;
	return *_safeGuardSection;
}	// QtPluginRunnerDialog::getSection


Plugin& QtPluginRunnerDialog::getPlugin ( )
{
	if (0 == _plugin)
	{
		INTERNAL_ERROR (exc, "Absence de fonction plugin.", "QtPluginRunnerDialog::getPlugin")
		throw exc;
	}	// if (0 == _plugin)

	return *_plugin;
}	// QtPluginRunnerDialog::getPlugin


const Plugin& QtPluginRunnerDialog::getPlugin ( ) const
{
	if (0 == _plugin)
	{
		INTERNAL_ERROR (exc, "Absence de fonction plugin.", "QtPluginRunnerDialog::getPlugin")
		throw exc;
	}	// if (0 == _plugin)

	return *_plugin;
}	// QtPluginRunnerDialog::getPlugin


Section* QtPluginRunnerDialog::getPluginParentSection ( ) const
{
	return _pluginParentSection;
}	// QtPluginRunnerDialog::getPluginParentSection


void QtPluginRunnerDialog::setPluginParentSection (Section* section)
{
	if (_pluginParentSection == section)
		return;

	delete _pluginParentSection;
	_pluginParentSection	= section;
}	// QtPluginRunnerDialog::setPluginParentSection


void QtPluginRunnerDialog::runPluginCallback ( )
{
	try
	{
		reinitializePlugin ( );
		Plugin&		plugin		= getPlugin ( );
		plugin.execute ( );
		if (0 != plugin.getErrorMessage ( ).length ( ))
			throw Exception (plugin.getErrorMessage ( ));
		if (0 != plugin.getWarningMessage ( ).length ( ))
			throw Exception (plugin.getWarningMessage ( ));
	}
	catch (const Exception& e)
	{
		UTF8String	message ("Erreur lors de l'exécution de la fonction plugin :\n", charset);
		message	+= e.getFullMessage ( );
		displayErrorMessage (message);
	}
	catch (...)
	{
		displayErrorMessage (UTF8STR (
			"Erreur non documentée lors de l'exécution de la fonction plugin."));
	}
}	// QtPluginRunnerDialog::runPluginCallback


void QtPluginRunnerDialog::reinitializePlugin ( )
{
	Plugin&			plugin		= getPlugin ( );
	plugin.reset ( );

	// Le nom de la bibliothèque et de la fonction appelée :
	const Section&			pluginSection	= getSection ( );
	const StringNamedValue&	libValue		=
		dynamic_cast<StringNamedValue&>(pluginSection.getNamedValue("library"));
	const string	library	= libValue.getStrValue ( );
	const StringNamedValue&	functionValue		=
		dynamic_cast<StringNamedValue&>(pluginSection.getNamedValue("function"));
	const string	functName= functionValue.getStrValue ( );
	plugin.setLibraryName (library);
	plugin.setSymbolName (functName);

	// Reconstituer la liste des arguments :
	if (false == pluginSection.hasSection ("arguments"))
		return;
	const Section&			argsSection	= pluginSection.getSection("arguments");
	PluginFunction&		function	= plugin.getFunction ( );
	vector<NamedValue*>	namedValues	= argsSection.getNamedValues ( );
	const char*			err			= NULL;
	UTF8String			error ("Impossible d'affecter à la fonction ", charset);
	error	+= functName + string (" l'argument ");
	for (vector<NamedValue*>::iterator	it	= namedValues.begin ( );
	     namedValues.end ( ) != it; it++)
	{
		CHECK_NULL_PTR_ERROR (*it)

		if (StringNamedValue::typeName == (*it)->getType ( ))
		{
			StringNamedValue*	strValue	=
								dynamic_cast<StringNamedValue*>(*it);
			CHECK_NULL_PTR_ERROR (strValue)
			err	= setStringArg (&function, (*it)->getName ( ).ascii( ).c_str( ),
				                strValue->getValue ( ).ascii ( ).c_str ( ));
			if (NULL != err)
			{
				error	+= (*it)->getName ( ).ascii ( ) + string (" : ") + 
						   string (err);
				throw Exception (error);
			}	// if (NULL != err)
		}	// if (StringNamedValue::typeName == (*it)->getType ( ))
		else if (BoolNamedValue::typeName == (*it)->getType ( ))
		{
			BoolNamedValue*	boolValue	= dynamic_cast<BoolNamedValue*>(*it);
			CHECK_NULL_PTR_ERROR (boolValue)
			err	= setBooleanArg (&function, (*it)->getName( ).ascii( ).c_str( ),
				                 true == boolValue->getValue ( ) ? 1 : 0);
			if (NULL != err)
			{
				error	+= (*it)->getName ( ).ascii ( ) + string (" : ") + 
						   string (err);
				throw Exception (error);
			}	// if (NULL != err)
		}	// if (BoolNamedValue::typeName == (*it)->getType ( ))
		else if (LongNamedValue::typeName == (*it)->getType ( ))
		{
			LongNamedValue*	longValue	= dynamic_cast<LongNamedValue*>(*it);
			CHECK_NULL_PTR_ERROR (longValue)
			err	= setLongArg (&function, (*it)->getName ( ).ascii ( ).c_str ( ),
				              longValue->getValue ( ));
			if (NULL != err)
			{
				error	+= (*it)->getName ( ).ascii ( ) + string (" : ") + 
						   string (err);
				throw Exception (error);
			}	// if (NULL != err)
		}	// if (LongNamedValue::typeName == (*it)->getType ( ))
		else if (UnsignedLongNamedValue::typeName == (*it)->getType ( ))
		{
			UnsignedLongNamedValue*	uLongValue	= 
								dynamic_cast<UnsignedLongNamedValue*>(*it);
			CHECK_NULL_PTR_ERROR (uLongValue)
			err	= setULongArg (&function, (*it)->getName ( ).ascii ( ).c_str( ), 
				               uLongValue->getValue ( ));
			if (NULL != err)
			{
				error	+= (*it)->getName ( ).ascii ( ) + string (" : ") + 
						   string (err);
				throw Exception (error);
			}	// if (NULL != err)
		}	// if (UnsignedLongNamedValue::typeName == (*it)->getType ( ))
		else if (DoubleNamedValue::typeName == (*it)->getType ( ))
		{
			DoubleNamedValue*	doubleValue	= 
									dynamic_cast<DoubleNamedValue*>(*it);
			CHECK_NULL_PTR_ERROR (doubleValue)
			err	= setDoubleArg (&function, (*it)->getName ( ).ascii( ).c_str( ),
				                doubleValue->getValue ( ));
			if (NULL != err)
			{
				error	+= (*it)->getName ( ).ascii ( ) + string (" : ") + 
						   string (err);
				throw Exception (error);
			}	// if (NULL != err)
		}	// if (DoubleNamedValue::typeName == (*it)->getType ( ))
		else
		{
			error	+= (*it)->getName ( ).ascii ( ) + string (" de type ")
					+ (*it)->getType ( ) + string (" type non supporté.");
			throw Exception (error);
		}	// else if ...
	}	// for (vector<NamedValue*>::iterator  it  = namedValues.begin ( ); ...
}	// QtPluginRunnerDialog::reinitializePlugin



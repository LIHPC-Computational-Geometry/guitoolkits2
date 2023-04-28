#include "PrefsQt/QtPreferencesDialog.h"
#include "PrefsXerces/XMLLoader.h"
#include <TkUtil/UTF8String.h>
#include <TkUtil/Exception.h>
#include <QtUtil/QtMessageBox.h>
#include <QtUtil/QtConfiguration.h>
#include <QtUtil/QtUnicodeHelper.h>

#include <QFileInfo>
#include <QCursor>
#ifndef QT_5
#include <QtGui/QLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QApplication>
#include <QtGui/QLabel>
#else	// QT_5
#include <QLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QApplication>
#include <QLabel>
#endif	// QT_5
#include <QtUtil/CharToLatinQString.h>

#include <memory>
#include <iostream>
#include <assert.h>


USING_STD
USING_UTIL
USING_PREFS

static const Charset    charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtPreferencesDialog::QtPreferencesDialog (
	QWidget* parent, Section* section, const string& fromFile,
	bool editable, bool withApply, bool modal, 
	const UTF8String& dlgTitle, const UTF8String& appTitle)
	: QDialog (true == modal ? parent : 0,
	           true == modal ? QtConfiguration::modalDialogWFlags :
	                           QtConfiguration::amodalDialogWFlags),
	 _prefsPanel (0), _statusBar (0), 
	 _appTitle (appTitle), _sectionFile (fromFile),
	 _menuBar (0), _helpMenu (0)
{
	setWindowTitle (UTF8TOQSTRING (dlgTitle));
	setModal (modal);

	QVBoxLayout*	layout	= new QVBoxLayout (this);
	layout->setMargin (10);
	layout->setSpacing (10);
	layout->setSizeConstraint (QLayout::SetMinimumSize);
	_menuBar	= createMenu (editable, withApply);
	layout->addWidget (_menuBar);
	_prefsPanel	= new QtPreferencesPanel (this, section, editable, appTitle);
	layout->addWidget (_prefsPanel);
	_statusBar	= new QStatusBar (this);
	_statusBar->showMessage (fromFile.c_str ( ));
	layout->addWidget (_statusBar);
	layout->activate ( );
	setMinimumSize (layout->sizeHint ( ));
	// v 2.29.1 : taille un peu plus grande par défaut :
	QSize	size	= layout->sizeHint ( );
	size	*= 2;
	resize (size);
}	// QtPreferencesDialog::QtPreferencesDialog


QtPreferencesDialog::QtPreferencesDialog (const QtPreferencesDialog&)
	: QDialog (0), _prefsPanel (0), _sectionFile (""), _appTitle ( ),
	 _menuBar (0), _helpMenu (0)
{
	assert (0 && "QtPreferencesDialog copy constructor is not allowed.");
}	// QtPreferencesDialog::QtPreferencesDialog


QtPreferencesDialog& QtPreferencesDialog::operator = (const QtPreferencesDialog&)
{
	assert (0 && "QtPreferencesDialog operator = is not allowed.");
	return *this;
}


QtPreferencesDialog::~QtPreferencesDialog ( )
{
}	// QtPreferencesDialog::~QtPreferencesDialog


Section* QtPreferencesDialog::releaseSection ( )
{
	assert ((0 != _prefsPanel) && "QtPreferencesDialog::releaseSection : null preference panel.");
	assert ((0 != _statusBar) && "QtPreferencesDialog::releaseSection : null status bar.");
	_statusBar->showMessage (" ");
	return _prefsPanel->releaseSection ( );
}	// QtPreferencesDialog::releaseSection


bool QtPreferencesDialog::hasSection ( ) const
{
	assert ((0 != _prefsPanel) && "QtPreferencesDialog::hasSection : null preference panel.");
	return _prefsPanel->hasSection ( );
}	// QtPreferencesDialog::hasSection


const Section& QtPreferencesDialog::getSection ( ) const
{
	assert ((0 != _prefsPanel) && "QtPreferencesDialog::getSection : null preference panel.");
	return _prefsPanel->getSection ( );
}	// QtPreferencesDialog::getSection


const Section& QtPreferencesDialog::getSafeguardedSection ( )
{
	return getSection ( );
}	// QtPreferencesDialog::getSection


bool QtPreferencesDialog::isEditable ( ) const
{
	assert ((0 != _prefsPanel) && "QtPreferencesDialog::isEditable : null preference panel.");
	return _prefsPanel->isEditable ( );
}	// QtPreferencesDialog::isEditable ( )


QMenuBar* QtPreferencesDialog::createMenu (bool editable, bool withApply)
{
	QMenuBar*	menu	= new QMenuBar (this);

	// Le menu "Fichier" :
	QMenu*		fileMenu	= new QMenu ("&Fichier", this);
	menu->addMenu (fileMenu);
	QAction*	action	= 0;
	if (true == editable)
	{
		action	= fileMenu->addAction ("Ouvrir ...", this, SLOT(loadCallback ( )));
		action->setWhatsThis ("Affiche une boite de dialogue de sélection d'un nouveau fichier de préférences.");
		action = fileMenu->addAction ("Enregistrer", this, SLOT(saveCallback ( )));
		action->setWhatsThis ("Enregistre les préférences dans le fichier courant.");
		action = fileMenu->addAction ("Enregistrer sous ...", this, SLOT(saveAsCallback ( )));
		action->setWhatsThis ("Affiche une boite de dialogue de sélection d'un nouveau fichier pour enregistrer les préférences.");

		fileMenu->addSeparator ( );
		if (true == withApply)
		{
			action = fileMenu->addAction (
						"&Appliquer", this, SLOT(applyCallback ( )));
			action->setWhatsThis ("Applique ces préférences à l'application.");
			fileMenu->addSeparator ( );
		}	// if (true == withApply)
	}	// if (true == editable)
	action = fileMenu->addAction ("Fermer", this, SLOT (hide ( )));
	action->setWhatsThis ("Ferme cette boite de dialogue.");

	if (false == editable)
		return menu;

	// Le menu "Edition" :
	QMenu*	editMenu	= new QMenu ("&Editer", this);
	menu->addMenu (editMenu);
	action = editMenu->addAction ("Insérer une section ...", this, 
	                      SLOT(insertSectionCallback ( )));
	editMenu->setWhatsThis ("Affiche une boite de dialogue d'insertion de section.");
	action = editMenu->addAction ("Insérer un booléen ...", this, 
	                      SLOT(insertBooleanCallback ( )));
	editMenu->setWhatsThis ("Affiche une boite de dialogue d'insertion d'un bouléen dans la section courante.");
	action = editMenu->addAction ("Insérer une chaine de caractères ...",  
	                      this, SLOT(insertStringCallback ( )));
	editMenu->setWhatsThis ("Affiche une boite de dialogue d'insertion d'une chaine de caractères dans la section courante.");
	action = editMenu->addAction ("Insérer un entier long ...", this, 
	                      SLOT(insertLongCallback ( )));
	editMenu->setWhatsThis ("Affiche une boite de dialogue d'insertion d'un entier long dans la section courante.");
	action = editMenu->addAction ("Insérer un entier long non signé ...",  
	                      this, SLOT(insertUnsignedLongCallback ( )));
	editMenu->setWhatsThis ("Affiche une boite de dialogue d'insertion d'un entier long non signé dans la section courante.");
	action = editMenu->addAction ("Insérer un réel double précision ...",
	                      this, SLOT(insertDoubleCallback ( )));
	editMenu->setWhatsThis ("Affiche une boite de dialogue d'insertion d'un réel double précision dans la section courante.");
	action = editMenu->addAction (
		"Insérer un triplet de réels double précision ...", this,
	                      SLOT(insertDoubleTripletCallback ( )));
	editMenu->setWhatsThis ("Affiche une boite de dialogue d'insertion d'un triplet de réels double précision dans la section courante.");
	action = editMenu->addAction ("Insérer une couleur ...", this,
	                      SLOT(insertColorCallback ( )));
	editMenu->setWhatsThis ("Affiche une boite de dialogue d'insertion d'une couleur dans la section courante.");
	editMenu->addSeparator ( );
	action = editMenu->addAction ("Supprimer", this, SLOT(removeCallback ( )));
	editMenu->setWhatsThis ("Supprime l'élément sélectionné.");
	editMenu->addSeparator ( );
	action = editMenu->addAction ("Commentaires ...", this, 
	                           SLOT(editCommentCallback ( )));

	// Le menu "Aide" :
	_helpMenu	= new QMenu ("&Aide", this);
	menu->addMenu (_helpMenu);
	action = _helpMenu->addAction ("A propos de ...", this, 
	                      SLOT(aboutCallback ( )));
	action->setWhatsThis ("Affiche une boite de dialogue d'informations sur cette boite de dialogue.");

	return menu;
}	// QtPreferencesDialog::createMenu


void QtPreferencesDialog::displayErrorMessage (const UTF8String& message)
{
	QtMessageBox::displayErrorMessage (this, _appTitle, message);
}	// QtPreferencesDialog::displayErrorMessage


void QtPreferencesDialog::loadCallback ( )
{
	assert (0 != _prefsPanel);
	assert (0 != _statusBar);

	try
	{
		QFileInfo	path (_sectionFile.c_str ( ));
		QFileDialog	dialog (this, UTF8TOQSTRING (_appTitle),
					        path.absoluteFilePath ( ), "*.*");
		dialog.setFileMode (QFileDialog::ExistingFile);
		dialog.setAcceptMode (QFileDialog::AcceptOpen);
		dialog.setOption (QFileDialog::DontUseNativeDialog);	// v 4.6.1
		if (QDialog::Accepted != dialog.exec ( ))
			return;

		// Charger la nouvelle config:
		QStringList	files	= dialog.selectedFiles ( );
		_sectionFile	= files [0].toStdString ( );
		unique_ptr<Section> section (XMLLoader::load (_sectionFile.c_str ( )));
		_statusBar->showMessage (_sectionFile.c_str ( ));
		_prefsPanel->setRootSection (section.get ( ));
		section.release ( );
	}
	catch (const Exception& e)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Erreur lors du chargement d'une configuration :\n"
		         << e.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		displayErrorMessage (UTF8STR (
			"Erreur non documentée lors du chargement d'une configuration."));
	}
}	// QtPreferencesDialog::loadCallback


void QtPreferencesDialog::saveCallback ( )
{
	assert (0 != _prefsPanel);
	if (0 == _sectionFile.length ( ))
	{
		saveAsCallback ( );
		return;
	}	// if (0 == _sectionFile.length ( ))

	try
	{
		if (true == _prefsPanel->hasSection ( ))
		{
			const Section&	safeguardedSection	= getSafeguardedSection ( );
			XMLLoader::ENCODING		encoding	= XMLLoader::UTF_8;	// v 5.5.6
//			XMLLoader::ENCODING		encoding	= XMLLoader::UTF_16;
//			if (true == safeguardedSection.isIso ( ))
//				encoding	= XMLLoader::ISO_8859;
//			if (true == safeguardedSection.isAscii ( ))
//				encoding	= XMLLoader::UTF_8;
			XMLLoader::save (safeguardedSection, _sectionFile, encoding);
		}	// if (true == _prefsPanel->hasSection ( ))
	}
	catch (const Exception& e)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Erreur lors de l'enregistrement de la configuration dans "
		         << "le fichier " << _sectionFile << " :\n"
		         << e.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		displayErrorMessage (UTF8STR (
			"Erreur non documentée lors de l'enregistrement de la configuration."));
	}
}	// QtPreferencesDialog::saveCallback


void QtPreferencesDialog::saveAsCallback ( )
{
	assert (0 != _prefsPanel);
	assert (0 != _statusBar);

	try
	{
		QFileInfo	path (_sectionFile.c_str ( ));
		QFileDialog	dialog (this, UTF8TOQSTRING (_appTitle),
					        path.absoluteFilePath ( ), "*.*");
		dialog.setFileMode (QFileDialog::AnyFile);
		dialog.setAcceptMode (QFileDialog::AcceptSave);
		dialog.setOption (QFileDialog::DontUseNativeDialog);	// v 4.6.1
		if (QDialog::Accepted != dialog.exec ( ))
			return;

		QStringList	files		= dialog.selectedFiles ( );
		QString		fileName	= files [0];
		string		stdFileName	= fileName.toStdString ( );
		QFileInfo	file (fileName);
		if (((true == file.exists ( )) && (false == file.isWritable ( ))) ||
		    (false == QFileInfo (file.dir ( ).absolutePath ( )).isWritable ( )))
		{
			UTF8String	message (charset);
			message << "Vous n'avez pas les droits en écriture sur le "
			        << "fichier\n" << stdFileName;
			displayErrorMessage (message);
			return;
		}

		// Ce fichier existe-t-il deja ?
		if (true == file.exists ( ))
		{
			UTF8String	message (charset);
			message << "Le fichier " << stdFileName
			        << "\nexiste déjà. Souhaitez-vous l'écraser ?";
			if (0 != QMessageBox::warning (this, UTF8TOQSTRING (_appTitle),
							UTF8TOQSTRING (message),
							"Oui", "Non", 0, 1))
				return;
		}

		_sectionFile	= stdFileName;
		_statusBar->showMessage (_sectionFile.c_str ( ));
		saveCallback ( );
	}
	catch (const Exception& e)
	{
		UTF8String	errorMsg (charset);
		errorMsg << "Erreur lors du chargement d'une configuration :\n"
		         << e.getMessage ( );
		displayErrorMessage (errorMsg);
	}
	catch (...)
	{
		displayErrorMessage (UTF8STR (
			"Erreur non documentée lors du chargement d'une configuration."));
	}
}	// QtPreferencesDialog::saveAsCallback


void QtPreferencesDialog::applyCallback ( )
{
	cerr << "ERREUR : QtPreferencesDialog::applyCallback non surchargée."
	     << endl;
}	// QtPreferencesDialog::applyCallback


void QtPreferencesDialog::insertSectionCallback ( )
{
	assert (0 != _prefsPanel);
	_prefsPanel->insertSection ( );
}	// QtPreferencesDialog::insertSectionCallback


void QtPreferencesDialog::insertBooleanCallback ( )
{
	assert (0 != _prefsPanel);
	_prefsPanel->insertBooleanNamedValue ( );
}	// QtPreferencesDialog::insertBooleanCallback


void QtPreferencesDialog::insertColorCallback ( )
{
	assert (0 != _prefsPanel);
	_prefsPanel->insertColorNamedValue ( );
}	// QtPreferencesDialog::insertColorCallback


void QtPreferencesDialog::insertStringCallback ( )
{
	assert (0 != _prefsPanel);
	_prefsPanel->insertStringNamedValue ( );
}	// QtPreferencesDialog::insertStringCallback


void QtPreferencesDialog::insertLongCallback ( )
{
	assert (0 != _prefsPanel);
	_prefsPanel->insertLongNamedValue ( );
}	// QtPreferencesDialog::insertLongCallback


void QtPreferencesDialog::insertUnsignedLongCallback ( )
{
	assert (0 != _prefsPanel);
	_prefsPanel->insertUnsignedLongNamedValue ( );
}	// QtPreferencesDialog::insertUnsignedLongCallback


void QtPreferencesDialog::insertDoubleCallback ( )
{
	assert (0 != _prefsPanel);
	_prefsPanel->insertDoubleNamedValue ( );
}	// QtPreferencesDialog::insertDoubleCallback


void QtPreferencesDialog::insertDoubleTripletCallback ( )
{
	assert (0 != _prefsPanel);
	_prefsPanel->insertDoubleTripletNamedValue ( );
}	// QtPreferencesDialog::insertDoubleTripletCallback


void QtPreferencesDialog::removeCallback ( )
{
	assert (0 != _prefsPanel);
	_prefsPanel->removeSelection ( );
}	// QtPreferencesDialog::removeCallback


void QtPreferencesDialog::editCommentCallback ( )
{
	assert (0 != _prefsPanel);
	_prefsPanel->editComment ( );
}	// QtPreferencesDialog::editCommentCallback


void QtPreferencesDialog::aboutCallback ( )
{
	UTF8String	aboutMsg (charset);

	aboutMsg << "Réalisation\t: Charles PIGNEROL (CEA/DAM/DSSI, tel : 5291)"
	         << "\n\n"
	         << "Format de fichier\t: " << XMLLoader::version.getVersion ( ) 
	         << "\n"
	         << "Xerces C++\t: " << XERCES_FULLVERSIONDOT << "\n";

	QMessageBox::about (this, QSTR ("Gestionnaire de préférences utilisateur"), 
	                    UTF8TOQSTRING (aboutMsg));
}	// QtPreferencesDialog::aboutCallback




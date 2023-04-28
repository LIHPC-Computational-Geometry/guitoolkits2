#include "QtNetWork/QtNetworkFileDialog.h"
#include <TkUtil/Exception.h>
#include <TkUtil/NetworkData.h>
#include <TkUtil/util_config.h>
#include <TkUtil/NumericConversions.h>
#include <TkUtil/RemoteProcess.h>
#include <TkUtil/UserData.h>
#include <QtUtil/QtAutoWaitingCursor.h>
#include <QtUtil/QtObjectSignalBlocker.h>
#include "QtUtil/QtMessageBox.h"
#include <QtUtil/QtUnicodeHelper.h>

#include <QDate>
#include <QFileIconProvider>
#include <QPushButton>
#include <QHBoxLayout>
#include <QListView>
#include <QListWidgetItem>

#include <assert.h>
#include <ctype.h>
#include <memory>
#include <sstream>


USING_STD
USING_UTIL

static const TkUtil::Charset	charset ("àéèùô");

USE_ENCODING_AUTODETECTION

// ============================ FONCTIONS STATIQUES ============================

/**
 * @param		Droits sur un fichier sous la forme "rwxrwxrwx".
 * @return		Les droits "Qt" sur un fichier.
 */
/* v 3.7.0
static int qtPermissions (const string& perms)
{
	// A partir de la version 2.41.1, supporte également le pattern
	// "rwxrwxrwx.".
	if (9 != perms.length ( ))
	{
		if ((10 != perms.length ( )) || ('.' != perms [9]))
		{
			UnicodeString	error;
			error << "QtRSHNetworkProtocol::qtPermissions : pattern ("
			      << perms << ") de droits sur les fichiers non supporté."
			      << "Patterns supportés : \"rwxrwxrwx\" et \"rwxrwxrwx.\".";
			throw Exception (error);
		}	// if ((10 != perms.length ( )) || ('.' != perms [9]))
	}	// if (9 != perms.length ( ))

	const char*	ptr	= perms.c_str ( );
	int			permissions	= 0;

	if (*ptr++ == 'r')
		permissions	|= QUrlInfo::ReadOwner;
	if (*ptr++ == 'w')
		permissions	|= QUrlInfo::WriteOwner;
	if (*ptr++ == 'x')
		permissions	|= QUrlInfo::ExeOwner;
	if (*ptr++ == 'r')
		permissions	|= QUrlInfo::ReadGroup;
	if (*ptr++ == 'w')
		permissions	|= QUrlInfo::WriteGroup;
	if (*ptr++ == 'x')
		permissions	|= QUrlInfo::ExeGroup;
	if (*ptr++ == 'r')
		permissions	|= QUrlInfo::ReadOther;
	if (*ptr++ == 'w')
		permissions	|= QUrlInfo::WriteOther;
	if (*ptr++ == 'x')
		permissions	|= QUrlInfo::ExeOther;

	return permissions;
}	// qtPermissions
*/


/**
 * @param		la date sous forme hh:mm ou yyyy
 * @return		l'année
 */
/* v 2.25.0
static int year (const char* date)
{
	if (4 == strlen (date))
		return (int)NumericConversions::strToULong (date);

	return QDate::currentDate ( ).year ( );
}	// year
v 2.25.0 */


/**
 * @param		le mois sous forme Jan, Feb, ...
 * @return		le mois, ou 0
 */
/* v 2.25.0
static int month (const char* date)
{
	if (0 == strcmp (date, "Jan"))
		return 1;
	if (0 == strcmp (date, "Feb"))
		return 2;
	if (0 == strcmp (date, "Mar"))
		return 3;
	if (0 == strcmp (date, "Apr"))
		return 4;
	if (0 == strcmp (date, "May"))
		return 5;
	if (0 == strcmp (date, "Jun"))
		return 6;
	if (0 == strcmp (date, "Jul"))
		return 7;
	if (0 == strcmp (date, "Aug"))
		return 8;
	if (0 == strcmp (date, "Sep"))
		return 9;
	if (0 == strcmp (date, "Oct"))
		return 10;
	if (0 == strcmp (date, "Nov"))
		return 11;
	if (0 == strcmp (date, "Dec"))
		return 12;

	return 0;
}	// month
v 2.25.0 */


/**
 * @param		la date sous forme hh:mm ou yyyy
 * @return		l'heure ou 0
 */
/* v 2.25.0
static int hour (const char* date)
{
	if (4 == strlen (date))
		return 0;

	if (5 == strlen (date))
		return (int)NumericConversions::strToULong (string (date).substr(0, 1));

	return 0;
}	// hour
v 2.25.0 */


/**
 * @param		la date sous forme hh:mm ou yyyy
 * @return		les minutes ou 0
 */
/* v 2.25.0
static int minutes (const char* date)
{
	if (4 == strlen (date))
		return 0;
	if (5 == strlen (date))
		return (int)NumericConversions::strToULong (string (date).substr(3, 4));

	return 0;
}	// minutes
v 2.25.0 */


/**
 * Compte le nombre de mots d'un ensemble de mots.
 */
static size_t countWords (const string& line)
{
	UTF8String	us (line);
	istringstream	stream (us.trim (true).utf8 ( ));
	size_t			wc  = 0;

	while ((true == stream.good ( )) && (false == stream.eof ( )))
	{
		string  str;
		wc++;
		stream >> str;
	}

	return wc - 1;
}   // countWords


static QStringList cottedListToList (const QString& str)
{
	QStringList	splitted	= str.split ('"', QString::SkipEmptyParts);
	const int	count		= splitted.size ( );
	QStringList	stringList;
	for (size_t i = 0; i < count; i++)
	{
		QString	treated	= splitted [i].trimmed ( );
		if (0 != treated.length ( ))
			stringList.push_back (treated);
	}	//  else if (0 == count)

	return stringList;
}	// cottedListToList


// =========================== LA CLASSE RemoteFile ============================

bool QtNetworkFileDialog::RemoteFile::exists ( ) const
{
	throw Exception (UTF8String ("RemoteFile::exists : méthode non implémentée.", charset));
}	// RemoteFile::exists


bool QtNetworkFileDialog::RemoteFile::isLink ( ) const
{
	throw Exception (UTF8String ("RemoteFile::isLink : méthode non implémentée.", charset));
}	// RemoteFile::isLink


bool QtNetworkFileDialog::RemoteFile::isReadable ( ) const
{
	throw Exception (UTF8String ("RemoteFile::isReadable : méthode non implémentée.", charset));
}	// RemoteFile::isReadable


bool QtNetworkFileDialog::RemoteFile::isWritable ( ) const
{
	throw Exception (UTF8String ("RemoteFile::isWritable : méthode non implémentée.", charset));
}	// RemoteFile::isWritable


bool QtNetworkFileDialog::RemoteFile::isExecutable ( ) const
{
	throw Exception (UTF8String ("RemoteFile::isExecutable : méthode non implémentée.", charset));
}	// RemoteFile::isExecutable


void QtNetworkFileDialog::RemoteFile::create ( bool asFile)
{
	throw Exception (UTF8String ("RemoteFile::create : méthode non implémentée.", charset));
}	// RemoteFile::create


void QtNetworkFileDialog::RemoteFile::reset ( )
{
	throw Exception (UTF8String ("RemoteFile::reset : méthode non implémentée.", charset));
}	// RemoteFile::reset


void QtNetworkFileDialog::RemoteFile::remove ( )
{
	throw Exception (UTF8String ("RemoteFile::remove : méthode non implémentée.", charset));
}	// RemoteFile::remove


// =========================== LA CLASSE QtInodItem ============================

QtNetworkFileDialog::QtInodItem::QtInodItem (
			const QtNetworkFileDialog::RemoteFile& inod, QListWidget * parent)
	: QListWidgetItem (getIcon (inod), inod.getFileName ( ).c_str ( ), parent),
	_inod (inod)
{
}	// QtInodItem::QtInodItem


QtNetworkFileDialog::QtInodItem::QtInodItem (const QtInodItem&)
	: QListWidgetItem (0), _inod ("", false)
{
	assert (0 && "QListWidgetItem copy constructor is not allowed.");
}	// QtInodItem::QtInodItem


QtNetworkFileDialog::QtInodItem& QtNetworkFileDialog::QtInodItem::operator = (
										const QtNetworkFileDialog::QtInodItem&)
{
	assert(0 && "QListWidgetItem operator = is not allowed.");
	return *this;
}	// QtInodItem::operator =


QIcon QtNetworkFileDialog::QtInodItem::getIcon (const QtNetworkFileDialog::RemoteFile& inod)
{
	static QFileIconProvider	iconProvider;
	QIcon	icon	= iconProvider.icon (true == inod.isDirectory ( ) ? QFileIconProvider::Folder: QFileIconProvider::File);
	return icon;
}	// QtInodItem::getIcon


// Issu de la fonction qt_clean_filter_list de qfiledialog.cpp de Qt 4.6.2
// A partir d'un filtre de type Image Files (*.png *.jpg) retourne une liste
// composée de *.png et *.jpg
static QStringList filterSetToFilters (const QString& filterSet)
{
	static const char *qt_file_dialog_filter_reg_exp =
		"([a-zA-Z0-9 -]*)\\(([a-zA-Z0-9_.*? +;#\\-\\[\\]@\\{\\}/!<>\\$%&=^~:\\|]*)\\)$";
	QRegExp	regexp (QString::fromLatin1 (qt_file_dialog_filter_reg_exp));
	QString	f	= filterSet.trimmed ( );	// trimmed indispensable
	int		i	= regexp.indexIn (f);
	if (i >= 0)
		f = regexp.cap (2);
	QStringList	flist	= f.split (QLatin1Char(' '), QString::SkipEmptyParts);

	return flist;
}	// filterSetToFilters


// A partir d'une liste d'ensemble de filtres nommés (ex : Fichiers ideas
// (*.unv); Fichiers Icem (*.ice *.uns)) retourne une liste composée de filtres
// non nommés (ex : *.unv *.ice *.uns).
static QStringList	filterRuleToFilters (const QString& filterSet)
{
	QStringList	filters;
	string::size_type	pos	= 0;
	const string		str	= filterSet.trimmed ( ).toStdString ( );

	while (string::npos != pos)
	{
		string				fs;
		string::size_type	current	= pos;
		pos	= str.find (';', current);

		if (string::npos != pos)
		{
			fs	= str.substr (current, pos - current);
			pos++;
		}	// if (string::npos != pos)
		else
		{
			fs	= str.substr (current);
			pos	= string::npos;
		}

		QStringList	currentFilters	= filterSetToFilters (fs.c_str ( ));
		for (int f = 0; f < currentFilters.count ( ); f++)
			filters.push_back (currentFilters [f]);
	}	// while (string::npos != pos)

	return filters;
}	// filterRuleToFilters


string purifyFilter (const string& str)
{
	UTF8String	filter;

	for (string::size_type i = 0; i < str.size ( ); i++)
	{
		const char	c	= str [i];
		if ((0 != isalnum (c)) || (0 != isblank (c)) || ('-' == c) ||
		    ('_' == c) || ('@' == c) || ('*' == c) || ('.' == c))
		{
			filter << c;
		}	// if ((0 != isalnum (c)) || (0 != isblank (c)) || ...
	}	// for (string::size_type i = 0; i < str.size ( ); i++)

	return filter.iso ( );
}	// purifyFilter


// ======================= LA CLASSE QtNetworkFileDialog =======================


string	QtNetworkFileDialog::homeCommand ("home");


QtNetworkFileDialog::QtNetworkFileDialog (QWidget* parent, const char* title, bool modal)
	: QDialog (parent),
	  _frozen (false), _directory ( ), _filter ( ),
	  _acceptMode (QFileDialog::AcceptOpen),
	  _fileMode (QFileDialog::ExistingFile),
	  _fileSystemListView (0), _filtersComboBox (0), _preselectionComboBox (0),
	  _directoryTextField (0), _fileNameLabel (0), _fileNameTextField (0),
	  _hostTextField (0)
{
	createGui (QSTR (title), NetworkData::getCurrentHostName ( ).c_str ( ));
	setModal (modal);
	update ( );
	updateWindowTitle ( );
}	// QtNetworkFileDialog::QtNetworkFileDialog


QtNetworkFileDialog::QtNetworkFileDialog (const QString& hostName, const QString& dirName, const QString& filter,
		QWidget* parent, const char* title, bool modal, bool frozen)
	: QDialog (parent),
	  _frozen (frozen), _directory (dirName), _filter (filter),
	  _acceptMode (QFileDialog::AcceptOpen),
	  _fileMode (QFileDialog::ExistingFile),
	  _fileSystemListView (0), _filtersComboBox (0), _preselectionComboBox (0),
	  _directoryTextField (0), _fileNameLabel (0), _fileNameTextField (0),
	  _hostTextField (0)
{
	createGui (title, hostName);
	setModal (modal);
	QStringList	filters;
	filters.push_back (filter);
	setNameFilters (filters);
//	update ( );	// v 3.1.0, called by setNameFilters
	updateWindowTitle ( );
}	// QtNetworkFileDialog::QtNetworkFileDialog


QtNetworkFileDialog::QtNetworkFileDialog (const QtNetworkFileDialog&)
{
	assert (0 && "QtNetworkFileDialog copy constructor is not allowed.");
}	// QtNetworkFileDialog::QtNetworkFileDialog (const QtNetworkFileDialog&)


QtNetworkFileDialog& QtNetworkFileDialog::operator = (const QtNetworkFileDialog&)
{
	assert (0 && "QtNetworkFileDialog operator = is not allowed.");
	return *this;
}	// QtNetworkFileDialog::operator =


QtNetworkFileDialog::~QtNetworkFileDialog ( )
{
}	// QtNetworkFileDialog::~QtNetworkFileDialog


void QtNetworkFileDialog::addPreselectedDirectories (const QStringList& directories)
{
	assert (0 != _preselectionComboBox);
	for (QStringList::const_iterator it	= directories.begin ( ); directories.end ( ) != it; it++)
		_preselectionComboBox->addItem (*it);
}	// QtNetworkFileDialog::addPreselectedDirectories


void QtNetworkFileDialog::createGui (const QString& title, const QString& hostName)
{
	setWindowTitle (title);
	Q_INIT_RESOURCE (QtNetwork);

	// Création du panneau principal :
	QGridLayout*	mainLayout	= new QGridLayout (this);
	setLayout (mainLayout);
	int	mainRow	= 0,	mainCol	= 0;

	// Saisie directe du path / boutons parent :
	int	row	= 0,	col	= 0;
	QWidget*		topPanel	= new QWidget (this);
	mainLayout->addWidget (topPanel, mainRow, mainCol++);
	QGridLayout*	layout		= new QGridLayout (topPanel);
	QLabel*	label	= new QLabel (QSTR ("Répertoire :"), topPanel);
	layout->addWidget (label, row, col++);
	_directoryTextField	= new QLineEdit (_directory, topPanel);
	layout->addWidget (_directoryTextField, row, col++);
	connect (_directoryTextField, SIGNAL (returnPressed ( )), this, SLOT (directoryChanged ( )));
	QIcon	homeIcon (":/images/home.png");
	QPushButton*	button	= new QPushButton (homeIcon, "", topPanel);
	button->setWhatsThis (QSTR ("répertoire racine"));
	layout->addWidget (button, row, col++);
	button->setAutoDefault (false);
	button->setDefault (false);
	connect (button, SIGNAL (clicked (bool)), this, SLOT (homeClicked (bool)));
	QIcon	parentIcon (":/images/up-32.png");
	button	= new QPushButton (parentIcon, "", topPanel);
	button->setWhatsThis ("parent");
	layout->addWidget (button, row, col++);
	button->setAutoDefault (false);
	button->setDefault (false);
	connect (button, SIGNAL (clicked (bool)), this, SLOT (parentClicked (bool)));

	// Vue système de fichiers :
	mainRow++;		mainCol	= 0;
	_fileSystemListView	= new QListWidget (this);
	_fileSystemListView->setFlow (QListWidget::TopToBottom);
	_fileSystemListView->setWrapping (true);
	_fileSystemListView->setResizeMode (QListWidget::Adjust);
	mainLayout->addWidget (_fileSystemListView, mainRow, mainCol);
	connect (_fileSystemListView, SIGNAL (itemClicked (QListWidgetItem*)), this, SLOT (itemClicked (QListWidgetItem*)));
	connect (_fileSystemListView, SIGNAL (itemDoubleClicked (QListWidgetItem*)), this, SLOT (itemDoubleClicked (QListWidgetItem*)));

	// Zone du bas : filtres, fichier, machine, et "Fermer"/"Annuler" :
	mainRow++;		mainCol	= 0;
	row	= 0,	col	= 0;
	QWidget*		bottomPanel	= new QWidget (this);
	mainLayout->addWidget (bottomPanel, mainRow, mainCol++);
	layout		= new QGridLayout (bottomPanel);
	// Rangée 0 : pré-sélection : v 5.3.0
	label	= new QLabel ("Présélection : ", bottomPanel);
	layout->addWidget (label, row, col++);
	_preselectionComboBox	= new QComboBox (bottomPanel);
	if (false == _directory.isEmpty ( ))
		_preselectionComboBox->addItem (_directory);
	connect (_preselectionComboBox, SIGNAL (currentIndexChanged (int)), this, SLOT (preselectionChanged (int)));
	layout->addWidget (_preselectionComboBox, row, col++);
	// Rangée 1 : Nom du fichier.
	row++;		col	= 0;
	_fileNameLabel	= new QLabel ("Nom du fichier : ", bottomPanel);
	layout->addWidget (_fileNameLabel, row, col++);
	_fileNameTextField	= new QLineEdit (bottomPanel);
	connect (_fileNameTextField, SIGNAL (textEdited (const QString&)), this,  SLOT (fileNameEdited (const QString&)));
	layout->addWidget (_fileNameTextField, row, col++);
	// Rangée 2 : Filtres sur les noms de fichiers
	row++;		col	= 0;
	label		= new QLabel ("Type de fichier : ", bottomPanel);
	layout->addWidget (label, row, col++);
	_filtersComboBox	= new QComboBox (bottomPanel);
	_filtersComboBox->addItem ("*");
	layout->addWidget (_filtersComboBox, row, col++);
	connect (_filtersComboBox, SIGNAL (currentIndexChanged (int)), this, SLOT (filtersChanged (int)));
	// Rangée 3 : Machine
	row++;		col	= 0;
	label		= new QLabel ("Machine  : ", bottomPanel);
	layout->addWidget (label, row, col++);
	_hostTextField	= new QLineEdit (hostName, bottomPanel);
	layout->addWidget (_hostTextField, row, col++);
	connect (_hostTextField, SIGNAL (returnPressed ( )), this, SLOT (hostChanged ( )));
	// Rangée 4 : "Fermer"/"Annuler" :
	row++;	col	= 0;
	QWidget*		buttons	= new QWidget (bottomPanel);
	QHBoxLayout*	hLayout	= new QHBoxLayout (buttons);
	layout->addWidget (buttons, row, col, 1, 3);
	hLayout->addStretch (200);
	button		= new QPushButton ("Annuler", buttons);
	button->setAutoDefault (false);
	button->setDefault (false);
	hLayout->addWidget (button);
	connect (button, SIGNAL (clicked ( )), this, SLOT (reject ( )));
	button	= new QPushButton ("Ouvrir", buttons);
	button->setAutoDefault (false);
	button->setDefault (false);
	connect (button, SIGNAL (clicked ( )), this, SLOT (accept ( )));
	hLayout->addWidget (button);

	mainLayout->activate ( );
}	// QtNetworkFileDialog::createGui


void QtNetworkFileDialog::setDirectory (const QString& dir)
{
	assert (0 != _directoryTextField);
	_directory	= 0 == dir.length ( ) ? QString ("/") : dir;
	_directoryTextField->setText (_directory);
}	// QtNetworkFileDialog::setDirectory


void QtNetworkFileDialog::updateWindowTitle ( )
{
	UTF8String				title (charset);
	QFileDialog::FileMode	mode	= fileMode ( );

	if ((QFileDialog::Directory == mode) || (QFileDialog::DirectoryOnly == mode))
		title << "Sélectionner un répertoire";
	else
	{
		title << (QFileDialog::AcceptOpen == acceptMode ( ) ?
		         "Ouvrir " : "Enregistrer ")
		      << (QFileDialog::ExistingFiles == fileMode ( ) ?
		         "un fichier" : "des fichiers");
	}

	setWindowTitle (UTF8TOQSTRING (title));
}	// QtNetworkFileDialog::updateWindowTitle


void QtNetworkFileDialog::itemClicked (QListWidgetItem*)
{
	assert (0 != _fileNameTextField);
	assert (0 != _fileSystemListView);
	QList<QListWidgetItem*>	selection	= _fileSystemListView->selectedItems( );
	UTF8String			list (Charset::UTF_8);
	_fileNameTextField->clear ( );
	for (size_t i = 0; i < selection.size ( ); i++)
	{
		QtInodItem*	inodItem	= dynamic_cast<QtInodItem*>(selection [i]);
		if (0 == inodItem)	// On est dans un callback => pas de throw
		{
			UTF8String	mess (charset);
			mess << "QtNetworkFileDialog::itemClicked (" << __FILE__ << ", "
			     << (unsigned long)__LINE__ << " Erreur : item de type imprévu ou nul.";
			ConsoleOutput::cerr ( ) << mess << co_endl;
			continue;
		}	// if (0 == inodItem)
		if (0 != list.length ( ))
			list << ' ';
		list << '"' << inodItem->getInod ( ).getFileName ( ) << '"';
	}	// for (size_t i = 0; i < selection.size ( ); i++)

	_fileNameTextField->setText (UTF8TOQSTRING(list));
}	// QtNetworkFileDialog::itemClicked


void QtNetworkFileDialog::itemDoubleClicked (QListWidgetItem* item)
{
	QtInodItem*	inodItem	= dynamic_cast<QtInodItem*>(item);
	if (0 == inodItem)	// On est dans un callback => pas de throw
	{
		UTF8String	mess (charset);
mess << "QtNetworkFileDialog::itemDoubleClicked (" << __FILE__ << ", "
     << (unsigned long)__LINE__ << " Erreur : item de type imprévu ou nul.";
		ConsoleOutput::cerr ( ) << mess << co_endl;
		return;
	}	// if (0 == inodItem)

	const RemoteFile	inod	= inodItem->getInod ( );
	if (true == inod.isDirectory ( ))
	{	// Répertoire : on s'y déplace :
		setDirectory (inod.getFullFileName ( ).c_str ( ));
		update ( );
	}
	else
		accept ( );	// Fichier : on ferme la boite de dialogue.
}	// QtNetworkFileDialog::itemDoubleClicked


QString QtNetworkFileDialog::getHostName ( ) const
{
	return (const QLineEdit*)0 == _hostTextField ? QString (NetworkData::getCurrentHostName ( ).c_str ( )) : _hostTextField->text ( );
}	// QtNetworkFileDialog::getHostName


bool QtNetworkFileDialog::allowHostNameModification ( ) const
{
	assert (0 != _hostTextField);
	return _hostTextField->isEnabled ( );
}	// QtNetworkFileDialog::allowHostNameModification


void QtNetworkFileDialog::allowHostNameModification (bool allow)
{
	assert (0 != _hostTextField);
	_hostTextField->setEnabled (allow);
}	// QtNetworkFileDialog::allowHostNameModification


void QtNetworkFileDialog::accept ( )
{
	QDialog::accept ( );
}	// QtNetworkFileDialog::accept


QStringList QtNetworkFileDialog::selectedFiles ( ) const
{
	assert (0 != _fileSystemListView);
	assert (0 != _fileNameTextField);
	QStringList		fileNames	= cottedListToList(_fileNameTextField->text( ));
	QStringList		files;
	for (size_t i = 0; i < fileNames.size ( ); i++)
	{
		UTF8String	url (Charset::UTF_8);
		url << getDirectory ( ).toStdString ( ) << "/" << QtUnicodeHelper::qstringToUTF8String (fileNames [i]);
		files.push_back (UTF8TOQSTRING (url));
	}	// for (size_t i = 0; i < fileNames.size ( ); i++)

	return files;
}	// QtNetworkFileDialog::selectedFiles


void QtNetworkFileDialog::setAcceptMode (QFileDialog::AcceptMode mode)
{
	if (mode != _acceptMode)
	{
		_acceptMode	= mode;
		updateWindowTitle ( );
	}	// if (mode != _acceptMode)
}	// QtNetworkFileDialog::setAcceptMode


void QtNetworkFileDialog::setFileMode (QFileDialog::FileMode fileMode)
{
	assert (0 != _fileSystemListView);
	assert (0 != _fileNameTextField);
	assert (0 != _fileNameLabel);

	_fileNameTextField->setEnabled (QFileDialog::AnyFile == fileMode ? true : false);
	if (fileMode != _fileMode)
	{
		QListWidget::SelectionMode	selectionMode	= QListWidget::SingleSelection;
		QString						label;
		_fileMode	= fileMode;
		switch (_fileMode)
		{
			case QFileDialog::AnyFile		:
			case QFileDialog::ExistingFile	:
			case QFileDialog::Directory		:
			case QFileDialog::DirectoryOnly	: selectionMode	= QListWidget::SingleSelection;		break;
			case QFileDialog::ExistingFiles	: selectionMode	= QListWidget::ExtendedSelection;	break;
		}	// switch (_fileMode)
		switch (_fileMode)
		{
			case QFileDialog::AnyFile		:
			case QFileDialog::ExistingFile	: label	= "Fichier :";	 				break;
			case QFileDialog::Directory		:
			case QFileDialog::DirectoryOnly	: label	= QSTR ("Répertoire :");	break;
			case QFileDialog::ExistingFiles	: label	= "Fichiers :";					break;
		}	// switch (_fileMode)

		_fileSystemListView->setSelectionMode (selectionMode);
		_fileNameLabel->setText (label);
		update ( );
	}	// if (fileMode != _fileMode)
}	// QtNetworkFileDialog::setFileMode


QStringList QtNetworkFileDialog::nameFilters ( ) const
{
	assert (0 != _filtersComboBox);
	QStringList	filters;
	for (size_t i = 0; i < _filtersComboBox->count ( ); i++)
		filters.push_back (_filtersComboBox->itemText (i));

	return filters;
}	// QtNetworkFileDialog::nameFilters


void QtNetworkFileDialog::setNameFilters (const QStringList& filters)
{
	assert (0 != _filtersComboBox);
	QtObjectSignalBlocker	blocker (_filtersComboBox);	// v 3.1.0
	_filtersComboBox->clear ( );
	for (size_t i = 0; i < filters.count ( ); i++)
		_filtersComboBox->addItem (filters [i]);

	update ( );
}	// QtNetworkFileDialog::setNameFilters


void QtNetworkFileDialog::selectNameFilter (const QString& name)
{
	assert (0 != _filtersComboBox);
	int	index	= _filtersComboBox->findText (name, Qt::MatchExactly);
	if (-1 != index)
	{
		_filtersComboBox->setCurrentIndex (index);
		_filter	= name;	// v 5.3.0 (correctif)
		update ( );
	}	// if (-1 != index)
}	// QtNetworkFileDialog::selectNameFilter


void QtNetworkFileDialog::update ( )
{
	if (true == _frozen)
		return;	// v 3.1.0

	assert (0 != _fileSystemListView);
	QtAutoWaitingCursor		cursor (true);
	_fileSystemListView->clear ( );

	try
	{
		const string	host	= getHostName ( ).toStdString ( );
		const string	dir		= getDirectory ( ).toStdString ( );
		unique_ptr<RemoteProcess>	ls (new RemoteProcess (host, "ls"));
		// "L" : met "-" ou "d" en cas de lien
		ls->getOptions ( ).addOption ("-lL");
		// v 2.25.0 : on force le format d'affichage de la date :
		// yyyy mm dd hh:mm Les séparateurs sont des tabulations.
		ls->getOptions ( ).addOption ("--time-style=+%Y%t%m%t%d%t%R");
		ls->getOptions ( ).addOption (dir);
		ls->enableChildToSonCommunications (true);
		ls->execute (false);

		// On transforme la liste de filtres en liste d'expressions
		// régulières :
		vector<QRegExp>		filterExpList;
		QStringList			filterList	= filterRuleToFilters (selectedNameFilter ( ));
		for (int i = 0; i < filterList.count ( ); i++)
		{
			string	filter	= purifyFilter (filterList [i].toStdString ( ));
			filterExpList.push_back(QRegExp(filter.c_str( ),Qt::CaseInsensitive,QRegExp::Wildcard));
		}	// for (int i = 0; i < filterList.count ( ); i++)

		// Lecture de la sortie de 'ls' :
		vector<RemoteFile>	files, directories;
		bool	done	= false;
		while (false == done)
		{
			string	line;
			try
			{	// Pour une raison indéterminée il arrive qu'une exception
				// soit levée en fin de flux, surtout sur le home.
				line	= ls->getChildLine ( );
			}
			catch (...)
			{
			}

			if ((0 == line.size ( )) && (true == ls->isCompleted ( )))
				done	= true;
			if ((0 == line.size ( )) || (line == " ") || (line == "\n"))
				continue;

//			char	rights [12], owner [128], group [128], name [128];
			char	rights [1025], owner [1025], group [1025], name [1025];	// v 5.5.5
			size_t	count = 0, size = 0, year = 0, month = 0, day = 0,
					hour = 0, min = 0;
			rights [0]  = owner [0] = group [0] = name [0]  = '\0';
			const QString	qLine	= line.c_str ( );
			const int		wordCount	= countWords (line);

			// Recherche
			// droits nb propr. groupe taille annee mois jour heure nom
			if (EOF == sscanf (line.c_str ( ),
			                   "%s%lu%s%s%lu%lu%lu%lu%lu:%lu%s",
			                   rights, &count, owner, group, &size, &year,
			                   &month, &day, &hour, &min, name))
				continue;

			if (string::npos != line.find ("No such file or directory"))	// v 5.5.5
			{
				UTF8String	error (charset);
				error << "Le répertoire " << dir << " n'existe pas.";
				QtMessageBox::displayErrorMessage (this, windowTitle ( ).toStdString ( ), error);
				//ConsoleOutput::cerr ( ) << message << co_endl;
			}	// if (9 != wordCount)
			if (wordCount > 9)
			{	// Nom de fichier/répertoire composé de plusieurs mots
				// Les séparateurs sont des espaces et des tabulations :
				istringstream	stream (line);
				string			str;
				char			buffer [1001];
				int				i	= 0;
				for (i = 0; i < 9; i++)
					stream >> str;
				stream.getline (buffer, 1000);
				UTF8String	us (buffer);
				str	= us.trim (true).iso ( );
				sprintf (name, "%s", str.c_str ( ));
			}	// if (wordCount > 9)
			if (0 == strlen (name))
				continue;
				
			// Comparaisons aux différents filtres. Si c'est un
			// répertoire on conserve également.
			for (vector<QRegExp>::iterator it = filterExpList.begin ( );
			     filterExpList.end ( ) != it; it++)
			{
				if ((-1 != (*it).indexIn (name)) || ('d' == *rights))
				{
					QDate		d (year, month, day);
					QTime		t (hour, min);
					QDateTime	dt (d, t);
//					int			permissions	= v 3.7.0
//										qtPermissions ((char*)(rights + 1));
					bool		isDir   = 'd' == *rights ?  true : false;
					bool		isLink	= 'l' == *rights ?  true : false;
					bool		isFile	= isDir == isLink ? true : false;
					UTF8String	path (Charset::UTF_8);
					path << _directory.toStdString ( ) << "/" << name;
					RemoteFile	file (path, isDir);
					if (true == isDir)
						directories.push_back (file);
					else
						files.push_back (file);
					break;
				}	// if ((-1 != (*it).indexIn (name)) || ('d' == *rights))
			}	// for (vector<QRegExp>::iterator it = filterExpList.begin ( );
		}	// while (false == done)

		Qt::ItemFlags	flags	= Qt::ItemIsEnabled;
		if ((QFileDialog::Directory == fileMode ( )) ||
		    (QFileDialog::DirectoryOnly == fileMode ( )))
			flags	|= Qt::ItemIsSelectable;
		for (vector<RemoteFile>::iterator itd = directories.begin ( );
		     directories.end ( ) != itd; itd++)
		{
			QtInodItem*		item	= new QtInodItem(*itd, _fileSystemListView);
			item->setFlags (flags);
		}
		if (QFileDialog::DirectoryOnly != fileMode ( ))
		{
			for (vector<RemoteFile>::iterator itf = files.begin ( );
			     files.end ( ) != itf; itf++)
			{
				flags	= Qt::ItemIsEnabled;
				if (QFileDialog::Directory != fileMode ( ))
					flags |= Qt::ItemIsSelectable;
				QtInodItem*	item	= new QtInodItem(*itf, _fileSystemListView);
				item->setFlags (flags);
			}
		}	// if ((QFileDialog::Directory != fileMode ( )) && ...
	}
	catch (...)
	{
	}
}	// QtNetworkFileDialog::update


void QtNetworkFileDialog::freeze (bool freeze)
{
	_frozen	= freeze;
	if (false == _frozen)
		update( );
}	// QtNetworkFileDialog::freeze


void QtNetworkFileDialog::homeClicked (bool)
{
	try
	{
		const size_t	maxTry	= 10;
		size_t	count	= 0;
		string	line, commandLine;
		while (maxTry >= ++count)
		{
			const string			host	= getHostName ( ).toStdString ( );
			unique_ptr<RemoteProcess>	home (new RemoteProcess(host, homeCommand));
			home->getOptions ( ).addOption (UserData ( ).getName ( ));
			home->enableChildToSonCommunications (true);
			commandLine	= home->getCommandLine ( );
			home->execute (false);
			line	= home->getChildLine ( );

			if ((false == line.empty ( )) && ('/' == line [0]))
				break;
		}	// while (maxTry > count)

		if ((true == line.empty ( )) || ('/' != line [0]))
		{
			UTF8String	error (charset);
			error << "Erreur lors de l'exécution de la commande ("
			      << (unsigned long)maxTry << " tentatives)"
			      << "\n" << commandLine << "\n" << line;
			throw Exception (error);
		}	// if ((true == line.empty ( )) || (('/' != line [0]))

		setDirectory (line.c_str ( ));
		update ( );
	}
	catch (const Exception& exc)
	{
		UTF8String	mess (charset);
		mess << "QtNetworkFileDialog::homeClicked. Exception : "
		     << exc.getFullMessage ( );
		ConsoleOutput::cerr ( ) << mess << co_endl;
	}
	catch (...)
	{
	}
}	// QtNetworkFileDialog::homeClicked


void QtNetworkFileDialog::preselectionChanged (int index)	// v 5.3.0
{
	assert (0 != _preselectionComboBox);
	try
	{
		if ((index >= 0) && (index < _preselectionComboBox->count ( )))
		{
			setDirectory (_preselectionComboBox->itemText (index));
			update ( );
		}    // if ((index >= 0) && (index < _preselectionComboBox->count ( )))
	}
	catch (...)
	{
	}
}	// QtNetworkFileDialog::preselectionChanged


void QtNetworkFileDialog::parentClicked (bool)
{
	try
	{
		File	directory (_directory.toStdString ( ));
		File	parent	= directory.getPath ( );
		if (parent.getFileName ( ) == ".")
			return;

		setDirectory (parent.getFullFileName ( ).c_str ( ));
		update ( );
	}
	catch (...)
	{
	}
}	// QtNetworkFileDialog::parentClicked


void QtNetworkFileDialog::directoryChanged ( )
{
	assert (0 != _directoryTextField);
	try
	{	
		setDirectory (_directoryTextField->text ( ));
		update ( );
	}
	catch (...)
	{
	}
}	// QtNetworkFileDialog::directoryChanged


void QtNetworkFileDialog::filtersChanged (int index)
{
	assert (0 != _filtersComboBox);
	if ((index >= 0) && (index < _filtersComboBox->count ( )))
	{
		_filter	= _filtersComboBox->itemText (index);
		update ( );
	}	// if ((index >= 0) && (index < _filtersComboBox->count ( )))
}	// QtNetworkFileDialog::filtersChanged


void QtNetworkFileDialog::fileNameEdited (const QString&)
{
	assert (0 != _fileNameTextField);
	assert (0 != _fileSystemListView);
	_fileSystemListView->clearSelection ( );
}	// QtNetworkFileDialog::fileNameEdited


void QtNetworkFileDialog::hostChanged ( )
{
	assert (0 != _hostTextField);
	update ( );
}	// QtNetworkFileDialog::hostChanged



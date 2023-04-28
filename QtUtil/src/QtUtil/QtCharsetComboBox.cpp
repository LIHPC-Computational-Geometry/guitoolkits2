#include "QtUtil/QtCharsetComboBox.h"
#include "QtUtil/QtUnicodeHelper.h"
#include <TkUtil/InternalError.h>
#include <TkUtil/MemoryError.h>

#include <QBoxLayout>
#include <QLabel>

#include <assert.h>

using namespace TkUtil;
using namespace std;

static const Charset    charset ("àéèùô");
USE_ENCODING_AUTODETECTION


QtCharsetComboBox::QtCharsetComboBox (
				QWidget* parent, Charset::CHARSET defaultCharset)
	: QComboBox (parent)
{
	for (size_t cs = (size_t)Charset::UNKNOWN; cs <= (size_t)Charset::UTF_16;
	     cs++)
	{
		Charset	c ((Charset::CHARSET)cs);
		addItem (c.name ( ).c_str ( ), QVariant ((uint)cs));
	}	// for (Charset::CHARSET cs = Charset::UNKNOWN; ...

	const int	idx	= findData ((uint)defaultCharset);
	setCurrentIndex (idx);
}	// QtCharsetComboBox::QtCharsetComboBox


QtCharsetComboBox::QtCharsetComboBox (
				QWidget* parent, const vector<Charset::CHARSET>& charsets,
				Charset::CHARSET defaultCharset)
	: QComboBox (parent)
{
	for (vector<Charset::CHARSET>::const_iterator it = charsets.begin ( );
	     charsets.end ( ) != it; it++)
	{
		Charset	c (*it);
		addItem (c.name ( ).c_str ( ), QVariant ((uint)*it));
	}	// for (Charset::CHARSET cs = Charset::UNKNOWN; ...

	const int	idx	= findData ((uint)defaultCharset);
	setCurrentIndex (idx);
}	// QtCharsetComboBox::QtCharsetComboBox


QtCharsetComboBox::QtCharsetComboBox (const QtCharsetComboBox&)
	: QComboBox (0)
{
	assert (0 && "QtCharsetComboBox copy constructor is not allowed.");
}	// QtCharsetComboBox::QtCharsetComboBox


QtCharsetComboBox& QtCharsetComboBox ::operator = (const QtCharsetComboBox&)
{
	assert (0 && "QtCharsetComboBox assignment operator is not allowed.");
	return *this;
}	// QtCharsetComboBox::QtCharsetComboBox


QtCharsetComboBox::~QtCharsetComboBox ( )
{
}	// QtCharsetComboBox::~QtCharsetComboBox


Charset::CHARSET QtCharsetComboBox::getCharset ( ) const
{
	const QVariant	data	= itemData (currentIndex ( ));
	return (Charset::CHARSET)data.toUInt ( );
}	// QtCharsetComboBox::getCharset


void QtCharsetComboBox::setCharset (Charset::CHARSET cset)
{
	const int	idx	= findData ((uint)cset);
	if (-1 == idx)
	{
		UTF8String	message (charset);
		message << "Jeu de caractères " << Charset (cset).name ( )
		        << " non proposé dans le sélecteur de jeux de caractères.";
		INTERNAL_ERROR (exc, message, "QtCharsetComboBox::setCharset")
		throw exc;
	}
	else
		setCurrentIndex (idx);
}	// QtCharsetComboBox::setCharset


QtLabelisedCharsetComboBox::QtLabelisedCharsetComboBox (
	QWidget* parent, const UTF8String& label, Charset::CHARSET defaultCharset)
	: QWidget (parent), _comboBox (0)
{
	QHBoxLayout*	layout	= new QHBoxLayout ( );
	setLayout (layout);
	QLabel*	l	= new QLabel (UTF8TOQSTRING (label), this);
	layout->addWidget (l);
	_comboBox	= new QtCharsetComboBox (this, defaultCharset);
	layout->addWidget (_comboBox);
	layout->addStretch (1000.);
}	// QtLabelisedCharsetComboBox::QtLabelisedCharsetComboBox


QtLabelisedCharsetComboBox::QtLabelisedCharsetComboBox (
	QWidget* parent, const UTF8String& label,
	const vector<Charset::CHARSET>& charsets, Charset::CHARSET defaultCharset)
	: QWidget (parent), _comboBox (0)
{
	QHBoxLayout*	layout	= new QHBoxLayout ( );
	setLayout (layout);
	QLabel*	l	= new QLabel (UTF8TOQSTRING (label), this);
	layout->addWidget (l);
	_comboBox	= new QtCharsetComboBox (this, charsets, defaultCharset);
	layout->addWidget (_comboBox);
	layout->addStretch (1000.);
}	// QtLabelisedCharsetComboBox::QtLabelisedCharsetComboBox


QtLabelisedCharsetComboBox::QtLabelisedCharsetComboBox (
											const QtLabelisedCharsetComboBox&)
	: QWidget (0), _comboBox (0)
{
	assert (0 && "QtLabelisedCharsetComboBox copy constructor is not allowed.");
}	// QtLabelisedCharsetComboBox::QtLabelisedCharsetComboBox


QtLabelisedCharsetComboBox& QtLabelisedCharsetComboBox::operator = (
											const QtLabelisedCharsetComboBox&)
{
	assert (0 && "QtLabelisedCharsetComboBox assignment operator is not allowed.");
	return *this;
}	// QtLabelisedCharsetComboBox::operator =


QtLabelisedCharsetComboBox::~QtLabelisedCharsetComboBox ( )
{
}	// QtLabelisedCharsetComboBox::~QtLabelisedCharsetComboBox


Charset::CHARSET QtLabelisedCharsetComboBox::getCharset ( ) const
{
	CHECK_NULL_PTR_ERROR (_comboBox)
	return _comboBox->getCharset ( );
}	// QtLabelisedCharsetComboBox::getCharset


void QtLabelisedCharsetComboBox::setCharset (Charset::CHARSET charset)
{
	CHECK_NULL_PTR_ERROR (_comboBox)
	_comboBox->setCharset (charset);
}	// QtLabelisedCharsetComboBox::setCharset


const QtCharsetComboBox& QtLabelisedCharsetComboBox::getComboBox ( ) const
{
	CHECK_NULL_PTR_ERROR (_comboBox)
	return *_comboBox;
}	// QtLabelisedCharsetComboBox::getComboBox


QtCharsetComboBox& QtLabelisedCharsetComboBox::getComboBox ( )
{
	CHECK_NULL_PTR_ERROR (_comboBox)
	return *_comboBox;
}	// QtLabelisedCharsetComboBox::getComboBox


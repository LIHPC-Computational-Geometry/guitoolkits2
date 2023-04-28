#include <QtUtil/QtConfiguration.h>
#include <TkUtil/Exception.h>


#ifndef QT_5
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#else	// QT_5
#include <QFrame>
#include <QLabel>
#include <QLayout>
#endif	// QT_5

#include <assert.h>
#include <values.h>


template <class T, class TF> QtMinMaxDialog<T, TF>::QtMinMaxDialog (
			QWidget* parent, const IN_STD string& appTitle,
			const IN_STD string& title,
			const QString& minLabel, const QString& maxLabel,
			T min, T max, bool modal,
			const IN_STD string& helpURL, const IN_STD string& helpTag)
	: QDialog (parent),
	  _minTextField (0), _maxTextField (0), _closurePanel (0),
	  _appTitle (appTitle)
{
	setModal (modal);
	setWindowTitle (QString (title.c_str ( )));

	QGridLayout*	layout	= new QGridLayout (this);
	layout->setMargin (QtConfiguration::margin);
	layout->setSizeConstraint (QLayout::SetMinimumSize);

	int	row	= 0, col	= 0;

	QSizePolicy	expand (QSizePolicy::Preferred, QSizePolicy::Preferred);
	QLabel*	label	= new QLabel (minLabel, this);
	layout->addWidget (label, row, col++);
	_minTextField	= new TF (min, this);
	layout->addWidget (_minTextField, row, col++);
	row++;	col = 0;
	label	= new QLabel (maxLabel, this);
	layout->addWidget (label, row, col++);
	_maxTextField	= new TF (max, this);
	layout->addWidget (_maxTextField, row, col++);
	row++;	col	= 0;

	QFrame*	separator	= new QFrame (this);
	separator->setFrameShadow (QFrame::Sunken);
	separator->setLineWidth (1);
	separator->setMidLineWidth (1);
	separator->setFrameShape (QFrame::HLine);
	layout->addWidget (separator, row, col, 1, 2);
	row++;	col	= 0;

	// La fermeture :
	_closurePanel	= new QtDlgClosurePanel (
							this, false, "OK", "", "Annuler", helpURL, helpTag);
	layout->addWidget (_closurePanel, row, col, 1, 2);
	_closurePanel->setMinimumSize (_closurePanel->sizeHint ( ));
	connect (_closurePanel->getApplyButton ( ), SIGNAL(clicked ( )), this,
	         SLOT(accept ( )));
	connect (_closurePanel->getCancelButton ( ), SIGNAL(clicked ( )), this,
	         SLOT(reject ( )));

	_closurePanel->getApplyButton ( )->setAutoDefault (false);
	_closurePanel->getApplyButton ( )->setDefault (false);
	_closurePanel->getCancelButton ( )->setAutoDefault (false);
	_closurePanel->getCancelButton ( )->setDefault (false);

	layout->activate ( );
	setMinimumSize (layout->sizeHint ( ));
}	// QtMinMaxDialog::QtMinMaxDialog


template <class T, class TF> QtMinMaxDialog<T, TF>::QtMinMaxDialog (
											const QtMinMaxDialog<T, TF>& view)
	: QDialog (0),
	  _minTextField (0), _maxTextField (0), _closurePanel (0), _appTitle ( )
{
	assert (0 && "QtMinMaxDialog copy constructor is not allowed.");
}	// QtMinMaxDialog::QtMinMaxDialog (const QtMinMaxDialog& view)


template <class T, class TF> QtMinMaxDialog<T, TF>&
		QtMinMaxDialog<T, TF>::operator = (const QtMinMaxDialog<T, TF>& view)
{
	assert (0 && "QtMinMaxDialog::operator = is not allowed.");
	return *this;
}	// QtMinMaxDialog::QtMinMaxDialog (const QtMinMaxDialog& view)


template <class T, class TF> QtMinMaxDialog<T, TF>::~QtMinMaxDialog ( )
{
}	// QtMinMaxDialog::~QtMinMaxDialog


template <class T, class TF> void QtMinMaxDialog <T, TF>::setMinRange (
																T min, T max)
{
	assert (0 != _minTextField);
	_minTextField->setRange (min, max);
}	// QtMinMaxDialog::setMinRange


template <class T, class TF> void QtMinMaxDialog <T, TF>::setMaxRange (
																T min, T max)
{
	assert (0 != _maxTextField);
	_maxTextField->setRange (min, max);
}	// QtMinMaxDialog::setMaxRange


template <class T, class TF> void QtMinMaxDialog <T, TF>::accept ( )
{
	const IN_UTIL Charset	charset ("àéèùô");
	bool	hasError	= false;
	IN_UTIL UTF8String	message ("Saisie des valeurs extrêmales : ", charset);
	try
	{
		T	min	= getMin ( );
		T	max	= getMax ( );

		if (min >= max)
			throw IN_UTIL Exception (IN_UTIL UTF8String ("La valeur minimum doit être strictement inférieure à la valeur maximum.", charset));

	}
	catch (const IN_UTIL Exception& exc)
	{
		message << exc.getFullMessage ( );
		hasError	= true;
	}
	catch (...)
	{
		message << "Erreur non documentée.";
		hasError	= true;
	}

	if (true == hasError)
		QtMessageBox::displayErrorMessage (this, _appTitle.c_str ( ), message);
	else
		QDialog::accept ( );
}	// QtMinMaxDialog::accept


template <class T, class TF> T QtMinMaxDialog<T, TF>::getMin ( ) const
{
	assert (0 != _minTextField);
	return _minTextField->getValue ( );
}	// QtMinMaxDialog::getMin


template <class T, class TF> T QtMinMaxDialog<T, TF>::getMax ( ) const
{
	assert (0 != _maxTextField);
	return _maxTextField->getValue ( );
}	// QtMinMaxDialog::getMax






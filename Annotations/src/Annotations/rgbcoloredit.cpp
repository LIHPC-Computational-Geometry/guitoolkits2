#include <QVariant>
#include <QLineEdit>
#include <QLabel>
#include <QToolButton>
#include <QLayout>
#include <QToolTip>
#include <QWhatsThis>
#include <QSpinBox>

#include "Annotations/rgbcoloredit.h"

/*
 *  Constructs a rgbcoloredit as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
rgbcoloredit::rgbcoloredit( QWidget* parent, const char* name, Qt::WindowFlags fl )
    : QWidget( parent, fl )
{
    setMinimumSize( QSize( 320, 25 ) );

    m_colorButton = new QToolButton( this );
    m_colorButton->setGeometry( QRect( 85, 1, 42, 20 ) );
    m_colorButton->setSizePolicy (
					QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    m_colorButton->setMinimumSize( QSize( 40, 20 ) );
    m_colorButton->setMaximumSize( QSize( 42, 20 ) );
	m_colorButton->setAutoFillBackground (true);
    m_colorButton->setPalette( QColor( 233, 233, 0 ) );
    QPalette pal;

    m_labelColorEdit = new QLabel( "m_labelColorEdit", this );
    m_labelColorEdit->setGeometry( QRect( 4, 1, 70, 21 ) );
    m_labelColorEdit->setAlignment(  Qt::AlignVCenter | Qt::AlignRight );

    QWidget* privateLayoutWidget = new QWidget (this);
//    privateLayoutWidget->setGeometry( QRect( 141, 1, 221, 23 ) );
    privateLayoutWidget->setGeometry( QRect( 141, 1, 221, 50 ) );	// v 1.8.0
    colorLineLayout = new QHBoxLayout (privateLayoutWidget); 

    labelRed = new QLabel( "labelRed", privateLayoutWidget );
    labelRed->setMaximumSize( QSize( 18, 20 ) );
    colorLineLayout->addWidget( labelRed );

    m_spinRed = new QSpinBox (privateLayoutWidget);
    m_spinRed->setMaximum( 255 );
    colorLineLayout->addWidget( m_spinRed );

    labelGreen = new QLabel( "labelGreen", privateLayoutWidget );
    labelGreen->setMaximumSize( QSize( 18, 20 ) );
    colorLineLayout->addWidget( labelGreen );

    m_spinGreen = new QSpinBox (privateLayoutWidget);
    m_spinGreen->setMaximum (255);
    colorLineLayout->addWidget( m_spinGreen );

    labelBlue = new QLabel( "labelBlue", privateLayoutWidget );
    labelBlue->setMaximumSize( QSize( 18, 20 ) );
    colorLineLayout->addWidget( labelBlue );

    m_spinBlue = new QSpinBox (privateLayoutWidget);
    m_spinBlue->setMaximum (255);
    colorLineLayout->addWidget( m_spinBlue );
    languageChange();
//    resize( QSize(369, 25).expandedTo(minimumSizeHint()) );
    resize( QSize(369, 50).expandedTo(minimumSizeHint()) );	// v 1.8.0
    
     // signals and slots connections
    connect( m_spinRed, SIGNAL( valueChanged(int) ), this, SLOT( redValueChanged(int) ) );
    connect( m_spinGreen, SIGNAL( valueChanged(int) ), this, SLOT( greenValueChanged(int) ) );
    connect( m_spinBlue, SIGNAL( valueChanged(int) ), this, SLOT( blueValueChanged(int) ) );
    connect( m_colorButton, SIGNAL( clicked() ), this, SLOT( edit_colors() ) );
    
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
rgbcoloredit::~rgbcoloredit()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void rgbcoloredit::languageChange()
{
     setWindowTitle ( tr( "ColorEditForm" ) );
    m_labelColorEdit->setText( tr( "label" ) );
    m_colorButton->setText( QString ( ) );
    labelRed->setText( tr( "R :" ) );
    labelGreen->setText( tr( "V :" ) );
    labelBlue->setText( tr( "B :" ) );
}

void rgbcoloredit::setColor( const QColor & color )
{
   m_spinRed->setValue( color.red());
   m_spinGreen->setValue( color.green());
   m_spinBlue->setValue( color.blue());
   m_color = color;
	m_colorButton->setAutoFillBackground (true);
    m_colorButton->setPalette (m_color);
   emit newColor(m_color);
}

QColor rgbcoloredit::getColor()
{
  return m_color;
}


void rgbcoloredit::init()
{
   m_red = 233;
   m_green =  233;
   m_blue = 0; 
   m_color = QColor(m_red,m_green,m_blue); 
	m_colorButton->setAutoFillBackground (true);
    m_colorButton->setPalette (m_color);
}

void rgbcoloredit::setLabel( const QString & s )
{
    m_labelColorEdit->setText(s);
}


void rgbcoloredit::destroy()
{

}

void rgbcoloredit::edit_colors()
{
   QColor c = m_color;
   c = QColorDialog::getColor(m_color, 0, QString ("Choix de la couleur"), QColorDialog::DontUseNativeDialog);
   if(c.isValid() )
   {
     setColor( c);   
   }
}

void rgbcoloredit::redValueChanged( int r )
{
   m_red = r; 
   QColor c( r,m_green, m_blue);    
   setColor(c);
}

void rgbcoloredit::greenValueChanged( int g)
{
	m_green = g;
   QColor c (m_red, g, m_blue);
   setColor(c);
}

void rgbcoloredit::blueValueChanged( int b)
{
   m_blue = b;
   QColor c (m_red, m_green, b);
   setColor(c);
}
	

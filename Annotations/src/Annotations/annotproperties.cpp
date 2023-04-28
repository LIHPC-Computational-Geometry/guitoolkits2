#include <QVariant>
#include <QColor>
#include <QPixmap>
#include <QString>
#include <QTextEdit>
#include <QCheckBox>
#include <QButtonGroup>
#include <QPushButton>
#include <QLayout>
#include <QToolTip>
#include <QWhatsThis>
#include <QComboBox>
#include <QGroupBox>

#include <vtkSystemIncludes.h>
#include <vtkCaptionRepresentation.h>
#include <vtkCaptionWidget.h>
#include <vtkCaptionActor2D.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkProperty2D.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

#include "Annotations/annotmgr.h"
#include "Annotations/annotproperties.h"
#include "Annotations/rgbcoloredit.h"
/*
 *  Constructs a AnnotProperties as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
AnnotProperties::AnnotProperties( QWidget* parent, const char* name, Qt::WindowFlags fl )
    : QWidget( parent, Qt::WindowStaysOnTopHint)
{
    setMinimumSize( QSize( 640, 210 ) );
    setMaximumSize( QSize( 641, 211 ) );
    setBaseSize( QSize( 41, 22 ) );

    //Boutons de positionnement de texte dans le caption
	QGroupBox*	alignGroupBox	= new QGroupBox ("", this);
	QWidget*	alignParent		= alignGroupBox;
    m_textalignGroup = new QButtonGroup (alignGroupBox);
	alignGroupBox->setGeometry (QRect(10, 185, 240, 20));
    m_textalignGroup->setExclusive (true);
    //!! L'ordre de creation insertion est important
    //!! il correspond a l'indice dans VTK
    m_textPosLeft = new QPushButton ("m_textPosLeft", alignParent);
    m_textPosLeft->setCheckable (true);
    m_textPosLeft->setChecked (true);
    m_textPosLeft->setGeometry( QRect( 0, 0, 80, 16 ) );
    m_textPosCenter = new QPushButton ("m_textPosCenter", alignParent);
    m_textPosCenter->setCheckable (true);
    m_textPosCenter->setGeometry( QRect( 80, 0, 80, 16 ) );
    m_textPosRight = new QPushButton ("m_textPosRight", alignParent);
    m_textPosRight->setCheckable (true);
    m_textPosRight->setGeometry( QRect( 160, 0, 80, 16 ) );
	m_textalignGroup->addButton (m_textPosLeft, 0);
	m_textalignGroup->addButton (m_textPosCenter, 1);
	m_textalignGroup->addButton (m_textPosRight, 2);

   //Combobox pour le choix des fontes
    m_fontChooser = new QComboBox (this);
    m_fontChooser->setGeometry( QRect( 15, 20, 100, 21 ) );
    m_fontChooser->setMaxCount( 5 );
    m_fontChooser->addItem (QString("Arial"), QVariant (VTK_ARIAL));
    m_fontChooser->addItem (QString("Courrier"), QVariant (VTK_COURIER));
    m_fontChooser->addItem (QString("Times"), QVariant (VTK_TIMES));

    //Checkbox pour l'affichage ou non du cadre du caption
    m_checkBorder = new QCheckBox (this);
    m_checkBorder->setGeometry( QRect( 128, 21, 73, 21 ) );
    m_checkBorder->setChecked( true );

    //Checkbox pour l'affichage ou non du pointeur du caption
    m_checkIndex = new QCheckBox (this);
    m_checkIndex->setGeometry( QRect( 197, 20, 73, 21 ) );
    m_checkIndex->setChecked( true );

    //Bouton d'application des modifs    
    m_applyPushbutton = new QPushButton ("m_applyPushbutton", this);
    m_applyPushbutton->setEnabled( false );
    m_applyPushbutton->setGeometry( QRect( 260, 170, 180, 30 ) );

    //Bouton de fermeture de la fenetre des proprietes  
    m_closePushbutton = new QPushButton ("m_closePushbutton", this);
    m_closePushbutton->setGeometry( QRect( 460, 170, 170, 30 ) );

    m_annottextEdit = new QTextEdit("m_annottextEdit", this);
    m_annottextEdit->setGeometry( QRect( 10, 50, 240, 133 ) );

    //Groupbox pour les couleurs
    m_colorsgroupBox = new QGroupBox (this);
    m_colorsgroupBox->setGeometry( QRect( 260, 42, 370, 120 ));
    
    QBoxLayout * l = new QVBoxLayout( m_colorsgroupBox );
      m_fgdColor = new rgbcoloredit(m_colorsgroupBox) ;
      //m_bkgdColor = new rgbcoloredit(m_colorsgroupBox); 
      m_boxColor = new rgbcoloredit(m_colorsgroupBox) ;
      l->addWidget(m_boxColor);
      l->addWidget(m_fgdColor);
      //l->addWidget(m_bkgdColor);
        
    languageChange();
//    resize( QSize(660, 210).expandedTo(minimumSizeHint()) );


    // signals and slots connections
    connect( m_closePushbutton, SIGNAL( clicked() ), this, SLOT( hide() ) );
    connect( m_annottextEdit, SIGNAL( textChanged() ), this, SLOT( texteditChanged() ) );
    connect( m_applyPushbutton, SIGNAL( clicked() ), this, SLOT( apply_slot() ) );

	connect (m_textalignGroup, SIGNAL (buttonClicked (int)), this, SLOT (textalignGroup_clicked (int)));
    connect( m_checkBorder, SIGNAL( stateChanged(int) ), this, SLOT( checkBorder_stateChanged(int) ) );
    connect( m_checkIndex, SIGNAL( stateChanged(int) ), this, SLOT( checkIndex_stateChanged(int) ) );
    connect( m_fontChooser, SIGNAL( activated(int) ), this, SLOT( font_activated(int) ) );
      
    connect( m_fgdColor, SIGNAL( newColor (QColor) ), this, SLOT( fgdColorChandeg(QColor) ) );
    connect( m_boxColor, SIGNAL( newColor (QColor) ), this, SLOT( boxColorChangerd(QColor) ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
AnnotProperties::~AnnotProperties()
{
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AnnotProperties::languageChange()
{
    setWindowTitle ( tr( "Propriétés" ) );
    m_checkBorder->setText( tr( "Cadre" ) );
    m_checkIndex->setText( tr( "Flèche" ) );
    m_textPosRight->setText( tr( "Droit" ) );
    m_textPosCenter->setText( tr( "Centre" ) );
    m_textPosLeft->setText( tr( "Gauche" ) );
    m_closePushbutton->setText( tr( "Fermer" ) );
    m_applyPushbutton->setText( tr( "Appliquer" ) );
    m_colorsgroupBox->setTitle( tr( "Couleurs" ) );
    m_fgdColor->setLabel(tr("Texte"));
    //m_bkgdColor->setLabel(tr("Fond"));
    m_boxColor->setLabel(tr("Bordure"));
}

void AnnotProperties::apply_slot()
{
    /* Mise a jour des donnees du caption par rapport  au widget QT*/
    updateData(true);
}


void AnnotProperties::init()
{    
   m_applyPushbutton->setEnabled (false);
   m_annottextEdit->clear();
   updateData(false);
}

//mise en coherence des donnees du panneau et du widget VTK 
void AnnotProperties::updateData( bool b )
{
	//retrouve l'annotation courante
    CaptionWidget * caption = AnnotMgr::get()->get_currentAnnot();
    if(caption){    
	vtkCaptionRepresentation*	captionRep	=
		dynamic_cast<vtkCaptionRepresentation*>(caption->GetRepresentation ( ));
      if(!b){ 
      	  //on met a jour le texte du widget par rapport a l'annot courante
          QString texte(  caption->GetCaptionActor2D()->GetCaption() );
          m_annottextEdit->setText(texte);
          m_applyPushbutton->setEnabled(false);
          //on met a jour la checkbox du pointeur
          m_checkIndex->setChecked( caption->GetCaptionActor2D()->GetLeader() );
          //on met a jour la checkbox du cadre
//          m_checkBorder->setChecked( caption->GetCaptionActor2D()->GetBorder() );
			if (0 != captionRep)
				m_checkBorder->setChecked (
						0 == captionRep->GetShowBorder ( ) ? false : true);
			else
				m_checkBorder->setChecked (false);
          //on met a jour le bouton de position du texte
          int allign = caption->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->GetJustification();
			switch (allign)
			{
				case	0	: m_textPosLeft->setChecked (true);		break;
				case	1	: m_textPosCenter->setChecked (true);	break;
				default		: m_textPosRight->setChecked (true);	break;
			}	// switch (allign)
          //on met a jour la combobox de la fonte
          int font = caption->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->GetFontFamily();
		int	index	= m_fontChooser->findData (QVariant (font));
		m_fontChooser->setCurrentIndex (index);
          
          //retrouve la couleur du texte de l'annot. courante
          double col[3];
          caption->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->GetColor(col);
          double frouge = col[0];
          double fvert = col[1];
          double fbleu = col[2];
          int rouge = frouge*255;
          int vert = fvert*255;
          int bleu = fbleu*255;
          QColor c1(rouge,vert,bleu);
          m_fgdColor->setColor(c1);
   
          //retrouve la couleur du cadre de l'annot. courante
          caption->GetCaptionActor2D()->GetProperty()->GetColor(col);
          frouge = col[0];
          fvert = col[1];
          fbleu = col[2];
          rouge = frouge*255;
          vert = fvert*255;
          bleu = fbleu*255;
          QColor c2(rouge,vert,bleu);
          m_boxColor->setColor(c2);
   
      }else{ 
      	//on met  a jour le texte de l'annotation
         QString texte (m_annottextEdit->toPlainText ( ));
          caption->GetCaptionActor2D ( )->SetCaption (
											texte.toStdString ( ).c_str ( ));
          m_applyPushbutton->setEnabled(false);
          caption->GetInteractor()->Render();
         //on met a jour le pointeur 
         caption->GetCaptionActor2D()->SetLeader( m_checkIndex->isChecked( ) );
          //on met a jour le cadre
//         caption->GetCaptionActor2D()->SetBorder( m_checkBorder->isChecked( ) );
if (0 != captionRep)
{
	if (true == m_checkBorder->isChecked ( ))
	{
		captionRep->SetShowBorderToOn ( );
		captionRep->GetBorderProperty ( )->SetLineWidth (3.);
	}
	else
	{
		captionRep->SetShowBorderToOff ( );
		captionRep->GetBorderProperty ( )->SetLineWidth (0.);
		caption->GetCaptionActor2D()->BorderOff ( );
	}
}

         //on met a jour la position du texte
         int allign = m_textalignGroup->checkedId ( );
         caption->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->SetJustification(allign); 
         //on met a jour la fonte du texte
		int		index	= m_fontChooser->currentIndex ( );	
		bool	ok		= true;
		int		font	= m_fontChooser->itemData (index).toInt (&ok);
         caption->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontFamily(font);
        
          //mettre a jour la couleur du texte de l'annot. courante
          double col[3];
          QColor c = m_fgdColor->getColor();
          double frouge = c.red();
          double fvert = c.green();
          double fbleu = c.blue();
          col[0] = frouge/255.;
          col[1] = fvert/255.;
          col[2] = fbleu/255.;
          caption->set_textColor(col);
   
          //mettre a jour la couleur du cadre de l'annot. courante
          c = m_boxColor->getColor();
          frouge = c.red();
          fvert = c.green();
          fbleu = c.blue();
          col[0] = frouge/255.;
          col[1] = fvert/255.;
          col[2] = fbleu/255.;
          caption->set_borderColor(col);

         caption->Render(); 	
      }
    }
    
}

void AnnotProperties::showEvent( QShowEvent* evt )
{
    /*Mise a jour du widget QT par rapport au caption */
   updateData(false);
}

void AnnotProperties::texteditChanged()
{
   /* Mise en fonction du bouton "Appliquer"  */
   m_applyPushbutton->setEnabled(true);
}


void AnnotProperties::displayBorder(bool b)
{
}

void AnnotProperties::displayIndex(bool b)
{
}

void AnnotProperties::textalignGroup_clicked( int ib)
{
   m_applyPushbutton->setEnabled(true);
}


void AnnotProperties::checkBorder_stateChanged( int )
{
   m_applyPushbutton->setEnabled(true);
}


void AnnotProperties::checkIndex_stateChanged( int )
{
   m_applyPushbutton->setEnabled(true);
}

void AnnotProperties::font_activated( int fId )
{
  CaptionWidget * caption = AnnotMgr::get()->get_currentAnnot();
  if(fId != caption->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->GetFontFamily())
   m_applyPushbutton->setEnabled(true);
}
 
void AnnotProperties::fgdColorChandeg(QColor c)
{
   m_applyPushbutton->setEnabled(true);
}

void AnnotProperties::boxColorChangerd(QColor c)
{
   m_applyPushbutton->setEnabled(true);
}

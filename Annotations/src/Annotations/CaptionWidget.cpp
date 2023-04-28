#include <vtkWidgetCallbackMapper.h>
#include <vtkCallbackCommand.h>
#include <vtkWidgetRepresentation.h>
#include <vtkBorderRepresentation.h>
#include <vtkCaptionRepresentation.h>
#include <vtkObjectFactory.h>
#include <vtkCaptionActor2D.h>
#include <vtkProperty2D.h>
#include <vtkCoordinate.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkHandleWidget.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkWidgetEvent.h>
#include <vtkRenderWindowInteractor.h>


#include "Annotations/annotmgr.h"
#include "Annotations/CaptionWidget.h"
#include "Annotations/CaptionRepresentation.h"

vtkStandardNewMacro(CaptionWidget);

CaptionWidget::CaptionWidget()
 :m_selected(false)
{
  // CaptionActor
  vtkCaptionActor2D *CaptionActor = vtkCaptionActor2D::New();
  CaptionActor->SetCaption("Texte");
#ifndef VTK_8
  CaptionActor->GetTextActor()->GetTextProperty()->SetJustificationToCentered();
  CaptionActor->GetTextActor()->GetTextProperty()->SetVerticalJustificationToCentered();
#else 	// VT_8
	CaptionActor->GetCaptionTextProperty ( )->SetJustificationToCentered ( );
	CaptionActor->GetCaptionTextProperty ( )->SetVerticalJustificationToCentered ( );
#endif	// VTK_8
  CaptionActor->VisibilityOn();

  //creer une representation
  CaptionRepresentation* rep = CaptionRepresentation::New();
  //associer rep<->widget
  this->SetRepresentation(rep);
  //associer acteur<->widget (associe acteur<->rep automatiquement)
  this->SetCaptionActor2D(CaptionActor);

  //relacher les "smart pointers" les associiations sont effectuees
  CaptionActor->Delete();
  rep->Delete();

  this->CallbackMapper->SetCallbackMethod(vtkCommand::RightButtonPressEvent,
                                          vtkCommand::RightButtonPressEvent,
                                          this, CaptionWidget::RightButtonPressed);                                        

  this->GetCaptionActor2D()->SetMaximumLeaderGlyphSize(10);
  this->GetCaptionActor2D()->ThreeDimensionalLeaderOff();   	
}

CaptionWidget::~CaptionWidget()
{
}
   	
void
CaptionWidget::RightButtonPressed(vtkAbstractWidget *w)
{  
  CaptionWidget *self = reinterpret_cast<CaptionWidget*>(w);
	
  if(self->GetRepresentation()->GetInteractionState() != vtkBorderRepresentation::Inside )
    {
      return;
    }
  
  self->EventCallbackCommand->SetAbortFlag(1);
  self->InvokeEvent(vtkCommand::RightButtonPressEvent);
}


void
CaptionWidget::set_selected(bool s)
{
  //aucun changement dans la selection
  if(s == m_selected) return ;
  m_selected = s;
  
  if(m_selected)
    {
       GetCaptionActor2D()->GetProperty()->SetLineStipplePattern(0xF18F);
       Render();
    }
   else
    {
    	GetCaptionActor2D()->GetProperty()->SetLineStipplePattern(0xFFFF );
    	Render();
    }
}

void
CaptionWidget::set_borderColor(double c[3])
{
	m_borderColor[0]=c[0];
	m_borderColor[1]=c[1];
	m_borderColor[2]=c[2];
	vtkCaptionRepresentation* rep = static_cast<vtkCaptionRepresentation*>(this->GetRepresentation());
	rep->GetBorderProperty()->SetColor(m_borderColor);// Bordure highlight
	GetCaptionActor2D()->GetProperty()->SetColor(m_borderColor);		// v 2.2.1
	Render ( );	// v 2.2.1
}

void 
CaptionWidget::get_borderColor(double* c)
{
	c[0] = m_borderColor[0];
	c[1] = m_borderColor[1];
	c[2] = m_borderColor[2];
}
void
CaptionWidget::set_textColor(double c[3])
{
	m_textColor[0]=c[0];
	m_textColor[1]=c[1];
	m_textColor[2]=c[2];
	GetCaptionActor2D()->GetCaptionTextProperty()->SetColor(m_textColor);
//	GetCaptionActor2D ( )->GetTextActor (
//				)->GetTextProperty ()->SetColor (m_textColor);	// V 1.8.0
	Render ( );	// v 2.4.0
}

void
CaptionWidget::get_textColor(double* c)
{
	c[0] = m_textColor[0];
	c[1] = m_textColor[1];
	c[2] = m_textColor[2];
}

//redefinition de la methode SelectRegion.
//Reagit au clic gauche sur le widget annotation
void  
CaptionWidget::SelectRegion(double pos[2])
{ 
  vtkRenderWindowInteractor* iren = GetInteractor();
  if((WidgetRep->GetInteractionState() == vtkBorderRepresentation::Outside) || !iren)
    return;
  if(!iren->GetShiftKey()){
    //On n'est pas en configuration selection multiple
    if(!m_selected)
      AnnotMgr::get()->addAnnotToSelection(this);// inserer l'annotation selectionnee dans la liste 
    // Double clic => repercute l'evenement aux observateurs 
    AnnotMgr::get()->deselectAll(this); 
    if(iren->GetRepeatCount()){
      AnnotMgr::get()->InvokeEvent(AnnotMgrI::AnnotDoubleClicked, this);
    } //(iren->GetRepeatCount())
  }else 
    AnnotMgr::get()->addAnnotToSelection(this);
    //(!iren->GetShiftKey())  
}



void  
CaptionWidget::Highlight(int highlightOn)
{}


#include <vtkObject.h>
#include <vtkCommand.h>  // for event defines
#ifdef VTK_9
#include <vtkCallbackCommand.h>
#endif	// VTK_9
#include <vtkObjectFactory.h>
#include <vtkCaptionActor2D.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#ifndef VTK_7	// VTK 8-9
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkRenderingFreeType)	// Pour les vtkTextRenderer utilisés par les vtkTextActor
#endif	// VTK_7
#include <Annotations/CaptionWidget.h>

#include <assert.h>
#include <algorithm>
#include <iostream>
using namespace std;

#include <vtkRenderWindow.h>
#include <vtkInteractorObserver.h>

#include "Annotations/annotmgr.h"

AnnotMgr* AnnotMgr::m_annotmgr=NULL;
double AnnotMgr::m_defaultTextColor[3]={1.,1.,1.};
double AnnotMgr::m_defaultBorderColor[3]={1.,1.,1.};

vtkStandardNewMacro(AnnotMgr);

AnnotMgr::AnnotMgr():
 m_leftButtonPressCallbackCommand(NULL)
 ,m_rightButtonPressCallbackCommand(NULL)
{
   AnnotMgr::m_annotmgr = this;
   m_currentAnnot = NULL;
}

void
AnnotMgr::Delete()
{
  vtkObject::Delete ( ); //CP
}

AnnotMgr::~AnnotMgr()
{
  // clean out connections
  AnnotVector::iterator iter;
  for(iter = get_annotVector().begin(); iter!=get_annotVector().end(); ++iter)
    {
       (*iter)->Delete();
    }
  for(iter = this->m_destroyedAnnots.begin(); iter != this->m_destroyedAnnots.end(); ++iter)
    {
       (*iter)->Delete();
    } 
   m_annotmgr=0; // CP 
   
//   this->m_rightButtonPressCallbackCommand->Delete();	// CP, bug 1.2
 //  this->m_leftButtonPressCallbackCommand->Delete();	// CP, bug 1.2
}


AnnotMgr*
AnnotMgr::get()
{
  return m_annotmgr;
}

CaptionWidget* 
AnnotMgr::get_currentAnnot()
{
	return m_currentAnnot;
}

void 
AnnotMgr::set_currentAnnot(CaptionWidget* cap)
{
  
  if(cap == m_currentAnnot) return;  
  
  m_currentAnnot = cap;
  if(m_currentAnnot == NULL)
     AnnotMgr::get()->InvokeEvent(AnnotMgrI::NoCurrentAnnot,NULL);
  else
    AnnotMgr::get()->InvokeEvent(AnnotMgrI::NewCurrentAnnot,NULL);
 }

/*
 * Service d'ajout d'une annotation a la scene
 */
void
AnnotMgr::annotate(vtkRenderer* rw)
{
  assert(rw);
  
  /*--- cree une nouvelle instance d'annotation ---*/
  //widget  
  CaptionWidget *Caption = CaptionWidget::New();
  Caption->SetInteractor(rw->GetRenderWindow()->GetInteractor());
  Caption->SetEnabled(1);
  
  /*--- ajout de l'instance dans la liste interne au manager ---*/
  AnnotMgr::get()->get_annotVector().push_back(Caption);

  Caption->set_borderColor(AnnotMgr::m_defaultBorderColor);
  Caption->set_textColor(AnnotMgr::m_defaultTextColor);

  //--- observe l'evenement de clic droit emis par une annotation ---
  AnnotMgr::get()->m_rightButtonPressCallbackCommand = vtkCallbackCommand::New();
  AnnotMgr::get()->m_rightButtonPressCallbackCommand->SetCallback(AnnotMgr::annotRightClickedCB );
  AnnotMgr::get()->m_rightButtonPressCallbackCommand->SetClientData(AnnotMgr::get());
  Caption->AddObserver(vtkCommand::RightButtonPressEvent, AnnotMgr::get()->m_rightButtonPressCallbackCommand, 1.0); 

  //--- observe l'evenement de clic gauche au niveau interacteur.
  //    Si le message n'est pas deja traite par une annotation, le manager le recevra
  //    afin de tout deselectionner ---
  if(!AnnotMgr::get()->m_leftButtonPressCallbackCommand){
     AnnotMgr::get()->m_leftButtonPressCallbackCommand = vtkCallbackCommand::New();
     AnnotMgr::get()->m_leftButtonPressCallbackCommand->SetCallback(AnnotMgr::OnLeftClickedCB );
     AnnotMgr::get()->m_leftButtonPressCallbackCommand->SetClientData(AnnotMgr::get());
     rw->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::LeftButtonPressEvent, 
                                                         AnnotMgr::get()->m_leftButtonPressCallbackCommand, .2); 
  }
  AnnotMgr::get()->set_currentAnnot(Caption);
 /*--- previent l'observateur de l'ajout d'une annotation */
  AnnotMgr::get()->InvokeEvent(AnnotMgrI::AnnotAdded,NULL);
}

/*
 * Service de destruction de l'annotation courante de la scene
 */
void
AnnotMgr::deleteCurrentAnnot()
{
  CaptionWidget* currentAnnot = AnnotMgr::get()->get_currentAnnot();
  if( currentAnnot == NULL ) return;
  
  AnnotVector::iterator pos;
  /*recherche l'annot courante dans la liste des annots*/
  pos = find(AnnotMgr::get()->get_annotVector().begin(), 
             AnnotMgr::get()->get_annotVector().end(), currentAnnot);
  if(pos != AnnotMgr::get()->get_annotVector().end())
    AnnotMgr::get()->get_annotVector().erase(pos);
    
  AnnotMgr::get()->removeAnnotFromSelection(currentAnnot);
   
  /*cache l'annotation */ 
  currentAnnot->Off();
  //currentAnnot->Delete();
 
  /*--- previent l'observateur de l'ajout d'une annotation */
  AnnotMgr::get()->InvokeEvent(AnnotMgrI::AnnotRemoved,NULL);
  AnnotMgr::get()->get_destroyedAnnotVector().push_back(currentAnnot);//pour destruction
  /* reinitialise le pointeur dans le manager */
  AnnotMgr::get()->set_currentAnnot(NULL);
  
  /*envoie le message derniere annotation detruite si c'est le cas*/
  if(AnnotMgr::get()->get_annotVector().size() == 0)
    AnnotMgr::get()->InvokeEvent(AnnotMgrI::LastAnnotRemoved, NULL);
}
 
/*
 * Service d'affichage/masquage des annotations 
 */
void
AnnotMgr::enableAnnotations(vtkRenderer* rw, bool view)
{
  assert(rw);
  AnnotVector::iterator iter = AnnotMgr::get()->get_annotVector().begin();
  for(; iter!=AnnotMgr::get()->get_annotVector().end();++iter)
    {
      (*iter)->SetEnabled(view);
    }
}

/*******************************************************************
 * Service callback active lors d'un clic droit sur une annotation
 */
void
AnnotMgr::annotRightClickedCB(vtkObject* caller, unsigned long event, void* clientdata, void* calldata)
{
  CaptionWidget *caption = reinterpret_cast<CaptionWidget*>(caller);
  vtkRenderWindowInteractor* iren = caption->GetInteractor();
 
  AnnotMgr::get()->set_currentAnnot(caption);
  AnnotMgr::get()->InvokeEvent(vtkCommand::RightButtonPressEvent, caller);
}

/*******************************************************************
 * Service callback active lors d'un clic gauche dans la scene
 */
void
AnnotMgr::OnLeftClickedCB(vtkObject* caller, unsigned long event, void* clientdata, void* calldata)
{
   AnnotMgr* mgr = reinterpret_cast<AnnotMgr*>(clientdata);
   mgr->deselectAll();
}

AnnotVector& 
AnnotMgr::get_selectedAnnotVector()
{
  return m_selectedAnnots;	
}

AnnotVector& 
AnnotMgr::get_annotVector()
{
  return m_annotations;	
}

AnnotVector& 
AnnotMgr::get_destroyedAnnotVector()
{
  return m_destroyedAnnots;	
}

void 
AnnotMgr::get_currentAnnotTextColor(double* col)
{
  CaptionWidget *caption =  AnnotMgr::get()->get_currentAnnot();
  caption->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->GetColor(col);
}

void 
AnnotMgr::get_currentAnnotBorderColor(double* col)
{
  CaptionWidget *caption =  AnnotMgr::get()->get_currentAnnot();
  caption->GetCaptionActor2D()->GetProperty()->GetColor(col);
}

void 
AnnotMgr::set_currentAnnotTextColor(double col[3])
{
  CaptionWidget *caption =  AnnotMgr::get()->get_currentAnnot();
  caption->set_textColor(col);
}

void 
AnnotMgr::set_currentAnnotBorderColor(double col[3])
{
  CaptionWidget *caption =  AnnotMgr::get()->get_currentAnnot();
  caption->set_borderColor(col);
}

//deselectionne toutes les annotations sauf cele en parametre
//si le parametre est NULL alors tout est deselectionne
void 
AnnotMgr::deselectAll(CaptionWidget* c)
{
  AnnotMgr::get()->set_currentAnnot(c);
  AnnotVector v = AnnotMgr::get()->get_selectedAnnotVector();
  AnnotVector::iterator iter = v.begin();
  for(; iter != v.end(); ++iter)
    if((*iter) != c)
       AnnotMgr::get()->removeAnnotFromSelection(*iter);
}

//donne les coordonnees d'attachement de l'annotation courante
//les coordonnees sont en World Coordinates
void 
AnnotMgr::set_currentAnnot_attachment(float p[3])
{
  CaptionWidget *caption =  AnnotMgr::get()->get_currentAnnot();
  caption->GetCaptionActor2D()->SetAttachmentPoint(p[0],p[1],p[2]); 
}

void
AnnotMgr:: set_defaultTextColor(double c[3])
{ 
  AnnotMgr::m_defaultTextColor[0]=c[0];
  AnnotMgr::m_defaultTextColor[1]=c[1];
  AnnotMgr::m_defaultTextColor[2]=c[2];
}

void
AnnotMgr:: set_defaultBorderColor(double c[3])
{
  AnnotMgr::m_defaultBorderColor[0]=c[0];
  AnnotMgr::m_defaultBorderColor[1]=c[1];
  AnnotMgr::m_defaultBorderColor[2]=c[2];
}

void
AnnotMgr:: get_defaultTextColor(double* c)
{ 
  c[0] = AnnotMgr::m_defaultTextColor[0];
  c[1] = AnnotMgr::m_defaultTextColor[1];
  c[2] = AnnotMgr::m_defaultTextColor[2];
}

void
AnnotMgr:: get_defaultBorderColor(double* c)
{ 
  c[0] = AnnotMgr::m_defaultBorderColor[0];
  c[1] = AnnotMgr::m_defaultBorderColor[1];
  c[1] = AnnotMgr::m_defaultBorderColor[2];
}

//Retrait d'une annotation de la liste de selection.
//Si la derniere annotation est retiree de la liste,
//alors on envoie un message "LastAnnotRemovedFromSelection"
void 
AnnotMgr::removeAnnotFromSelection(CaptionWidget* c)
{ 
 /*recherche l'annot courante dans la liste des annots selectionnees*/
  AnnotVector::iterator pos;
  pos = find(AnnotMgr::get()->get_selectedAnnotVector().begin(), 
             AnnotMgr::get()->get_selectedAnnotVector().end() , c); 
             
  if(pos != AnnotMgr::get()->get_selectedAnnotVector().end())
   {
     (*pos)->set_selected(false);
     if(AnnotMgr::get()->get_currentAnnot () == c)
       AnnotMgr::get()->set_currentAnnot(NULL);
     AnnotMgr::get()->get_selectedAnnotVector().erase(pos);
     //--- previent l'observateur du retraite d'une annotation de la liste de selection
     AnnotMgr::get()->InvokeEvent(AnnotMgrI::SomeAnnotDeSelected, NULL);
   }  
   
  if(AnnotMgr::get()->get_selectedAnnotVector().size() == 0)
   //--- previent l'observateur du retrait de la derniere annotation de la liste de selection 
    AnnotMgr::get()->InvokeEvent(AnnotMgrI::LastAnnotRemovedFromSelection, NULL);
}

//Ajout d'une annotation a la liste de selection,
//seulement si l'annotation n'est pas deja dans la liste.
//Dans tous les cas, l'annotation devient la courante.
void 
AnnotMgr::addAnnotToSelection(CaptionWidget* c)
{
 //recherche l'annot courante dans la liste des annots selectionnees
  AnnotVector::iterator pos;
  pos = find(AnnotMgr::get()->get_selectedAnnotVector().begin(), 
             AnnotMgr::get()->get_selectedAnnotVector().end() , c); 
             
  if(pos == AnnotMgr::get()->get_selectedAnnotVector().end())
      AnnotMgr::get()->get_selectedAnnotVector().push_back(c); 

   AnnotMgr::get()->set_currentAnnot(c);
   c->set_selected(true);
   //--- previent l'observateur de l'ajout d'une annotation a la liste de selection
   AnnotMgr::get()->InvokeEvent(AnnotMgrI::SomeAnnotSelected, NULL);
}

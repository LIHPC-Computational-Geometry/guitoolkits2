#ifndef ANNOTMGR_H
#define ANNOTMGR_H

#include <vtkRenderer.h>
#include <vtkCallbackCommand.h>
#include <vector>

#include <Annotations/CaptionWidget.h>

#include "Annotations/annotmgri.h"
/*!
 * \mainpage    Page principale de la bibliothèque <I>Annotations</I>
 *
 * \section presentation    Présentation
 * La bibliothèque <I>annotmgr</I> propose des composants pour annoter des
 * scènes <I>VTK</I> dans une application <I>Qt</I>.
 */

/**
  * Classe sigleton permettant de gerer les instances d'annotations.
  * 
  * @author saumande.
  * herite de l'interface de AnnotMgrI
  */
  
class AnnotVector : public std::vector< CaptionWidget* > {};

class AnnotMgr : public vtkObject
{
protected:
  /** Constructeur.
   * Le constructeur du manager est appele par la methode initialize()
   * @see AnnotMgr#initialize
   */ 
  AnnotMgr();
  
  /** destructeur
   * Le destructeur du manager est appele par la methode release()
   * @see AnnotMgr#release
  */
  virtual ~AnnotMgr();

  /*static void ProcessEvents(vtkObject* caller, unsigned long event,
                            void* clientdata, void* calldata);*/
                            
   vtkCallbackCommand* m_rightButtonPressCallbackCommand;
   vtkCallbackCommand* m_leftButtonPressCallbackCommand;
   
public:
  /*static void annotLeftClickedCB(vtkObject* caller, unsigned long event,
                            void* clientdata, void* calldata);*/
  static void OnLeftClickedCB(vtkObject* caller, unsigned long event,
                            void* clientdata, void* calldata);
  static void annotRightClickedCB(vtkObject* caller, unsigned long event,
                            void* clientdata, void* calldata);
 
  /** Initialisation de l'instance de manager.
      Cette methode est appelee automatiquement par get()
      @return Le pointeur sur l'instance de manager
      @see AnnotMgr#get
  */
  static AnnotMgr* New();
  vtkTypeMacro(AnnotMgr, vtkObject)

  /** destruction de l'instance de manager.
      Cette methode est appele le destructeur
  */
  virtual void Delete();
  
  /** Retrouve le manager.
      Si celui-ci n'existe pas il est instancie automatiquement
      par appel a la methode get().
      @return Le pointeur sur l'instance de manager
      @see AnnotMgr#initialize
  */
  static AnnotMgr* get();

  /**
   * Creation d'un module d'annotations
   * @param rw Fenetre de rendu associee a ce module
   * @param layer Couche a laquelle est associee le renderer
   */
  static void annotate(vtkRenderer* rw);
  static void set_currentAnnot_attachment(float p[3]);
  static void set_defaultTextColor(double c[3]);
  static void set_defaultBorderColor(double c[3]);
  static void get_defaultTextColor(double*);
  static void get_defaultBorderColor(double*);
  
  static void enableAnnotations(vtkRenderer* rw, bool view);
  static void deleteCurrentAnnot();
  static void get_currentAnnotTextColor(double* col);
  static void get_currentAnnotBorderColor(double* col);
  static void set_currentAnnotTextColor(double col[3]);
  static void set_currentAnnotBorderColor(double col[3]);
  static void deselectAll(CaptionWidget* c = NULL);
   
  enum AnnotPrivateEventIds{
    SomeAnnotMoved = AnnotMgrI::LastEvent, //emis lorsqu'une annotation est repositionnee
    QueryProperties
  };
  
   
  /**
   *  Ajoute une annotation au vecteur de stockage des annotations.
   */
  void addAnnotTolist(CaptionWidget* annot);

   /**
   * Initialise le pointeur sur l'annotation courante.
   */
  void set_currentAnnot(CaptionWidget* cap);
  
   /*
   * Retourne une reference sur le vecteur des annotations
   */
  AnnotVector& get_annotVector();
  /*
   * Retourne une reference sur le vecteur des annotations selectionnees
   */
  AnnotVector& get_selectedAnnotVector();
   /*
   * Retourne une reference sur le vecteur des annotations marquees pour destruction
   */
  AnnotVector& get_destroyedAnnotVector();
  /*
   * Retourne un pointeur sur l'annotation courante
   */
  CaptionWidget* get_currentAnnot();
  
  void deselectAnnot(CaptionWidget* annot);
  void removeAnnotFromSelection(CaptionWidget* c);
  void addAnnotToSelection(CaptionWidget* c); 
  
private:
  /*
   * pointeur sur l'annotation courante
   */
  CaptionWidget* m_currentAnnot;

  /*
   *  Vecteur de stockage des annotations selectionnees
   */
  AnnotVector m_selectedAnnots;

  /*
   *  Vecteur de stockage des annotations
   */
  AnnotVector m_annotations;

  /*
   *  Vecteur de stockage des annotations detruites
   */
  AnnotVector m_destroyedAnnots;

  /**
   *Pointeur de classe.
   */
  static AnnotMgr* m_annotmgr;
  
  /*
   * pointeur sur la fonction par defaut de traitement des messages
   */
   
  static double m_defaultBorderColor[3];
  static double m_defaultTextColor[3];
  
};

#endif

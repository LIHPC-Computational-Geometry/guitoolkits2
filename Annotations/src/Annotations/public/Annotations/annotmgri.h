#ifndef ANNOTMGRI_H
#define ANNOTMGRI_H

#include <vtkCommand.h>
#include <vtkRenderer.h>


/*
 * Classe interface positionnee entre l'application et le manager d'annotations
 *@author saumande
 */

class AnnotMgrI {

public:

  /** Initialisation de l'instance de manager.
   *   @return booleen initialis&eacute; a false si echec.
   */
  static bool initialize();

  /** Destruction de l'instance de manager.
  */
  static void release();

 /*********************************************************/
 /*  services de requ&ecirc;te au manager d'annotations   */
 /*********************************************************/
 
 /** 
  * Service d'annotation d'une vue vtk.
  * @param rw Fenetre de rendu (vue vtk) associee.
  */
  static void annotate (vtkRenderer* rw );
 
 /** 
  * Service de positionnement de l'extremite de l'index de l'annotation courante.
  * @param p Trois flottants representant les composantes x,y,z en World Coordinates
  */
  static void set_currentAnnot_attachment(float p[3]);
 
 /**
  * Couleur par defaut du texte des annotations a la creation.
  * @param c Trois flottants representant les composantes rouge vert et bleu [0.0-1.0]
  */
  static void set_defaultTextColor(double c[3]);

 /**
  * Couleur par defaut de la bordure des annotations a la creation.
  * @param c Trois flottants representant les composantes rouge vert et bleu [0.0-1.0]
  */
  static void set_defaultBorderColor(double c[3]);
   
 /**
  * Effacement de l'annotation courante 
  */
  static void deleteCurrentAnnot();
  
  /**
   * Deselectionne toutes les annotations de la scene
   * */
  static void deselectAll();
  
  /**
   * Retrouve les composantes rougre,vert,bleu du texte de l'annotation courante
   */
  static void get_currentAnnotTextColor(double* col);
 
  /**
   * Initialise les composantes rougre,vert,bleu du texte de l'annotation courante
   */
  static void set_currentAnnotTextColor(double col[3]);
 
  /**
   * Retrouve les composantes rougre,vert,bleu de la bordure de l'annotation courante
   */
  static void get_currentAnnotBorderColor(double* col);
 
  /**
   * Initialise les composantes rougre,vert,bleu de la bordure de l'annotation courante
   */
  static void set_currentAnnotBorderColor(double col[3]);
 
  /** Service d'affichage/masquage des annotations associ&eacute;es a la vue vtk.
   * @param rw Fenetre de rendu (vue vtk) associee.
   */
  static void enableAnnotations( vtkRenderer* rw, bool view=true);


 /** Service d'ajout de callback d'observation 
   * @param evt identifiant de l'evenement.
   * @param cmd pointeur sur la fonction callback activee par l'evenement.
 */
 static void AddObserver(unsigned long evt, vtkCommand* cmd, float priority);

 enum AnnotEventIds{
   AnnotAdded = vtkCommand::UserEvent,//emis lorsque une annotation a ete creee (elle devient la courante)
   AnnotRemoved, //emis lorsqu'une annotation est detruite
   LastAnnotRemoved, //emis lorsque la derniere annotation a ete detruite
   AnnotDoubleClicked,
   SomeAnnotSelected , //emis lorsqu'une annotation est selectionnee
   SomeAnnotDeSelected,//emis lorsqu'une annotation a ete deselectionnee
   LastAnnotRemovedFromSelection, //emis lorsque toutes les annotations ont ete deselectionnees
   NoCurrentAnnot, //emis lorsque il n'existe plus aucune annotation courante
   NewCurrentAnnot, //emis lorsqu'une annotation devient l'annotation courante
   LastEvent
  };                      

 
protected:
  AnnotMgrI();
  virtual ~AnnotMgrI();
  AnnotMgrI& operator = (const AnnotMgrI& i);
};

#endif

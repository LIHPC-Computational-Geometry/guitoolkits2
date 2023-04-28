#ifndef CAPTIONWIDGET_H_
#define CAPTIONWIDGET_H_

#include <vtkCommand.h>
#include <vtkObject.h>
#include <vtkCallbackCommand.h>
#include <vtkCaptionWidget.h>

class CaptionWidget : public vtkCaptionWidget
{
  bool m_selected;
  double m_borderColor[3];
  double m_textColor[3];
  
public:
  virtual const char *GetClassName() const {return "CaptionWidget";};
  static CaptionWidget* New();
  
  /**
   * Service de marquage de l'annotation comme selectionnee/non selectionnee 
   */
  virtual void set_selected(bool s);
  
  /**
   * attribution d'une couleur a la bordure de l'annotation 
   */
  virtual void set_borderColor(double c[3]);
  
  /**
   * retourne la couleur de la bordure de l'annotation 
   */
  virtual void get_borderColor(double* c);
  
  
  /**
   * attribution d'une couleur au texte de l'annotation 
   */
  virtual void set_textColor(double c[3]);

  /**
   * retourne la couleur du texte de l'annotation 
   */
  virtual void get_textColor(double* c);
  
  virtual void Highlight(int highlightOn); 
  
protected:
	CaptionWidget();
	virtual ~CaptionWidget();
	
    virtual void SelectRegion(double pos[2]);    
    
    static void RightButtonPressed(vtkAbstractWidget *w);	
};

#endif /*CAPTIONWIDGET_H_*/

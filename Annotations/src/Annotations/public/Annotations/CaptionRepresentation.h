#ifndef CAPTIONREPRESENTATION_H_
#define CAPTIONREPRESENTATION_H_

#include <vtkCaptionRepresentation.h>

class CaptionRepresentation : public vtkCaptionRepresentation
{
  
public:
  virtual const char *GetClassName() const {return "CaptionRepresentation";};
  static CaptionRepresentation* New();

  virtual void MovingOn() ;
  virtual void MovingOff();    

protected:
	CaptionRepresentation();
	virtual ~CaptionRepresentation();

private:
  CaptionRepresentation(const vtkCaptionRepresentation&);
  void operator=(const CaptionRepresentation&); 

};

#endif /*CAPTIONREPRESENTATION_H_*/

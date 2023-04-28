#ifndef CAPTIONREPRESENTATION_CPP_
#define CAPTIONREPRESENTATION_CPP_

#include <vtkCaptionActor2D.h>
#include <vtkObjectFactory.h>
#include <vtkProperty2D.h>

#include "Annotations/CaptionRepresentation.h"

vtkStandardNewMacro(CaptionRepresentation);

CaptionRepresentation::CaptionRepresentation()
{
//  SetShowBorderToActive();
  SetShowBorderToOn();
  GetBorderProperty()->SetLineWidth(3);
}

CaptionRepresentation::~CaptionRepresentation()
{}

void
CaptionRepresentation::MovingOn()
{
  Moving = 1;
}

void 
CaptionRepresentation::MovingOff() 
{
  Moving = 0;
}    

#endif /*CAPTIONREPRESENTATION_CPP_*/

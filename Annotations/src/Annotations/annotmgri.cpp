#include "Annotations/annotmgri.h"
#include "Annotations/annotmgr.h"

static bool initialized = false;

AnnotMgrI::AnnotMgrI(){}

AnnotMgrI::~AnnotMgrI(){}

bool
AnnotMgrI::initialize()
{
  initialized = AnnotMgr::New()!=NULL;
  return  initialized;
}

void
AnnotMgrI::release()
{
  AnnotMgr::get()->Delete();
  initialized = false;
}

void
AnnotMgrI::annotate(vtkRenderer* rw)
{
  AnnotMgr::annotate(rw);
}

void
AnnotMgrI::deleteCurrentAnnot()
{
  AnnotMgr::deleteCurrentAnnot();
}

void
AnnotMgrI::enableAnnotations(vtkRenderer* rw, bool view)
{
  AnnotMgr::enableAnnotations(rw,view); 
}


void
AnnotMgrI::AddObserver( unsigned long evt, vtkCommand* cmd, float priority)
{
  AnnotMgr::get()->AddObserver( evt, cmd,  priority);
}

void 
AnnotMgrI::get_currentAnnotTextColor(double *col)
{
  AnnotMgr::get_currentAnnotTextColor( col );
}

void 
AnnotMgrI::get_currentAnnotBorderColor(double *col)
{
  AnnotMgr::get_currentAnnotBorderColor( col );
}

void 
AnnotMgrI::set_currentAnnotTextColor(double col[3])
{
  AnnotMgr::set_currentAnnotTextColor( col );
}

void 
AnnotMgrI::set_currentAnnotBorderColor(double col[3])
{
  AnnotMgr::set_currentAnnotBorderColor( col );
}

void 
AnnotMgrI::deselectAll()
{
  AnnotMgr::deselectAll();
}

void
AnnotMgrI::set_currentAnnot_attachment(float p[3])
{
  AnnotMgr::set_currentAnnot_attachment(p);
}

void
AnnotMgrI::set_defaultTextColor(double c[3])
{ 
  AnnotMgr::set_defaultTextColor(c);
}

void
AnnotMgrI::set_defaultBorderColor(double c[3])
{ 
  AnnotMgr::set_defaultBorderColor(c);
}




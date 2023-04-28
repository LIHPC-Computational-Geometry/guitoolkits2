#include <QApplication>
#ifndef VTK_7
#include <QSurfaceFormat>
#ifdef VTK_8
#include <QVTKOpenGLWidget.h>
#else
#include <QVTKOpenGLNativeWidget.h>	// => VTK_9
#endif	// VTK_8
#include <vtkAutoInit.h>
#endif	// VTK_7

#include "Annotations/MainWidget.h"

#include "Annotations/annotmgri.h"

int main(int argc, char** argv)
{
#ifndef VTK_7
	// set surface format before application initialization
#ifdef VTK_8
	QSurfaceFormat::setDefaultFormat( QVTKOpenGLWidget::defaultFormat());
#else // => VTK_9
	QSurfaceFormat::setDefaultFormat( QVTKOpenGLNativeWidget::defaultFormat());
#endif	// VTK_8
#endif	// VTK_7
  double txtCol[3] = {0.,1.,0.};
  double brdCol[3] = {1.,0.,1.};
  AnnotMgrI::initialize(); // a initialiser en tout premier
  AnnotMgrI::set_defaultTextColor(txtCol);
  AnnotMgrI::set_defaultBorderColor(brdCol);
  
  QApplication app(argc, argv);

  MainWidget widget;
  
  
  widget.show();
  
//  return app.exec();
	int	retCode	= app.exec ( );
  
  AnnotMgrI::release();

	return retCode;
}



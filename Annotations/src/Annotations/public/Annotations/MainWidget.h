#ifndef MAINWIDGET_H
#define MAINWIDGET_H


#include <QVariant>
#include <QPixmap>
#include <QMainWindow>

#include "Annotations/annotproperties.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QAction;
class QActionGroup;
class QToolBar;
class QMenu;

#include <QtVtk/QtVtkGraphicWidget.h>


class QButtonGroup;
class QRadioButton;
class QLabel;
class vtkRenderer;
class vtkEventQtSlotConnect;
class vtkObject;
class vtkCommand;

class CaptionWidget;

class MainWidget : public QMainWindow
{
    Q_OBJECT

public:
    MainWidget( QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::Window );
    ~MainWidget();

    QtVtkGraphicWidget*	qVTK;

    QLabel* coord;
    QMenuBar *MenuBar;
    QAction* fileExitAction;
    QAction* annotateAction;
    QAction* annotateAnnotateAction;
    QAction* annotateHideAction;
    QAction* annotateShowAction;
    QAction* deleteAnnotAction;
    QAction* showAnnotPropsAction;
    QAction* annotateDeleteAction;
    QAction* boxColorAction;
    QAction* textColorAction;
    QAction* colorAction;
    
    static void annotRightClickedCB( vtkObject * caller, unsigned long eid, void * clientdata, void * calldata );
    static void annotDoubleClickedCB( vtkObject * caller, unsigned long eid, void * clientdata, void * calldata );
//    static void annotLeftClickedCB( vtkObject * caller, unsigned long event, void * clientdata, void * calldata );
    static void annotAddedCB( vtkObject * caller, unsigned long eid, void * clientdata, void * calldata );
    static void lastAnnotDeletedCB( vtkObject * caller, unsigned long eid, void * clientdata, void * calldata );
    static void noAnnotSelectedCB(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata);
    static void noCurrentAnnotCB(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata);
    static void newCurrentAnnotCB(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata);
    
public slots:
    virtual void fileExit();
    virtual void updateCoords( vtkObject * obj );
    virtual void annotate();
    virtual void hideAnnotations();
    virtual void showAnnotations();
    virtual void activateAnnotPopup( QPoint pt );
    virtual void showAnnotProps();
    virtual void hideAnnotProps();
    virtual void deleteAnnot();
    virtual void annotTextColor();
    virtual void annotBorderColor();

signals:
    void annotAdded();

protected:
    vtkEventQtSlotConnect* m_connections;
    vtkRenderer* m_renderer ;
    QMenu*		m_annotPopup;
    AnnotProperties* m_annotPropsDialog;

    QVBoxLayout* MainWidgetLayout;
    QHBoxLayout* layout2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

    void init();
    void destroy();

};

#endif // MAINWIDGET_H

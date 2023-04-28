#include <QVariant>
#include <QColor>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QLabel>
#include <QLayout>
#include <QToolTip>
#include <QWhatsThis>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QColorDialog>


#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkEventQtSlotConnect.h>

#ifndef VTK_WIDGET
#include <vtkGenericOpenGLRenderWindow.h>
#endif  // VTK_WIDGET

#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCaptionActor2D.h>
#include <vtkCaptionWidget.h>
#include <vtkCallbackCommand.h>
#include <vtkSphereSource.h>

#include "Annotations/annotproperties.h"
#include "Annotations/annotmgri.h"

#include "Annotations/MainWidget.h"


static QAction* createAction (QObject* parent, const QString& label)
{
	QAction*	action	= 0;

	action	= new QAction (label, parent);

	return action;
}	// createAction


/*
 *  Constructs a MainWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 */
MainWidget::MainWidget( QWidget* parent, const char* name, Qt::WindowFlags fl )
    : QMainWindow( parent, fl )
{
    (void)statusBar();
    setCentralWidget( new QWidget (this));
    MainWidgetLayout = new QVBoxLayout (centralWidget ( ));

    layout2		= new QHBoxLayout (0);
    
    qVTK		= new QtVtkGraphicWidget (centralWidget ( ));

    layout2->addWidget( qVTK );
    MainWidgetLayout->addLayout( layout2 );

	coord = new QLabel("coord", centralWidget ( ));
    coord->setAlignment (Qt::AlignCenter);
    coord->setFixedHeight (coord->sizeHint ( ).height ( ));
    MainWidgetLayout->addWidget( coord );

    // actions
    fileExitAction = createAction ( this, "fileExitAction" );
    annotateAction = createAction ( this, "annotateAction" );
    annotateAnnotateAction = createAction ( this, "annotateAnnotateAction" );
    annotateAnnotateAction->setCheckable (false);
    annotateAnnotateAction->setChecked (false);
    annotateAnnotateAction->setEnabled( true );
    annotateHideAction = createAction ( this, "annotateHideAction" );
    annotateHideAction->setCheckable (false);
    annotateHideAction->setChecked (false);
    annotateHideAction->setEnabled( false );
    annotateShowAction = createAction ( this, "annotateShowAction" );
    annotateShowAction->setEnabled( false );
    deleteAnnotAction = createAction ( this, "deleteAnnotAction" );
    showAnnotPropsAction = createAction ( this, "showAnnotPropsAction" );
    annotateDeleteAction = createAction ( this, "annotateDeleteAction" );
    annotateDeleteAction->setEnabled( false );

    boxColorAction = createAction ( this, "boxColorAction" );
    textColorAction = createAction ( this, "textColorAction" );
    colorAction = createAction ( this, "colorAction" );

    // toolbars


    // menubar
    MenuBar	= new QMenuBar (this);


    QMenu*	fileMenu		= menuBar ( )->addMenu ("Fichier");
	QMenu*	annotateMenu	= fileMenu->addMenu ("Annotations ...");
	annotateAnnotateAction	= annotateMenu->addAction (QString ("Annoter la scène"));
	fileMenu->addSeparator ( );
	annotateHideAction		= fileMenu->addAction ("Masquer");
	annotateShowAction		= fileMenu->addAction ("Afficher");
	annotateDeleteAction	= fileMenu->addAction (QString ("Détruire"));
	fileExitAction			= fileMenu->addAction ("Quitter");

    languageChange();
    resize( QSize(589, 402).expandedTo(minimumSizeHint()) );

    // signals and slots connections
    connect( fileExitAction, SIGNAL( triggered(bool) ), this, SLOT( close() ) );
    connect( annotateAnnotateAction, SIGNAL( triggered(bool) ), this, SLOT( annotate() ) );
    connect( annotateHideAction, SIGNAL( triggered(bool) ), this, SLOT( hideAnnotations() ) );
    connect( annotateShowAction, SIGNAL( triggered(bool) ), this, SLOT( showAnnotations() ) );
    connect( annotateDeleteAction, SIGNAL( triggered(bool) ), this, SLOT( deleteAnnot() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
MainWidget::~MainWidget()
{
    destroy();
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void MainWidget::languageChange()
{
    setWindowTitle( tr( "Annotation" ) );
    showAnnotPropsAction->setText( tr( "Propriétés" ) );
	deleteAnnotAction->setText( tr( "Détruire" ) );
	annotateShowAction->setText( tr( "Afficher" ) );
    annotateDeleteAction->setText( tr( "Détruire" ) );
    boxColorAction->setText( tr( "Cadre..." ) );
    textColorAction->setText( tr( "Texte..." ) );
    colorAction->setText( tr( "Couleurs" ) );
}



void MainWidget::init()
{
  // create a window to make it stereo capable and give it to QVTKWidget
#ifdef VTK_WIDGET
  vtkRenderWindow* renwin = vtkRenderWindow::New();
#else 	// VTK_WIDGET
	vtkGenericOpenGLRenderWindow* renwin = vtkGenericOpenGLRenderWindow::New ( );	// VTK 8-9
#endif	// VTK_WIDGET
  renwin->StereoCapableWindowOn();
  qVTK->SetRenderWindow(renwin);
  renwin->Delete();

  // add a renderer
  m_renderer = vtkRenderer::New();
  qVTK->GetRenderWindow()->AddRenderer(m_renderer);

  // put cone in one window
  vtkConeSource* cone = vtkConeSource::New();
  vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
#ifdef VTK_5
  mapper->SetInput(cone->GetOutput());
#else	// VTK_5
  mapper->SetInputConnection (cone->GetOutputPort ( ));
#endif	// VTK_5
  vtkActor* actor = vtkActor::New();
  actor->SetMapper(mapper);
  m_renderer->AddViewProp(actor);
  actor->Delete();
  mapper->Delete();
  cone->Delete();

  vtkSphereSource* cone2 = vtkSphereSource::New();
  vtkPolyDataMapper* mapper2 = vtkPolyDataMapper::New();
#ifdef VTK_5
  mapper2->SetInput(cone2->GetOutput());
#else	// VTK_5
  mapper2->SetInputConnection (cone2->GetOutputPort ( ));
#endif	// VTK_5
  vtkActor* actor2 = vtkActor::New();
  actor2->SetMapper(mapper2);
  m_renderer->AddViewProp(actor2);
  actor2->Delete();
  mapper2->Delete();
  cone2->Delete();
  
  /* Creation du menu affiche au clic droit sur une annotation */ 
	m_annotPopup		= new QMenu(qVTK);
	QMenu*	colorPopup	= m_annotPopup->addMenu ("Couleurs ...");
	colorPopup->addAction (boxColorAction);
	colorPopup->addAction (textColorAction);
	m_annotPopup->addSeparator ( );
	m_annotPopup->addAction (showAnnotPropsAction);
	m_annotPopup->addAction (deleteAnnotAction);
 
   connect( showAnnotPropsAction, SIGNAL( triggered(bool) ), this, SLOT( showAnnotProps() ) );
   connect( deleteAnnotAction, SIGNAL( triggered(bool) ), this, SLOT( deleteAnnot() ) );
   connect( boxColorAction, SIGNAL( triggered(bool) ), this, SLOT( annotBorderColor() ) );
   connect( textColorAction, SIGNAL( triggered(bool) ), this, SLOT(annotTextColor() ) );

  /* Creation du widget d'affichage des proprietes d'une annotation */    
   m_annotPropsDialog = new AnnotProperties;
   
      
   m_connections = vtkEventQtSlotConnect::New();
   
  // update coords as we move through the window
  m_connections->Connect(qVTK->GetRenderWindow()->GetInteractor(),
                       vtkCommand::MouseMoveEvent,
                       this,
                       SLOT(updateCoords(vtkObject*)));
  
  //m_connections->PrintSelf(cout, vtkIndent());
  
 /* Creation des callbacks pour les differents evenements du manager */ 
  vtkCallbackCommand* callbackCommand;
  
  callbackCommand = vtkCallbackCommand::New();
  callbackCommand->SetCallback(annotRightClickedCB );
  callbackCommand->SetClientData(this);
  AnnotMgrI::AddObserver(vtkCommand::RightButtonPressEvent,callbackCommand,1.0);
  
  callbackCommand = vtkCallbackCommand::New();
  callbackCommand->SetCallback(annotDoubleClickedCB );
  callbackCommand->SetClientData(this);
  AnnotMgrI::AddObserver(AnnotMgrI::AnnotDoubleClicked, callbackCommand, 1.0);

  callbackCommand = vtkCallbackCommand::New();
  callbackCommand->SetCallback(annotAddedCB );
  callbackCommand->SetClientData(this);
  AnnotMgrI::AddObserver(AnnotMgrI::AnnotAdded, callbackCommand, 1.0);

  callbackCommand = vtkCallbackCommand::New();
  callbackCommand->SetCallback(lastAnnotDeletedCB );
  callbackCommand->SetClientData(this);
  AnnotMgrI::AddObserver(AnnotMgrI::LastAnnotRemoved, callbackCommand, 1.0);
 
  callbackCommand = vtkCallbackCommand::New();
  callbackCommand->SetCallback(noAnnotSelectedCB );
  callbackCommand->SetClientData(this);
  AnnotMgrI::AddObserver(AnnotMgrI::LastAnnotRemovedFromSelection, callbackCommand, 1.0);
  
  callbackCommand = vtkCallbackCommand::New();
  callbackCommand->SetCallback(noCurrentAnnotCB );
  callbackCommand->SetClientData(this);
  AnnotMgrI::AddObserver(AnnotMgrI::NoCurrentAnnot, callbackCommand, 1.0);

  callbackCommand = vtkCallbackCommand::New();
  callbackCommand->SetCallback(newCurrentAnnotCB );
  callbackCommand->SetClientData(this);
  AnnotMgrI::AddObserver(AnnotMgrI::NewCurrentAnnot, callbackCommand, 1.0);

  /*--- observe l'evenement clic gauche sur l'annotation ---*/
 /* callbackCommand = vtkCallbackCommand::New();
  callbackCommand->SetCallback(annotLeftClickedCB );
  callbackCommand->SetClientData(this);
  AnnotMgrI::AddObserver(vtkCommand::StartInteractionEvent, callbackCommand, 1.0); */
 
}

void MainWidget::destroy()
{
  // explicit destroy to avoid Memory Leaks
  m_renderer->Delete();
  m_connections->Delete();
  delete m_annotPropsDialog;
  delete m_annotPopup;
}

void MainWidget::fileExit()
{
}


void MainWidget::updateCoords( vtkObject * obj)
{
  // get interactor
  vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(obj);
  // get event position
  int event_pos[2];
  iren->GetEventPosition(event_pos);
  // update label
  QString str;
  str.sprintf("x=%d : y=%d", event_pos[0], event_pos[1]);
  coord->setText(str);

}

/*
 * Services appeles par le protocole de callbacks du manager
* 
*
 * Callback activee sur message, clic droit, de la souris sur une annotation
 * Active le menu cache de l'annotation
 * 
 */
void MainWidget::annotRightClickedCB(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata)
{
   MainWidget* mainWdg = reinterpret_cast<MainWidget*>(clientdata);
   vtkRenderWindowInteractor* iren = mainWdg->qVTK->GetRenderWindow()->GetInteractor();
   
   // get event location
   int* sz = iren->GetSize();
   int* position = iren->GetEventPosition();
   // remember to flip y
   QPoint pt = QPoint(position[0], sz[1]-position[1]);
   // show popup menu at global point
   mainWdg->activateAnnotPopup( pt );
}

/*
 * Callback activee lorsqu'un double clic gauche de la souris intervient 
 *  sur une annotation.
 * Fait apparaitre les proprietes.
 */
void MainWidget::annotDoubleClickedCB(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata)
{
  MainWidget* mainWdg = reinterpret_cast<MainWidget*>(clientdata);
  
  mainWdg->showAnnotProps();
}

/*void MainWidget::annotLeftClickedCB(vtkObject* caller, unsigned long event, void* clientdata, void* calldata)
{
  MainWidget* mainWdg = reinterpret_cast<MainWidget*>(clientdata);
  
  mainWdg->m_annotPropsDialog->updateData(false);//demande la mise a jour des donnees de la fenetre des proprietes 
}
*/
/*
 * Service appele lors de l'ajout d'une annotation dans la scene
 */
void MainWidget::annotAddedCB(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata)
{ 
   MainWidget* mainWdg = reinterpret_cast<MainWidget*>(clientdata);
  /*Met le menu en etat de:
   *   -cacher les annotations de la scene
   */
  mainWdg->annotateHideAction->setEnabled(true);
  mainWdg->annotateDeleteAction->setEnabled(true);
}

/*
 * Service appele lors de la destruction de la derniere annotation de la scene
 */
void MainWidget::lastAnnotDeletedCB(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata)
{
  MainWidget* mainWdg = reinterpret_cast<MainWidget*>(clientdata);
  /*--- remet le menu a l'etat initial ---*/
  mainWdg->annotateShowAction->setEnabled(false);
  mainWdg->annotateHideAction->setEnabled(false);
  mainWdg->annotateDeleteAction->setEnabled(false);
  mainWdg->annotateAnnotateAction->setEnabled(true);
  mainWdg->hideAnnotProps();//cache la fenetre des proprietes
}

/*
 * Service appele lorsque plus aucune annotation n'est selectionnee 
 */
void MainWidget::noAnnotSelectedCB(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata)
{
  MainWidget* mainWdg = reinterpret_cast<MainWidget*>(clientdata);
}


void MainWidget::noCurrentAnnotCB(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata)
{
  MainWidget* mainWdg = reinterpret_cast<MainWidget*>(clientdata);
  mainWdg->hideAnnotProps();//cache la fenetre des proprietes
}

void MainWidget::newCurrentAnnotCB(vtkObject *caller, unsigned long eid, void *clientdata, void *calldata)
{
  MainWidget* mainWdg = reinterpret_cast<MainWidget*>(clientdata);
  mainWdg->m_annotPropsDialog->updateData(false);
}


/*
 * Services appeles par les elements visuels QT 
 *  de l'application
 *
 * Insertion d'une annotation dans la scene
 * 
 */
void MainWidget::annotate()
{
  AnnotMgrI::annotate(m_renderer);
  float p[3];
  p[0]=0;
  p[1]=0;
  p[2]=0;
  AnnotMgrI::set_currentAnnot_attachment(p);
}

/*
 * Service appele lorsque l'item "Masquer" du menu est active
 */
void MainWidget::hideAnnotations()
{ 
   /*Met le menu en etat de:
    *    -empecher l'ajout d'annotation 
    *    -reafficher les annotations de la scene
    */
   annotateAnnotateAction->setEnabled(false);
   annotateHideAction->setEnabled(false);
   annotateDeleteAction->setEnabled(false);
   annotateShowAction->setEnabled(true);
   hideAnnotProps();//cache la fenetre des proprietes
   AnnotMgrI::enableAnnotations(m_renderer,false);
}

/*
 * Service appele lorsque l'item "Afficher" du menu est active
 */
void MainWidget::showAnnotations()
{
   /*Met le menu en etat de :
    *     -cacher les annotations de la scene
    *     -ajouter des annotations a la scene
    */ 
   annotateShowAction->setEnabled(false);
   annotateHideAction->setEnabled(true);
   annotateDeleteAction->setEnabled(true);
   annotateAnnotateAction->setEnabled(true);
   AnnotMgrI::enableAnnotations(m_renderer,true);
}

/*
 * Service appele lors de l'activation du menu sur une annotation par clic droit
 */
void MainWidget::activateAnnotPopup( QPoint pt )
{
   // 
   QPoint global_pt = m_annotPopup->parentWidget()->mapToGlobal(pt);
   m_annotPopup->popup(global_pt);
}

/*
 * Service appele pour l'affichage des proprietes d'une annotation
 */
void MainWidget::showAnnotProps()
{
  m_annotPropsDialog->updateData(false);//mise a jour des donnees de la fenetre des proprietes
  m_annotPropsDialog->show();//affichage de la fenetre des proprietes
}
/*
 * Service appele pour cacher les proprietes d'une annotation
 */
void MainWidget::hideAnnotProps()
{
  m_annotPropsDialog->hide();//la fenetre des proprietes est cachee
}

void MainWidget::deleteAnnot()
{
  AnnotMgrI::deleteCurrentAnnot();
}

void MainWidget::annotTextColor()
{
   //retrouve l'annotation courante
   double col[3];
   AnnotMgrI::get_currentAnnotTextColor(col);
   double frouge = col[0];
   double fvert = col[1];
   double fbleu = col[2];
   int rouge = frouge*255;
   int vert = fvert*255;
   int bleu = fbleu*255;
   QColor c1(rouge,vert,bleu); 
   QColor c = QColorDialog::getColor(c1, 0, QString ("Couleur du texte"), QColorDialog::DontUseNativeDialog);
   frouge = c.red();
   fvert = c.green();
   fbleu = c.blue();
   col[0] = frouge/255;
   col[1] = fvert/255;
   col[2] = fbleu/255;
   AnnotMgrI::set_currentAnnotTextColor(col);
   m_annotPropsDialog->updateData(false);
}

void MainWidget::annotBorderColor()
{
  //retrouve l'annotation courante
   double col[3];
   AnnotMgrI::get_currentAnnotBorderColor(col);
   double frouge = col[0];
   double fvert = col[1];
   double fbleu = col[2];
   int rouge = frouge*255;
   int vert = fvert*255;
   int bleu = fbleu*255;
   QColor c1(rouge,vert,bleu); 
   QColor c = QColorDialog::getColor(c1, 0, QString ("Couleur de la bordure"), QColorDialog::DontUseNativeDialog);
   frouge = c.red();
   fvert = c.green();
   fbleu = c.blue();
   col[0] = frouge/255;
   col[1] = fvert/255;
   col[2] = fbleu/255;
   AnnotMgrI::set_currentAnnotBorderColor(col);
   m_annotPropsDialog->updateData(false);
}


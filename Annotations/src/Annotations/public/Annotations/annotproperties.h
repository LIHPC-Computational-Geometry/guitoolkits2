#ifndef ANNOTPROPERTIES_H
#define ANNOTPROPERTIES_H

#include <QVariant>
#include <QColor>
#include <QWidget>


/*!
 * \mainpage    Page principale de la bibliothèque <I>annotwidgets</I>
 *
 * \section presentation    Présentation
 * La bibliothèque <I>annotwidgets</I> propose des composants <I>Qt</I>pour
 * annoter des scènes <I>VTK</I>.
 *
 * \section binaries    Binaires
 *
 * Cette bibliothèque est disponible sous la forme
 * libiceannotwidgets.version-compilateur-qt.versionqt-vtk.versionvtk.a
 *<BR>
 * où :
 * <OL>
 * <LI><I>Version</I> est la version de la bibliothèque,
 * <LI><I>Compilateur</I> représente le compilateur utilisé et sa version (ex :
 * gnu.3.4.6 pour le la version 3.4.6 du compilateur g++),
 * <LI><I>Versionqt</I> représente la version utilisée de <I>Qt</I>
 * (ex : 4.7.4).
 * <LI><I>Versionvtk</I> représente la version utilisée de <I>VTK</I>
 * (ex : 5.10.0).
 * </OL>
 *
 * \section compilation Compilation
 *
 * La page principale de la boite à outil "Toolkit" contient les informations
 * nécessaire à la compilation d'applications utilisant
 * <A HREF="../../../index.html#REF_VTK">VTK</A> et 
 * <A HREF="../../../index.html#REF_QT">QT</A>.
 *
 * \section linking Edition des liens
 *
 * La page principale de la boite à outil "Toolkit" contient les informations
 * nécessaire à l'édition des liens d'applications utilisant
 * <A HREF="../../../index.html#REF_VTK">VTK</A> et
 * <A HREF="../../../index.html#REF_QT">QT</A>.
 */

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QPushButton;
class QTextEdit;
class QCheckBox;
class QButtonGroup;
class QPushButton;
class QComboBox;
class QGroupBox;

class rgbcoloredit;

class AnnotProperties : public QWidget
{
    Q_OBJECT

public:
    AnnotProperties( QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0 );
    ~AnnotProperties();

    QPushButton* m_closePushbutton;
    QTextEdit* m_annottextEdit;
    QPushButton* m_applyPushbutton;
    QCheckBox* m_checkBorder;
    QCheckBox* m_checkIndex;
    QButtonGroup* m_textalignGroup;
    QPushButton* m_textPosRight;
    QPushButton* m_textPosCenter;
    QPushButton* m_textPosLeft;
    QComboBox* m_fontChooser;
    QGroupBox* m_colorsgroupBox;    

public slots:
    virtual void apply_slot();
    virtual void updateData( bool b );
    virtual void showEvent( QShowEvent * evt );
    virtual void font_activated( int fId);
    virtual void fgdColorChandeg(QColor c);
    virtual void boxColorChangerd(QColor c);

signals:
    void hidding();
    void showing();

protected:
    void init();

protected slots:
    virtual void displayBorder( bool b );
    virtual void displayIndex( bool b );
    virtual void textalignGroup_clicked( int i);
    virtual void checkBorder_stateChanged( int );
    virtual void checkIndex_stateChanged( int );
    
    virtual void languageChange();
    virtual void texteditChanged();

private:
    rgbcoloredit* m_fgdColor;
    rgbcoloredit* m_bkgdColor;
    rgbcoloredit* m_boxColor;
};

#endif // ANNOTPROPERTIES_H

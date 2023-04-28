#ifndef RGBCOLOREDIT_H_
#define RGBCOLOREDIT_H_

#include <QVariant>
#include <QColor>
#include <QPixmap>
#include <QValidator>
#include <QWidget>
#include <QColorDialog>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLineEdit;
class QLabel;
class QToolButton;
class QSpinBox;

class rgbcoloredit : public QWidget
{
    Q_OBJECT

public:
    rgbcoloredit( QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = 0 );
    ~rgbcoloredit();

    QLabel* m_labelColorEdit;
    QToolButton* m_colorButton;
    QLabel* labelRed;
    QSpinBox* m_spinRed;
    QLabel* labelGreen;
    QSpinBox* m_spinGreen;
    QLabel* labelBlue;
    QSpinBox* m_spinBlue;
 
    virtual void init();
    virtual void setLabel( const QString & s );
    virtual void destroy();
    virtual void setColor( const QColor & color );
    virtual QColor getColor();

public slots:

    virtual void edit_colors();
    virtual void redValueChanged( int r );
    virtual void greenValueChanged( int g );
    virtual void blueValueChanged( int b );

protected:
    QHBoxLayout* colorLineLayout;

protected slots:
    virtual void languageChange();

signals:
    void newColor(QColor c);

private:
    QColor m_color;
    QIntValidator* m_validator;
    int m_red;
    int m_green;
    int m_blue;


};

#endif /*RGBCOLOREDIT_H_*/

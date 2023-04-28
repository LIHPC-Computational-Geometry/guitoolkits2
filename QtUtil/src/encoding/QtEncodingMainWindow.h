#ifndef QT_ENCODING_MAIN_WINDOW_H
#define QT_ENCODING_MAIN_WINDOW_H

#ifndef QT_5
#include <QtGui/QMainWindow>
#else   // QT_5
#include <QtWidgets/QMainWindow>
#endif  // QT_5

class QtEncodingMainWindow : public QMainWindow
{
	Q_OBJECT

	public :

	QtEncodingMainWindow (QWidget* parent);
	virtual ~QtEncodingMainWindow ( );


	private slots :

	virtual void exitCallback ( );


	private :

#ifdef QT_4
	QtEncodingMainWindow (const QtEncodingMainWindow&);
#endif	// QT_4
	QtEncodingMainWindow& operator = (const QtEncodingMainWindow&);
};	// class QtEncodingMainWindow


#endif	// QT_ENCODING_MAIN_WINDOW_H

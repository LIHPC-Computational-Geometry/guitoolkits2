#ifndef QT_NETWORK_APP_H
#define QT_NETWORK_APP_H


#include <QApplication>


using namespace std;


class QtNetworkApp : public QApplication
{
	Q_OBJECT

	public :

	QtNetworkApp (int& argc, char** argv);


	public slots :

	virtual void open ( );
	virtual void openNetwork ( );
};	// class QtNetworkApp


#endif	// QT_NETWORK_APP_H

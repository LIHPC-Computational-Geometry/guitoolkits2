#include "QtNetworkApp.h"
#include "TkUtil/Process.h"
#include "TkUtil/RemoteProcess.h"


USING_STD
USING_UTIL


int main (int argc, char* argv [], char* envp[])
{
	Process::initialize (envp);
	QtNetworkApp*	app	= new QtNetworkApp (argc, argv);

	return app->exec ( );
}	// main

#include "app_api.h"
#include "core.h"
#include "TkUtil/util_config.h"


#include <iostream>

USING_STD


extern "C"
{

void hello ( )
{
	string	message ("Hello world from ");
	message	+= string (__FILE__) + string (" ");
	writeMessage (message);
}	// hello


unsigned long getLongValue ( )
{
	return 12;
}	// getLongValue

};	// extern "C"


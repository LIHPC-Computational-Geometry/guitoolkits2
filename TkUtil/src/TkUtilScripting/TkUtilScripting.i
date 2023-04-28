//
// Binding Python/Swig pour accéder aux services de ce module
//


//%module TkUtil
%{
#include "TkUtil/ReferencedObject.h"
#include "TkUtil/UTF8String.h"
#include "TkUtil/Version.h"
using namespace TkUtil;
%}  // module TkUtil

%include TkUtil/UTF8String.h
%include TkUtil/ReferencedObject.h
%include TkUtil/Version.h


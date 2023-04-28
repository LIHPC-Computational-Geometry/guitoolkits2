#include "TkUtil/ProcessMemory.h"
#include "TkUtil/UTF8String.h"

USING_STD


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


ProcessMemory::ProcessMemory ( )
	: _mallinfo ( )
{
	reset ( );
}	// ProcessMemory::ProcessMemory ( )


ProcessMemory::ProcessMemory (const ProcessMemory& mem)
	: _mallinfo (mem._mallinfo)
{
}	// ProcessMemory::ProcessMemory (const ProcessMemory&)


ProcessMemory& ProcessMemory::operator = (const ProcessMemory& mem)
{
	if (this != &mem)
		_mallinfo	= mem._mallinfo;

	return *this;
}	// ProcessMemory::operator =


ProcessMemory::~ProcessMemory ( )
{
}	// ProcessMemory::~ProcessMemory


void ProcessMemory::reset ( )
{
	_mallinfo	= mallinfo ( );
}	// ProcessMemory::reset


unsigned long ProcessMemory::getTotalSpace ( ) const
{
	return getFastMemorySpace ( ) + getMappedMemorySpace ( );
}	// ProcessMemory::getTotalSpace


unsigned long ProcessMemory::getFastMemorySpace ( ) const
{
	return _mallinfo.arena;
}	// ProcessMemory::getFastMemorySpace


unsigned long ProcessMemory::getMappedMemorySpace ( ) const
{
	return _mallinfo.hblkhd;
}	// ProcessMemory::getMappedMemorySpace


unsigned long ProcessMemory::getUsedSpace ( ) const
{
	return _mallinfo.uordblks + _mallinfo.usmblks;
}	// ProcessMemory::getUsedSpace


unsigned long ProcessMemory::getFreeSpace ( ) const
{
	return _mallinfo.fordblks + _mallinfo.fsmblks;
}	// ProcessMemory::getFreeSpace


string ProcessMemory::getReport ( ) const
{
	UTF8String	report (charset);
	report << "Total memory in use .............. : " << getTotalSpace ( )
	       << "\n"
	       << "          ordinary memory ........ : " << getFastMemorySpace ( )
	       << "\n"
	       << "          mapped memory .......... : " << getMappedMemorySpace ( )
	       << "\n"
	       << "Used space ....................... : " << getUsedSpace ( )
	       << "\n"
	       << "Free space ....................... : " << getFreeSpace ( );

//	return report.iso ( );
	return report.utf8 ( );	// v 5.6.1
}	// ProcessMemory::getReport	



END_NAMESPACE_UTIL

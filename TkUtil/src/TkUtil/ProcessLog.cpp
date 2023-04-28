#include "TkUtil/ProcessLog.h"


BEGIN_NAMESPACE_UTIL


ProcessLog::ProcessLog (
	const UTF8String& process, const UTF8String& outputs)
	: DefaultLog (ProcessLog::PROCESS, outputs), _processName (process)
{
}	// ProcessLog::ProcessLog ( )


ProcessLog::ProcessLog (
	const UTF8String& process, const IN_STD string& outputs)
	: DefaultLog (ProcessLog::PROCESS, outputs), _processName (process)
{
}	// ProcessLog::ProcessLog ( )


ProcessLog::ProcessLog (const ProcessLog& log)
	: DefaultLog (log), _processName (log.getProcessName ( ))
{
}	// ProcessLog::ProcessLog (const ProcessLog&)


ProcessLog& ProcessLog::operator = (const ProcessLog& log)
{
	if (&log != this)
	{
		DefaultLog::operator = (log);
		_processName	= log.getProcessName ( );
	}	// if (&log != this)

	return *this;
}	// ProcessLog::operator =


ProcessLog::~ProcessLog ( )
{
}	// ProcessLog::~ProcessLog


Log* ProcessLog::clone ( ) const
{
	return new ProcessLog (*this);
}	// ProcessLog::clone



END_NAMESPACE_UTIL

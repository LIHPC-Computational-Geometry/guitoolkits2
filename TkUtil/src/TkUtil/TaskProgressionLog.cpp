#include "TkUtil/TaskProgressionLog.h"


USING_STD


BEGIN_NAMESPACE_UTIL

static const Charset	charset ("àéèùô");


TaskProgressionLog::TaskProgressionLog (
	const UTF8String& task, double percent, unsigned short dec)
	: DefaultLog (TaskProgressionLog::TASK_PROGRESSION, UTF8String (charset)),
	  _task (task), _percent (percent)
{
	UTF8String	text (charset);
	text << task << " effectué à " << IN_UTIL setprecision (dec)
	     << ios_base::fixed << percent << "%.";
	setText (text);
}	// TaskProgressionLog::TaskProgressionLog ( )


TaskProgressionLog::TaskProgressionLog (const UTF8String& message)
	: DefaultLog (TaskProgressionLog::TASK_PROGRESSION, message),
	  _task ( ), _percent (0.)
{
}	// TaskProgressionLog::TaskProgressionLog ( )


TaskProgressionLog::TaskProgressionLog (const string& message)
	: DefaultLog (TaskProgressionLog::TASK_PROGRESSION, message),
	  _task ( ), _percent (0.)
{
}	// TaskProgressionLog::TaskProgressionLog ( )


TaskProgressionLog::TaskProgressionLog (const TaskProgressionLog& log)
	: DefaultLog (log), _task (log.getTask ( )), _percent (0.)
{
}	// TaskProgressionLog::TaskProgressionLog (const TaskProgressionLog&)


TaskProgressionLog& TaskProgressionLog::operator = (
												const TaskProgressionLog& log)
{
	if (&log != this)
	{
		DefaultLog::operator = (log);
		_task		= log.getTask ( );
		_percent	= log.getProgression ( );
		setText (log.getText ( ));
	}	// if (&log != this)

	return *this;
}	// TaskProgressionLog::operator =


TaskProgressionLog::~TaskProgressionLog ( )
{
}	// TaskProgressionLog::~TaskProgressionLog


Log* TaskProgressionLog::clone ( ) const
{
	return new TaskProgressionLog (*this);
}	// TaskProgressionLog::clone



END_NAMESPACE_UTIL

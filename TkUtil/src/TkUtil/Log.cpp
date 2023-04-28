#include "TkUtil/Log.h"



BEGIN_NAMESPACE_UTIL


const Log::TYPE		Log::NONE				= 0;

const Log::TYPE		Log::ERROR				= 1;
const Log::TYPE		Log::WARNING			= ERROR<<1;
const Log::TYPE		Log::INFORMATION		= WARNING<<1;
const Log::TYPE		Log::PROCESS			= INFORMATION<<1;
const Log::TYPE		Log::SCRIPTING			= PROCESS<<1;
const Log::TYPE		Log::TASK_PROGRESSION	= SCRIPTING<<1;

const Log::TYPE		Log::TRACE_1			= TASK_PROGRESSION<<1;
const Log::TYPE		Log::TRACE_2			= TRACE_1<<1;
const Log::TYPE		Log::TRACE_3			= TRACE_2<<1;
const Log::TYPE		Log::TRACE_4			= TRACE_3<<1;
const Log::TYPE		Log::TRACE_5			= TRACE_4<<1;

const Log::TYPE		Log::PRODUCTION			=
		ERROR | WARNING | INFORMATION | PROCESS | SCRIPTING | TASK_PROGRESSION;

const Log::TYPE		Log::ALL_TRACES			=
						TRACE_1 | TRACE_2 | TRACE_3 | TRACE_4 | TRACE_5;

const Log::TYPE		Log::ALL_KINDS			= PRODUCTION | ALL_TRACES;


Log::Log ( )
{
}	// Log::Log ( )


Log::Log (const Log&)
{
}	// Log::Log (const Log&)


Log& Log::operator = (const Log&)
{
	return *this;
}	// Log::operator =


Log::~Log ( )
{
}	// Log::~Log



END_NAMESPACE_UTIL

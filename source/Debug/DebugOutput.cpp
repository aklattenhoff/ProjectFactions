#include "DebugOutput.h"

inline int DebugOutput(char* str, ...)
{
#ifdef DEBUG_FLAG_DEBUG_OUTPUT
	va_list arg;
	va_start(arg, str);
	vfprintf_s(stdout, str, arg);
	va_end(arg);
#endif

	return 0;
}

inline int DebugOutputFatalError(char* str, ...)
{
#ifdef DEBUG_FLAG_DEBUG_OUTPUT
	va_list arg;
	va_start(arg, str);
	std::string output = "FATAL ERROR: ";
	output.append(str);
	vfprintf_s(stdout, output.c_str(), arg);
	va_end(arg);
#endif

	return 0;
}

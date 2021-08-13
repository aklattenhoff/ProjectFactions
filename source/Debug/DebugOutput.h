#ifndef _DEBUG_OUTPUT_H
#define _DEBUG_OUTPUT_H

#include <string>
#include <stdio.h>
#include <stdarg.h>

#include "DebugFlags.h"

#ifdef __cplusplus
extern "C" {
#endif

extern inline int DebugOutput(char* str, ...);
extern inline int DebugOutputFatalError(char* str, ...);

#ifdef __cplusplus
}
#endif

#endif /* _DEBUG_OUTPUT_H */

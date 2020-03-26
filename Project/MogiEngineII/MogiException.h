#pragma once

#pragma comment(lib, "shlwapi.lib")
#pragma comment( lib, "dbghelp.lib" )

#include <DbgHelp.h>
#include <shlwapi.h>

#include "1_Exception/ExceptionCode.h"
#include "1_Exception/StackWalker.h"
#include "1_Exception/Exception.h"
#include "1_Exception/MiniDump.h"
#include "utils.h"
#include "win32_main.h"

#pragma warning(push, 0)
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <vulkan/vulkan.h>
#include <windows.h>
#pragma warning(pop)


//~ CONSOLE OUTPUT
internal void
win32_console_print(char* message) {
	
	OutputDebugStringA(message);
	DWORD length = (DWORD)strlen(message);
	LPDWORD numberWritten = 0;
	WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE),
				  message,
				  length,
				  numberWritten,
				  NULL);
}

internal void
win32_console_printError(char* message) {
	
	OutputDebugString(message);
	DWORD length = (DWORD)strlen(message);
	LPDWORD numberWritten = 0;
	WriteConsole(GetStdHandle(STD_ERROR_HANDLE),
				 message,
				 length,
				 numberWritten,
				 NULL);
}

//~ LOGGING

internal void
win32_logging_initiliaze(void) {
	
	// TODO(Jai): Create new/wipe existing log file, open it.
}

internal void
win32_logging_shutdown(void) {
	
	// TODO(Jai): If the file is open, close it.
}

internal void
win32_logging_appendToFile(readOnly char* message) {
	
	// TODO(Jai): Write to file.
	// TODO(Jai): Queue up.
}

internal void
win32_logging_output(LogLevel level,
					 readOnly char* message, 
					 ...) {
	
	char levelStr[9];
	b32 isError = false;
	switch (level) {
		
		case LOG_LEVEL_FATAL: {
			strcpy(levelStr,
				   "[FATAL]: ");
			isError = true;
		} break;
		
		case LOG_LEVEL_ERROR: {
			strcpy(levelStr,
				   "[ERROR]: ");
			isError = true;
		} break;
		
		case LOG_LEVEL_WARN: {
			strcpy(levelStr,
				   "[WARN]: ");
		} break;
		
		case LOG_LEVEL_INFO: {
			strcpy(levelStr,
				   "[INFO]: ");
		} break;
		
		case LOG_LEVEL_DEBUG: {
			strcpy(levelStr,
				   "[DEBUG]: ");
		} break;
		
		case LOG_LEVEL_TRACE: {
			strcpy(levelStr,
				   "[TRACE]: ");
		} break;
		
		case LOG_LEVEL_COUNT: break;
	}
	
	va_list args;
	va_start(args, message);
	
	char outMessage[255];
	s32 outMessageLength;
	
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-nonliteral"
	outMessageLength = _vsnprintf(outMessage,
								  sizeof(outMessage) - 1,
								  message,
								  args);
#pragma clang diagnostic pop
	
	va_end(args);
	
	u64 totalLength = (u64)(outMessageLength + 11);
	char* outBuffer = (char*)malloc(sizeof(char) * totalLength);
	sprintf(outBuffer,
			"%s%s",
			levelStr, outMessage);
	
	outBuffer[totalLength - 1] = '\0';
	
	if (isError) { win32_console_printError(outBuffer); }
	else { win32_console_print(outBuffer); }
	
	//win32_logging_appendToFile(outBuffer);
	
	free(outBuffer);
}

//~ MAIN
int
main(int argc, char** argv) {
	
	readOnly char* str = "A test log";
	
	HB_FATAL("Message - Float: %f, Int: %d, Bool: %i, String: '%s'\n", 3.14, 456, true, str);
    HB_ERROR("Message - Float: %f, Int: %d, Bool: %i, String: '%s'\n", 3.14, 456, true, str);
    HB_WARN("Message - Float: %f, Int: %d, Bool: %i, String: '%s'\n", 3.14, 456, true, str);
    HB_INFO("Message - Float: %f, Int: %d, Bool: %i, String: '%s'\n", 3.14, 456, true, str);
    HB_DEBUG("Message - Float: %f, Int: %d, Bool: %i, String: '%s'\n", 3.14, 456, true, str);
    HB_TRACE("Message - Float: %f, Int: %d, Bool: %i, String: '%s'\n", 3.14, 456, true, str);
	return 0;
}

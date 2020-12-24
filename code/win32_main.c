#include "utils.h"
#include "game_platform.h"
#include "win32_main.h"

#include "game.c"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#include <stdarg.h>
#include <stdio.h>
#include <vulkan/vulkan.h>
#include <Windows.h>
#pragma clang diagnostic pop

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

#ifdef _MSC_VER
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
	
	char outMessage[255];
	s32 outMessageLength;
	
	va_list args;
	va_start(args, message);
	outMessageLength = _vsnprintf(outMessage,
								  sizeof(outMessage) - 1,
								  message,
								  args);
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
#endif

//~ CLOCK

typedef struct Clock {
    s64 frequency;
    s64 startTime;
    s64 elapsed;
} Clock;

internal void
clock_initialize(Clock* clock) {
    
    LARGE_INTEGER startTime;
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&startTime);
    clock->frequency = frequency.QuadPart;
    clock->startTime = startTime.QuadPart;
}

internal s64
clock_absoluteTime(Clock* clock) {
    LARGE_INTEGER timeNow;
    QueryPerformanceCounter(&timeNow);
    
    return timeNow.QuadPart;
}

internal void
clock_update(Clock* clock) {
    if (clock->startTime) { clock->elapsed = clock_absoluteTime(clock) - clock->startTime; }
}

internal void
clock_start(Clock* clock) {
    clock->startTime = clock_absoluteTime(clock);
    clock->elapsed = 0;
}

internal void
clock_stop(Clock* clock) {
    clock->startTime = 0;
}

//~ MAIN
int
main(int argsCount, char** args) {
	
    win32_State platformState = {0};
    
#if HOMEBREW_INTERNAL
    LPVOID baseAddress = (LPVOID)TB(2);
#else
    LPVOID baseAddress = (LPVOID)0;
#endif
    game_Memory gameMemory  = {
        .persistentStorageSize = MB(64),
        .transientStorageSize = GB(2)
    };
    
    platformState.totalSize = gameMemory.persistentStorageSize + gameMemory.transientStorageSize;
    platformState.gameMemoryBlock = VirtualAlloc(baseAddress,
                                                 (size_t)platformState.totalSize,
                                                 MEM_RESERVE | MEM_COMMIT, 
                                                 PAGE_READWRITE);
	
    
    game_update();
    
    return 0;
}

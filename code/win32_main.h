/* date = December 16th 2020 7:45 am */

#ifndef WIN32_MAIN_H
#define WIN32_MAIN_H

//~ LOGGING

#define LOG_SIZE 5000

typedef enum LogLevel {
	LOG_LEVEL_FATAL = 0,
	LOG_LEVEL_ERROR = 1,
	LOG_LEVEL_WARN  = 2,
	LOG_LEVEL_INFO  = 3,
	LOG_LEVEL_DEBUG = 4,
	LOG_LEVEL_TRACE = 5,
	
	LOG_LEVEL_COUNT = 6
} LogLevel;

internal void
win32_logging_initiliaze(void);

internal void
win32_logging_shutdown(void);

internal void
win32_logging_output(LogLevel level,
					 readOnly char* message, ...);

internal void
win32_logging_toFile(readOnly char* message);


#define HB_LOG_WARN_ENABLED     1
#define HB_LOG_INFO_ENABLED     1
#define HB_LOG_DEBUG_ENABLED    1
#define HB_LOG_TRACE_ENABLED    1

#if HOMEBREW_RELEASE
#undef HB_LOG_DEBUG_ENABLED
#define HB_LOG_DEBUG_ENABLED    0
#undef HB_LOG_TRACE_ENABLED
#define HB_LOG_TRACE_ENABLED    0
#endif

#define HB_FATAL(message, ...) win32_logging_output(LOG_LEVEL_FATAL,\
message,\
__VA_ARGS__);
#define HB_ERROR(message, ...) win32_logging_output(LOG_LEVEL_ERROR,\
message,\
__VA_ARGS__);

#if HB_LOG_WARN_ENABLED
#define HB_WARN(message, ...) win32_logging_output(LOG_LEVEL_WARN,\
message,\
__VA_ARGS__);
#else
#define HB_WARN(message, ...)
#endif

#if HB_LOG_INFO_ENABLED
#define HB_INFO(message, ...) win32_logging_output(LOG_LEVEL_INFO,\
message,\
__VA_ARGS__);
#else
#define HB_INFO(message, ...)
#endif

#if HB_LOG_DEBUG_ENABLED
#define HB_DEBUG(message, ...) win32_logging_output(LOG_LEVEL_DEBUG,\
message,\
__VA_ARGS__);
#else
#define HB_DEBUG(message, ...)
#endif

#if HB_LOG_TRACE_ENABLED
#define HB_TRACE(message, ...) win32_logging_output(LOG_LEVEL_TRACE,\
message,\
__VA_ARGS__);
#else
#define HB_TRACE(message, ...)
#endif

#endif //WIN32_MAIN_H

/* date = December 16th 2020 7:45 am */

#ifndef WIN32_MAIN_H
#define WIN32_MAIN_H

//~ NOTE(Jai): Recursive macro definition only for 
// editor syntax highlighting
#define DWORD DWORD
#define FILETIME FILETIME
#define HMODULE HMODULE
#define HANDLE HANDLE
#define BITMAPINFO BITMAPINFO
#define HDC HDC
#define LARGE_INTEGER LARGE_INTEGER
#define XINPUT_STATE XINPUT_STATE
#define LONG LONG
#define SHORT SHORT
#define LPVOID LPVOID
#define HINSTANCE HINSTANCE
#define PSTR PSTR
#define HRESULT HRESULT
#define LPCGUID LPCGUID
#define LPUNKNOWN LPUNKNOWN
#define XINPUT_VIBRATION XINPUT_VIBRATION
#define WIN32_FIND_DATAA WIN32_FIND_DATAA
#define WIN32_FILE_ATTRIBUTE_DATA WIN32_FILE_ATTRIBUTE_DATA
#define LPDIRECTSOUNDBUFFER LPDIRECTSOUNDBUFFER
#define DSBUFFERDESC DSBUFFERDESC
#define WAVEFORMATEX WAVEFORMATEX
#define LPDIRECTSOUND LPDIRECTSOUND
#define MSG MSG
#define HCURSOR HCURSOR
#define PAINTSTRUCT PAINTSTRUCT
#define MONITORINFO MONITORINFO
#define LPARAM LPARAM
#define WINDOWPLACEMENT WINDOWPLACEMENT
#define WPARAM WPARAM
#define UINT UINT
#define HWND HWND
#define LRESULT LRESULT
#define SIZE_T SIZE_T
#define WNDCLASSA WNDCLASSA
#define INT INT
#define WNDPROC WNDPROC
#define HMENU HMENU
#define LPDWORD LPDWORD
#define RECT RECT
#define LONG_PTR LONG_PTR

//~ LOGGING

#ifdef _MSC_VER
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
#endif

//~ CLOCK
typedef struct Clock {
    s64 frequency;
    s64 startTime;
    s64 elapsed;
} Clock;

//~ BACKBUFFER
typedef struct win32_OffScreenBuffer {
    // NOTE(Jai): Pixels are always 32 bits wide, Memory Order: BB GG RR xx
    u32 width;
    u32 height;
    u32 pitch;
    u32 bytesPerPixel;
    BITMAPINFO info;
    void* memory;
} win32_OffScreenBuffer;

//~ WINDOWING
typedef struct Window {
    b32 isVisible;
    u32 clientWidth;
    u32 clientHeight;
    u64* handle;
} Window;

//~ PLATFORM STATE
typedef struct win32_State {
    void* gameMemoryBlock;
    u64 totalSize;
    
    Clock clock;
    s64 lastTime;
    
    Window window;
    
} win32_State;

#endif //WIN32_MAIN_H

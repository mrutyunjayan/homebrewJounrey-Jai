#include "utils.h"
#include "game_platform.h"

#include "game.c"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#include <stdarg.h>
#include <stdio.h>
#include <vulkan/vulkan.h>
#include <Windows.h>
#pragma clang diagnostic pop

#include "win32_main.h"

//~ GLOBALS
global b32 running = true;

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
	WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE),
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

//~ MAIN WINDOW CALLBACK
internal LRESULT CALLBACK 
win32_mainWindowCallBack(HWND windowHandle,
                         UINT message,
                         WPARAM wParam, 
                         LPARAM lParam) {
    
    LRESULT result = 0;
    
    switch (message) {
        case WM_CLOSE: {
            running = false;
        } break;
        case WM_DESTROY: {
            running = false;
            PostQuitMessage(0);
        } break;
        default: {
            return DefWindowProc(windowHandle,
                                 message,
                                 wParam,
                                 lParam);
        }
    }
    
    
    return result;
}

//~ WINDOWING
internal b32
win32_window_initialize(Window* window,
                        s32 x, s32 y,
                        u32 width, u32 height,
                        readOnly char* title,
                        readOnly char* className) {
    
    window->isVisible = false;
    window->clientWidth = width;
    window->clientHeight = height;
    window->handle = (u64*)NULL;
    
    s32 clientX = x;
    s32 clientY = y;
    u32 clientWidth = width;
    u32 clientHeight = height;
    
    s32 windowX = x;
    s32 windowY = y;
    s32 windowWidth = (s32)width;
    s32 windowHeight = (s32)height;
    
    u32 windowStyles = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    u32 windowExtendedStyles = WS_EX_APPWINDOW;
    
    RECT borderRect = {0};
    AdjustWindowRectEx(&borderRect,
                       windowStyles,
                       0,
                       windowExtendedStyles);
    
    // NOTE(Jai): Border Rectangle is negative
    
    windowX += borderRect.left;
    windowY +=  borderRect.top;
    
    // Grow by the size of the OS border
    windowWidth += (borderRect.right - borderRect.left);
    windowHeight += (borderRect.bottom - borderRect.top);
    
    HWND windowHandle = CreateWindowExA(windowExtendedStyles,
                                        className,
                                        title,
                                        windowStyles,
                                        windowX, windowY,
                                        windowWidth, windowHeight,
                                        (HWND)NULL,
                                        (HMENU)NULL,
                                        GetModuleHandle(0),
                                        NULL);
    
    if (!windowHandle) {
        MessageBox((HWND)NULL,
                   "Window Creation Failed",
                   "Error",
                   MB_ICONEXCLAMATION | MB_OK);
        return false;
    }
    
    SetWindowLongPtrA(windowHandle,
                      GWLP_USERDATA,
                      (LONG_PTR)windowHandle);
    window->handle = (u64*)windowHandle;
    
    return true;
}

internal b32
win32_window_destroy(Window* window) {
    
    if (window->handle) {
        
        DestroyWindow((HWND)window->handle);
        window->handle = (u64*)NULL;
        window->clientWidth = window->clientHeight = window->isVisible = 0;
        
        return true;
    }
    
    return false;
}

internal b32
win32_window_show(Window* window) {
    
    if (window->handle && !window->isVisible) {
        
        b32 activate = true;
        s32 showWindowCommandFlags = activate ? SW_SHOW : SW_SHOWNOACTIVATE;
        
        ShowWindow((HWND)window->handle,
                   showWindowCommandFlags);
        window->isVisible = true;
        
        return true;
    }
    
    return false;
}

internal b32
win32_window_hide(Window* window) {
    
    if (window->handle && window->isVisible) {
        
        ShowWindow((HWND)window->handle,
                   SW_HIDE);
        window->isVisible = false;
        
        return true;
    }
    
    return false;
}

internal b32
win32_window_close(Window* window) {
    
    if (window->handle) {
        
        CloseWindow((HWND)window->handle);
        window->isVisible = true;
        
        return true;
    }
    
    return false;
}

//~ MAIN
INT 
WinMain(HINSTANCE instance, 
        HINSTANCE prevInstance,
        PSTR commandLine, 
        INT commandShow) {
    
    win32_State platformState = {0};
    
    clock_initialize(&platformState.clock);
    
    WNDCLASSA windowClass = {
        .style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
        .lpfnWndProc = win32_mainWindowCallBack,
        .hInstance = instance,
        .hCursor = LoadCursor((HINSTANCE)NULL, IDC_CROSS),
        .lpszClassName = "Homebrew Journey: Jai"
    };
    
    if (!RegisterClass(&windowClass)) {
        MessageBox((HWND)NULL, 
                   "Window Registeration failed",
                   "Error",
                   MB_ICONEXCLAMATION | MB_OK);
        return -1;
    }
    
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
	
    if (!win32_window_initialize(&platformState.window,
                                 100, 100, //CW_USEDEFAULT, CW_USEDEFAULT,
                                 1280, 720,
                                 "Homebrew Journey: Jai",
                                 windowClass.lpszClassName)) {
        MessageBox((HWND)NULL, 
                   "Window Initialization failed",
                   "Error",
                   MB_ICONEXCLAMATION | MB_OK);
        
        return -1;
    }
    
    win32_window_show(&platformState.window);
    
    MSG message = {0};
    while (running) {
        
        clock_update(&platformState.clock);
        s64 currentTime = platformState.clock.elapsed;
        // NOTE(Jai): Calculate the delta in ms and convert to seconds
        f32 deltaTime = ((f32)currentTime - (f32)platformState.lastTime) * 0.001f;
        
        while (PeekMessage(&message,
                           (HWND)NULL,
                           0, 0,
                           PM_REMOVE)) {
            
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        
        game_update();
        
        platformState.lastTime = currentTime;
    }
    PostMessageA((HWND)platformState.window.handle,
                 WM_DESTROY,
                 0, 0);
    
    return 0;
}

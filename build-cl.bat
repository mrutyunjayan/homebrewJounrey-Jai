@ECHO OFF

IF NOT DEFINED DevEnvDir (
	call "J:\Program Files (x86)\Microsoft Visual Studio\2019\Preview\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64 >nul
)

SET debug_flags=^
	-Zi^
	-Od^
	-MTd^
	-DEBUG

SET compiler_flags=^
	%debug_flags%^
	-nologo^
	-FC^
	-fp:fast^
	-GR-^
	-EHa-^
	-Oi^
	-MTd^
	-TC^
	-Wall^
	-WX^
	-wd4201^
	-wd4100^
	-wd4189^
	-wd4101^
	-wd4068^
	-wd4820^
	-wd4505

SET defines=^
	-DHOMEBREW_SLOW=1^
	-DHOMEBREW_INTERNAL=1^
	-DHOMEBREW_RELEASE=0^
	-D_CRT_SECURE_NO_WARNINGS^
	-DHOMEBREW_CONSOLE

SET libs=^
	user32.lib^
	gdi32.lib^
	winmm.lib^
	Kernel32.lib^
	vulkan-1.lib

SET linker_flags=^
	-link^
	-LIBPATH:%VULKAN_SDK%\Lib^
	-incremental:no^
	-opt:ref^
	-SUBSYSTEM:CONSOLE^
	%libs%

SET project=%cd%

SET include_directories=^
	-I %project%\include^
	-I %VULKAN_SDK%\Include

IF NOT EXIST build mkdir build

PUSHD build

cl^
	%defines%^
	%include_directories%^
	%compiler_flags%^
	%project%\code\win32_main.c^
	-Fehomebrew_journey^
	%linker_flags%

POPD

IF %ERRORLEVEL% NEQ 0  (
	ECHO 	   ===============
	ECHO Error:	%ERRORLEVEL%
	ECHO 	   ================
)
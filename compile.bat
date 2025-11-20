@echo off
set remove_temps="true"
set debug_flag="false"
set file_path=""
set compile_flag="false"

@REM Fetch source
IF "%~1" == "" (
    echo Error: Empty Path
    exit /b 1
)
IF not EXIST "%~1" (
    echo Error: Source program does not exist
    exit /b 1
)
IF not "%~x1" == ".bf" (
    echo Error: Found file extension %~x1, expected .bf
    exit /b 1
)

set in_orig=%~1
set in_path=%~n1.s
set in_root=%~n1
SHIFT

@REM Clean up any previous junk
IF EXIST "brainfire-lib.o" (
    DEL "brainfire-lib.o"
)
IF EXIST "%in_root%.o" (
    DEL "%in_root%.o"
)
IF EXIST "%in_path%" (
    DEL "%in_path%"
)

:parse_args
IF "%~1" == "" GOTO end_parse

IF /I "%~1" == "-save-temps" (
    set remove_temps="false"
) ELSE IF /I "%~1" == "-s" (
    set remove_temps="false"
) ELSE IF /I "%~1" == "-debug" (
    set debug_flag="true"
) ELSE IF /I "%~1" == "-g" (
    set debug_flag="true"
) ELSE IF /I "%~1" == "-help" (
    GOTO print_help
) ELSE IF /I "%~1" == "-r" (
    set compile_flag="true"
) ELSE IF /I "%~1" == "-o" (
    IF /I "%~2" == "" (
        echo Invalid usage of -o flag, no output provided
        echo Use "compile -help" for more information
        exit /b 1
    ) ELSE (
        set file_path=%~2
        SHIFT
    )
) ELSE (
    GOTO print_help
)
SHIFT
GOTO parse_args

:print_help
echo Usage: compile ^<path^>[options]
echo Options:
echo    -help                   Prints this help screen
echo    -save-temps/-s          Preserves the Assembly file after compilation
echo    -debug/-d               Compiles with debug options
echo    -o ^<output-path^>      Outputs brainfire compiler to desired file instead of default brainfire
exit /b 1

:end_parse

@REM Compile compiler
IF not EXIST "internals/brainfire-windows.exe" (
    gcc internals/brainfire-windows.c -o internals/brainfire-windows
)
IF %compile_flag% == "true" (
    gcc internals/brainfire-windows.c -o internals/brainfire-windows
)

@REM Verify output file exists
IF "%file_path%" == "" (
    echo Please specify an output file using -o ^<output-path^>
    exit /b 1
)

@REM Compile
IF not EXIST %in_path% (
    echo. > %in_path%
)

start /WAIT /B "" "%~dp0internals\brainfire-windows.exe" %in_orig% %in_path%
@REM pause
@REM Compile with gdb if specified
IF %debug_flag% == "true" (
    gcc -c -g %in_path% -o %in_root%.o
    gcc -c -g internals/brainfire_lib.s -o "brainfire-lib.o"
    gcc -g %in_root%.o "brainfire-lib.o" -o %file_path%
) ELSE (
    gcc -c %in_path% -o %in_root%.o
    gcc -c internals/brainfire_lib.s -o "brainfire-lib.o"
    gcc %in_root%.o "brainfire-lib.o" -o %file_path%
)

DEL "brainfire-lib.o"
DEL "%in_root%.o"
IF %remove_temps% == "true" (
    DEL %in_path%
)
@echo off
set remove_temps="true"
set debug_flag="false"
set recompile_flag="false"
set file_path=brainfire

:parse_args
IF "%~1" == "" GOTO end_parse

IF /I "%~1" == "-recompile-objects" (
    set recompile_flag="true"
) ELSE IF /I "%~1" == "-r" (
    set recompile_flag="true"
) ELSE IF /I "%~1" == "-save-temps" (
    set remove_temps="false"
) ELSE IF /I "%~1" == "-s" (
    set remove_temps="false"
) ELSE IF /I "%~1" == "-debug" (
    set debug_flag = "true"
) ELSE IF /I "%~1" == "-g" (
    set debug_flag = "true"
) ELSE IF /I "%~1" == "-help" (
    GOTO print_help
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
echo Usage: compile [options]
echo Options:
echo    -help                   Prints this help screen
echo    -recompile-objects/-r   Recompiles object files regardless if they already exist
echo    -save-temps/-s          Preserves the temporary object files after compilation
echo    -debug/-d               Compiles with debug options
echo    -o ^<arg^>                Outputs brainfire compiler to desired file instead of default brainfire
exit /b 1

:end_parse
IF %debug_flag% == "true" (
    gcc -c -g internals/brainfire_runner.s -o temp/br.o
    gcc -c -g internals/brainfire_lib.s -o temp/bl.o
    gcc -g temp/bl.o temp/br.o -o %file_path%
    GOTO remove
)

IF %recompile_flag%=="true" (
    gcc -c internals/brainfire_lib.s -o temp/bl.o
    gcc -c internals/brainfire_runner.s -o temp/br.o
)
IF not EXIST "temp/br.o" (
    gcc -c internals/brainfire_runner.s -o temp/br.o
)
IF not EXIST "temp/bl.o" (
    gcc -c internals/brainfire_lib.s -o temp/bl.o
)

gcc temp/bl.o temp/br.o -o %file_path%

:remove
IF %remove_temps% == "true" (
    DEL "%~dp0temp\bl.o"
    DEL "%~dp0temp\br.o"
)
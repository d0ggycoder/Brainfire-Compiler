@echo off
set remove_temps="true"
:parse_args
IF "%~1" == "" GOTO end_parse

IF /I "%~1" == "-recompile-objects" (
    gcc -c internals/brainfire_lib.s -o temp/bl.o
    gcc -c internals/brainfire_runner.s -o temp/br.o
) ELSE IF /I "%~1" == "-save-temps" (
    set remove_temps="false"
) ELSE (
    echo Unknown option %~1
    echo Usage: %~nx0 [-recompile-objects]
    exit /b 1
)
SHIFT
GOTO parse_args


:end_parse
IF not EXIST "temp/br.o" (
    gcc -c internals/brainfire_runner.s -o temp/br.o
)
IF not EXIST "temp/bl.o" (
    gcc -c internals/brainfire_lib.s -o temp/bl.o
)

gcc temp/bl.o temp/br.o -o test
echo %~dp0

IF %remove_temps% == "true" (
    DEL "%~dp0temp\bl.o"
    DEL "%~dp0temp\br.o"
)
@echo off
IF "%~1"=="" GOTO make_default
IF "%~1"=="resource" GOTO compile_resource
IF "%~1"=="fast" GOTO make_fast
IF "%~1"=="debug" GOTO make_debug
IF "%~1"=="c" GOTO make_c_default
IF "%~1"=="cfast" GOTO make_c_fast
IF "%~1"=="cdebug" GOTO make_c_debug
IF "%~1"=="legacy32" GOTO make_legacy_32
GOTO undefined_command

:compile_resource
cmd /c "%cd%/scripts/resource.bat"
GOTO close

rem V area

:make_debug
cmd /c "%cd%/scripts/debug.bat"
exit

:make_fast
cmd /c "%cd%/scripts/fast.bat"
GOTO close

:make_default
cmd /c "%cd%/scripts/resource.bat"
cmd /c "%cd%/scripts/default.bat"
GOTO close

rem C area
:make_c_default
cmd /c "%cd%/scripts/resource.bat"
cmd /c "%cd%/scripts/default.c.bat"
GOTO close

:make_c_fast
cmd /c "%cd%/scripts/default.c.bat"
GOTO close

:make_c_debug
cmd /c "%cd%/scripts/debug.c.bat"
GOTO close

rem C area (for Windows 2000)
:make_legacy_32
cmd /c "%cd%/scripts/legacy32.c.bat"
GOTO close

:undefined_command
echo Your command was undefined!
pause
GOTO quit

:close
echo Finished!
pause
GOTO quit

:quit
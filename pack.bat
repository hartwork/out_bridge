@ECHO off
IF "%1"=="" GOTO param_error

REM --------------------------------------------------------
REM Zip sources using 7zip
REM --------------------------------------------------------
7z.exe a -tzip -mx=9 -mm=Deflate -mfb=255 "out_bridge_%1.zip" "Binary" "Build" "Source" "cleanup.bat" "LGPL.txt" "pack.bat" "readme.txt" -r -x!Release_Temp\* -x!Build\*.layout -x!Build\*.depend -x!Build\.objs -x!Build\.objs\*
GOTO exit

REM --------------------------------------------------------
REM Usage info
REM --------------------------------------------------------
: param_error
ECHO USAGE: pack XXX
ECHO.
ECHO   XXX is the release id string e.g. "20".
ECHO   Filename will be "out_bridge_XXX.zip".
ECHO.

: exit

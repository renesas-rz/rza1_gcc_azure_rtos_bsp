@echo off

rem <> Manually set path to JLink install directory here if you do not
rem    want to use the auto detect method. Make sure a backslash
rem    is at the end of the path
set BASE=C:\Program Files (x86)\SEGGER\JLink_V670\
if exist "%BASE%\JLink.exe" goto PATH_SET

rem <> Try to automatically detect JLink install directory
set KEYNAME=HKCU\Software\SEGGER\J-Link
set VALNAME=InstallPath
rem Check if JLink is installed first
reg query %KEYNAME% /v %VALNAME%
if not "%ERRORLEVEL%" == "0" (goto NO_PATH)
rem Query the value and then pipe it through findstr in order to find the matching line that has the value.
rem Only grab token 3 and the remainder of the line. %%b is what we are interested in here.
for /f "tokens=2,*" %%a in ('reg query %KEYNAME% /v %VALNAME% ^| findstr %VALNAME%') do (
    set BASE=%%b
)
if exist "%BASE%\JLink.exe" goto PATH_SET

:NO_PATH
chgclr 0C
echo ===================================================================
echo ERROR: You need to set the path for JLink.exe 
echo ===================================================================
pause
chgclr 07
exit
:PATH_SET

rem <> extract the version number from the path
set MINJVER=V600
set JVER=%BASE:~-5%
if %JVER:~0,1%==V (goto check_version)
set JVER=%BASE:~-6%
set JVER=%JVER:~0,4%

:check_version
REM set JVER=%JVER:~0,-1%
rem Remove '_' if present
set JVER=%JVER:_V=V%
echo %BASE%
echo Your JLINK Version is %JVER%
echo Minimum JLINK Version is %MINJVER%
echo.
if /I %JVER% GEQ %MINJVER% (goto JTAGCONF_CHECK)
chgclr 0C
echo ===================================================================
echo ERROR: You need at least JLINK verison %MINJVER%
echo ===================================================================
pause
chgclr 07
exit

:JTAGCONF_CHECK

:loop

 
echo.
echo Please select one of the following options:
echo. 
echo  1 - Flash Streamit bootloader
echo  2 - Flash Displayit bootloader
echo. 
echo  x - Exit
echo.
set /p SELECTED=Your choice: 

if "%SELECTED%" == "x" goto :eof
if "%SELECTED%" == "1" goto :StreamitBootloader
if "%SELECTED%" == "2" goto :DisplayitBootloader
goto :errorInput 


:StreamitBootloader
echo.
echo You are about to flash bootloader for StreamIt, do you want to proceed? (y/n)
set /p INPUT=
if %INPUT% NEQ y goto Bootloader_Selection
cd JLink
"%BASE%\JLink.exe" -if JTAG -speed 12000 -device R7S721031 -jtagconf -1,-1 -CommanderScript %~dp0\JLink\FlashBootloader_StreamIt.Command
pause
cd ..
goto :loop

:DisplayitBootloader
echo.
echo You are about to flash bootloader for StreamIt, do you want to proceed? (y/n)
set /p INPUT=
if %INPUT% NEQ y goto Bootloader_Selection
cd JLink
"%BASE%\JLink.exe" -if JTAG -speed 12000 -device R7S721031 -jtagconf -1,-1 -CommanderScript %~dp0\JLink\FlashBootloader_Displayit.Command
pause
cd ..
goto :loop

:eof
exit
:errorInput
echo.
echo Illegal input! Please try again!
echo.
pause
goto :loop


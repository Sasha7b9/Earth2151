@echo off
MSBuild.exe ..\..\generated\wxWidgets\wxWidgets.sln -t:rebuild
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Success


:Failed
echo .
echo .
echo .
echo       !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!!!!!!!!
echo .
echo .
echo .
goto Exit

:Success

:Exit


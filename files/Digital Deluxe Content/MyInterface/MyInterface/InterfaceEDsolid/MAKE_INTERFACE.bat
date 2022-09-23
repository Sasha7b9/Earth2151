@echo off

rem --- Setting variables ---

SET TOOLS_PATH=..\..\Tools
SET INTERFACE_NAME="My great interface"
SET INTERFACE_ID=JS


rem --- Initial Clearing ---

if exist Interface%INTERFACE_ID%.wd del Interface%INTERFACE_ID%.wd
mkdir Interface


rem --- Converting INTERFACE cursors  ---

%TOOLS_PATH%\pic2tex.exe -key -ccursors.txt 4 9 #cursors.tga
move cursors.tex Interface\cursors%INTERFACE_ID%.tex


rem --- Converting INTERFACE compass ---

%TOOLS_PATH%\Pic2Tex.exe -key -mipmap9 2 1 #compass.tga
move compass.tex Interface\compass%INTERFACE_ID%.tex


rem --- Converting INTERFACE textures ---

%TOOLS_PATH%\Pic2Tex.exe -key -mipmap1 8 4 DIAL32.bmp
%TOOLS_PATH%\Pic2Tex.exe -key -mipmap1 4 1 DIAL64.bmp
%TOOLS_PATH%\Pic2Tex.exe -key -mipmap1 3 1 #DIALT32.tga

move DIAL32.tex Interface\DIAL32%INTERFACE_ID%.tex
move DIAL64.tex Interface\DIAL64%INTERFACE_ID%.tex
move DIALT32.tex Interface\DIALT32%INTERFACE_ID%.tex
copy DIAL32@.bmp DIAL32%INTERFACE_ID%@.bmp
copy DIAL64@.bmp DIAL64%INTERFACE_ID%@.bmp
copy DIALT32@.bmp DIALT32%INTERFACE_ID%@.bmp


rem --- Converting INTERFACE data ---

copy dialogs.txt dialogs%INTERFACE_ID%.txt
echo Materials >> dialogs%INTERFACE_ID%.txt
echo { >> dialogs%INTERFACE_ID%.txt
echo dial64%INTERFACE_ID%:64 >> dialogs%INTERFACE_ID%.txt
echo dial32%INTERFACE_ID%:32 >> dialogs%INTERFACE_ID%.txt
echo dialT32%INTERFACE_ID%:32 >> dialogs%INTERFACE_ID%.txt
echo } >> dialogs%INTERFACE_ID%.txt

%TOOLS_PATH%\InterfaceMap.exe dialogs%INTERFACE_ID%.txt Interface\dialogs%INTERFACE_ID%.dat

del dialogs%INTERFACE_ID%.txt
del DIAL32%INTERFACE_ID%@.bmp
del DIAL64%INTERFACE_ID%@.bmp
del DIALT32%INTERFACE_ID%@.bmp


rem --- Creating INTERFACE definition file ---

echo Dialogs%INTERFACE_ID%>Interface\Interface%INTERFACE_ID%.def
echo Cursors%INTERFACE_ID%>>Interface\Interface%INTERFACE_ID%.def
echo Compass%INTERFACE_ID%>>Interface\Interface%INTERFACE_ID%.def

%TOOLS_PATH%\AddFileInfo.exe Interface\Interface%INTERFACE_ID%.def Interface\Interface%INTERFACE_ID%.int -text -name %INTERFACE_NAME%

del Interface\Interface%INTERFACE_ID%.def


rem --- Binding data into WD file ---

%TOOLS_PATH%\WDCreator.exe Interface%INTERFACE_ID%.wd p ./Interface/*.tex ./Interface/*.dat ./Interface/*.int

rem --- Clearing ---

del Interface\cursors%INTERFACE_ID%.tex
del Interface\compass%INTERFACE_ID%.tex
del Interface\DIAL32%INTERFACE_ID%.tex
del Interface\DIAL64%INTERFACE_ID%.tex
del Interface\DIALT32%INTERFACE_ID%.tex
del Interface\dialogs%INTERFACE_ID%.dat
del Interface\Interface%INTERFACE_ID%.int
rmdir Interface


echo ------------------------------------------------------
echo - All Work complete.
echo - Interface %INTERFACE_NAME% stored in file:
echo - Interface%INTERFACE_ID%.wd
echo ------------------------------------------------------

pause

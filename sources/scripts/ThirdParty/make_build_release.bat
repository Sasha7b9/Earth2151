cd ../../ThirdParty/wxWidgets
rmdir generated /s /q
cd ../scripts
cmake.exe ..\..\ThirdParty\wxWidgets\CMakeLists.txt -B..\..\generated\wxWidgets -G "Visual Studio 17 2022" -A Win32 -DwxBUILD_SHARED=ON -DwxBUILD_USE_STATIC_RUNTIME=ON
call build.bat

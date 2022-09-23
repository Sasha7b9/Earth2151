rmdir ..\..\ThirdParty\wxWidgets\generated /s /q
cmake.exe ..\..\ThirdParty\wxWidgets\CMakeLists.txt -B..\..\generated\wxWidgets -DCMAKE_SYSTEM_VERSION=10.0.18362.0 -G "Visual Studio 17 2022" -A Win32 -DwxBUILD_SAMPLES=ALL

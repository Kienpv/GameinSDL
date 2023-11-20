@echo off

"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake" -S ../ -B . -G "Visual Studio 15 2017" -A x64 -DCMAKE_BUILD_TYPE=Release
"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\amd64\MSBuild" gSDL.sln /p:Configuration=Release /p:VisualStudioVersion=15.0
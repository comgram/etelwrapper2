cls
REM You should excute this batch file in VC x64 Native tool Environment
REM call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

IF EXIST build GOTO EXISTDIR
:EXISTDIR
rmdir /q /s build
mkdir build
:END



IF EXIST my_dist GOTO EXISTDIR
:EXISTDIR
rmdir /q /s my_dist
mkdir my_dist
:END


python setup.py build_ext

copy ..\lib\w64\dll\*c.dll my_dist
copy ..\lib\w64\dll\7z.dll my_dist
copy ..\lib\w64\dll\7z.exe my_dist
copy build\lib.win-amd64-3.8\*.pyd my_dist

copy ..\lib\w64\dll\*c.dll test
copy ..\lib\w64\dll\7z.dll test
copy ..\lib\w64\dll\7z.exe test
copy build\lib.win-amd64-3.8\*.pyd test

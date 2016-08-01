call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

set SOURCE_ROOT=C:\Qt\source
set OUTPUT_ROOT=C:\Qt\install
set BUILD_ROOT=C:\Qt\build
set QTVER=5.7.0
set VERNUM=2013
set PLATFORM=86
set JOMDIR=C:\Qt\Tools\QtCreator\bin

set CURRENT_DIR=%~dp0
set QMAKESPEC=win32-msvc%VERNUM%
set OUTPUT_DIR=%OUTPUT_ROOT%\%QTVER%-msvc%VERNUM%-x%PLATFORM%-opengl
set BUILD_DIR=%BUILD_ROOT%\%QTVER%-msvc%VERNUM%-x%PLATFORM%-opengl

set PATH=%JOMDIR%;%BUILD_DIR%\qtbase\bin;%BUILD_DIR%\gnuwin32\bin;%PATH%

rem mkdir %OUTPUT_DIR%

mkdir %BUILD_DIR%
cd %BUILD_DIR%

call %SOURCE_ROOT%\qt-everywhere-opensource-src-%QTVER%\configure.bat ^
    -prefix "%OUTPUT_DIR%" ^
    -debug-and-release ^
    -confirm-license -opensource ^
    -no-angle ^
    -saveconfig config ^
    -opengl desktop ^
    -no-warnings-are-errors ^
    -nomake examples ^
    -nomake tests ^
    -skip qtdeclarative ^
    -skip qtwebengine ^
    -skip qtwebchannel

call jom clean
call jom
call jom install

cd %CURRENT_DIR%

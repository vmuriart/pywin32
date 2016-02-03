set MSVC_VERSION=2015
set MSVC_DIR=C:\Program Files (x86)\Microsoft Visual Studio 14.0
rem if [%LIBPATH%]==[] call "%MSVC_DIR%\VC\vcvarsall" amd64
set VS90COMNTOOLS=%VS140COMNTOOLS%
set VS100COMNTOOLS=%VS140COMNTOOLS%
set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC
call vcvarsall.bat x86_amd64

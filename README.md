# PyWin32

This is the readme for the Python for Win32 (pywin32) extensions source code.

See `CHANGELOG.md` for recent changes.

`setup.py` is a standard distutils build script.  You probably want to:

    python setup.py install

or

    python setup.py --help

These extensions require the same version of MSVC as used for the
corresponding version of Python itself.  Some extensions require a recent
"Platform SDK"  from Microsoft, and in general, the latest service packs
should be  installed, but run `setup.py` without any arguments to see
specific information about dependencies.  A vanilla MSVC installation should
be able to build most extensions and list any extensions that could not be
built due to missing libraries - if the build actually fails with your
configuration, please log a bug via http://sourceforge.net/projects/pywin32.

May need to run the following before building:

    set MSVC_VERSION=2015
    set MSVC_DIR=C:\Program Files (x86)\Microsoft Visual Studio 14.0
    rem if [%LIBPATH%]==[] call "%MSVC_DIR%\VC\vcvarsall" amd64
    set VS90COMNTOOLS=%VS140COMNTOOLS%
    set VS100COMNTOOLS=%VS140COMNTOOLS%
    set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC
    call vcvarsall.bat x86_amd64

@echo off
pushd %temp%
set BUILD_NAME=noconsole_build
if not exist %BUILD_NAME% md %BUILD_NAME%
cd %BUILD_NAME%
cmake %~dp0 -G"MinGW Makefiles"
\r\nset BUILD_CORES=2\r\nif DEFINED CXX_BUILD_CORES set BUILD_CORES=%CXX_BUILD_CORES%\r\nmingw32-make -j%BUILD_CORES% install
popd
@echo on
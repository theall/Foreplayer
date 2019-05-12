@echo off
pushd %temp%
set BUILD_NAME=expat_build
if not exist %BUILD_NAME% md %BUILD_NAME%
cd %BUILD_NAME%
cmake %~dp0 -G"MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=%~dp0\..\..\ -DBUILD_tools=OFF -DBUILD_examples=OFF -DBUILD_tests=OFF -DBUILD_tools_default=OFF -DBUILD_shared=OFF

set BUILD_CORES=2
if DEFINED CXX_BUILD_CORES set BUILD_CORES=%CXX_BUILD_CORES%
mingw32-make -j%BUILD_CORES% install
popd
@echo on
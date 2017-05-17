@echo off
set CURDIR=%cd%
pushd %temp%
md fft_build
cd fft_build
cmake %CURDIR% -G"MinGW Makefiles"
mingw32-make -j2 install
popd
@echo on
@echo Building aosdk...
@pushd aosdk
@call build.bat
@popd
@echo Building aosdk completed.

@echo Building Audio3D...
@pushd Audio3D
@call build.bat
@popd
@echo Building Audio3D completed.

@echo Building fftreal...
@pushd fftreal
@call build.bat
@popd
@echo Building fftreal completed.

@echo Building gme...
@pushd gme
@call build.bat
@popd
@echo Building gme completed.

@echo Building m1lib...
@pushd m1lib
@call build.bat
@popd
@echo Building m1lib completed.

@echo Building unrar...
@pushd unrar
@call build.bat
@popd
@echo Building unrar completed.

@echo Building unzip...
@pushd unzip
@call build.bat
@popd
@echo Building unzip completed.

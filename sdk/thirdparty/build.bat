@echo Building aosdk...
@call %~dp0\aosdk\build.bat
@echo Building aosdk completed.

@echo Building Audio3D...
@call %~dp0\Audio3D\build.bat
@echo Building Audio3D completed.

@echo Building fftreal...
@call %~dp0\fftreal\build.bat
@echo Building fftreal completed.

@echo Building gme...
@call %~dp0\gme\build.bat
@echo Building gme completed.

@echo Building m1lib...
@call %~dp0\m1lib\build.bat
@echo Building m1lib completed.

@echo Building unrar...
@call %~dp0\unrar\build.bat
@echo Building unrar completed.

@echo Building unzip...
@call %~dp0\unzip\build.bat
@echo Building unzip completed.

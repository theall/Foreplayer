@echo off
protoc.exe --version>nul
if not %errorlevel%==0 (
	echo Can't find protoc.exe.
	@echo on
	exit/b
)

protoc.exe m1.proto --cpp_out=..\src\
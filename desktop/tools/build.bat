@call %~dp0\3dparty\build.bat

@echo Building noconsole...
@call %~dp0\noconsole\build.bat
@echo Building noconsole completed.

@echo Building exportor...
@call %~dp0\exportor\build.bat
@echo Building exportor completed.


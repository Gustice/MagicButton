:: Resources\DemoFail.bat
@echo off
echo %time%
timeout 4 > NUL
echo %time%

exit 1
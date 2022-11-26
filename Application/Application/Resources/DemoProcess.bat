:: Resources\DemoProcess.bat
@echo off
echo %time%
timeout 5 > NUL
echo %time%

exit 0
exit 1 :: Move up to show failed process
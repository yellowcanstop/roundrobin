@echo OFF

:simulate
start /B /wait roundrobin.exe
echo This is a console only output. If you want to save your output, use RUN_AND_SAVE.bat

choice /m "Start New Simulation"
if errorlevel 2 goto end
if errorlevel 1 goto simulate

:end
exit
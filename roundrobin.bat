@echo OFF

:simulate
start /B /wait roundrobin.exe > analysis.txt
start analysis.txt
if not exist "log" mkdir log
for /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c-%%a-%%b)
for /f "tokens=1-2 delims=/:" %%a in ('time /t') do (set mytime=%%a%%b)
copy /y analysis.txt "log/%mydate%-%mytime%.txt"
echo This is a temporary file. You can view all saved outputs in the log folder.

choice /m "Start New Simulation"
if errorlevel 2 goto end
if errorlevel 1 goto simulate

:end
exit
@echo off
echo Compiling HMS project...
g++ main.cpp utils.cpp patient.cpp doctor.cpp appointment.cpp bed.cpp pharmacy.cpp billing.cpp hospitalGraph.cpp -o HMS.exe
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b
)
echo Compilation successful!
echo Run HMS.exe to start the program.
pause

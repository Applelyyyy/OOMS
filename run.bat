@echo off
echo Compiling program...
if not exist output (
    mkdir output
)
if not exist log (
    mkdir log
)
:: Compile...
gcc main.c github_sync.c -o output/main.exe 2> log/Compile_error.txt

:: Check if compilation succeeded
if %errorlevel% neq 0 (
    echo ❌ Compilation failed. error in log Folder:
    type log\Compile_error.txt
    pause
    exit /b
)
start cmd /c "cd output && main.exe"
exit

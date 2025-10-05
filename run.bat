@echo off
echo Compiling program...
if not exist output (
    mkdir output
)
if not exist log (
    mkdir log
)

gcc main.c github_sync.c unit_test.c -o output/main.exe 2> log/Compile_error.txt


if %errorlevel% neq 0 (
    echo ❌ Compilation failed. error in log Folder:
    type log\Compile_error.txt
    pause
    exit /b
)
echo Compilation successful. Running program...
start cmd /c "cd output && main.exe"
exit

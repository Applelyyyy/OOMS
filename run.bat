@echo off
setlocal enabledelayedexpansion

REM Check if running as administrator
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo.
    echo Administrator privileges required for automatic installation.
    echo    Right-click on run.bat and select "Run as administrator"
    echo.
    pause
    goto :check_gcc_without_admin
)

:check_gcc_without_admin
echo Checking for GCC compiler...

REM Check if gcc is available
gcc --version >nul 2>&1
if %errorlevel% equ 0 (
    echo GCC found!
    goto :compile
)

echo ❌ GCC not found.
echo.

REM Check if running as admin for auto-install
net session >nul 2>&1
if %errorlevel% neq 0 (
    echo GCC is required to compile this program.
    echo.
    echo Options:
    echo 1. Right-click run.bat and "Run as administrator" for auto-install
    echo 2. Install MinGW-w64 manually from: https://www.mingw-w64.org/downloads/
    echo 3. Use package managers:
    echo    - Chocolatey: choco install mingw
    echo    - winget: winget install mingw-w64
    echo.
    pause
    exit /b 1
)

REM Auto-install section (runs only with admin privileges)
echo 🔧 Attempting to install MinGW-w64 automatically...
echo.

REM Check if winget is available
winget --version >nul 2>&1
if %errorlevel% equ 0 (
    echo Using winget to install MinGW-w64...
    winget install -e --id mingw-w64.mingw-w64
    if !errorlevel! equ 0 (
        echo MinGW-w64 installed successfully via winget!
        echo Refreshing environment variables...
        call refreshenv >nul 2>&1
        goto :verify_install
    ) else (
        echo ❌ winget installation failed, trying Chocolatey...
        goto :try_chocolatey
    )
)

:try_chocolatey
REM Check if chocolatey is available
choco --version >nul 2>&1
if %errorlevel% equ 0 (
    echo Using Chocolatey to install MinGW-w64...
    choco install mingw -y
    if !errorlevel! equ 0 (
        echo MinGW-w64 installed successfully via Chocolatey!
        echo Refreshing environment variables...
        call refreshenv >nul 2>&1
        goto :verify_install
    ) else (
        echo ❌ Chocolatey installation failed.
        goto :manual_install_prompt
    )
)

if !errorlevel! equ 0 (
    echo MinGW-w64 downloaded and extracted to C:\mingw64
    echo Adding to system PATH...
    
    REM Add to system PATH
    for /f "tokens=2*" %%a in ('reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v PATH 2^>nul') do set "SYSTEM_PATH=%%b"
    
    echo !SYSTEM_PATH! | find "C:\mingw64\bin" >nul
    if !errorlevel! neq 0 (
        reg add "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v PATH /t REG_EXPAND_SZ /d "!SYSTEM_PATH!;C:\mingw64\bin" /f >nul
        echo PATH updated successfully!
    )
    
    REM Update current session PATH
    set "PATH=%PATH%;C:\mingw64\bin"
    goto :verify_install
) else (
    goto :manual_install_prompt
)

:manual_install_prompt
echo Automatic installation failed.
echo.
echo Please install MinGW-w64 manually:
echo 1. Download from: https://www.mingw-w64.org/downloads/
echo 2. Or install package managers:
echo    - Chocolatey: https://chocolatey.org/install
echo    - Then run: choco install mingw
echo 3. Or use winget: winget install mingw-w64
echo.
echo After installation, run this script again.
pause
exit /b 1

:verify_install
echo Verifying GCC installation...
timeout /t 2 /nobreak >nul

REM Refresh PATH and check again
gcc --version >nul 2>&1
if %errorlevel% equ 0 (
    echo GCC verification successful!
    gcc --version
    echo.
    goto :compile
) else (
    echo ❌ GCC still not found after installation.
    echo Please restart your command prompt or computer and try again.
    echo    Or add C:\mingw64\bin to your PATH manually.
    pause
    exit /b 1
)

:compile
echo  Checking for PowerShell (required for GitHub sync)...
powershell -Command "Write-Host 'PowerShell available'" >nul 2>&1
if %errorlevel% equ 0 (
    echo PowerShell found!
) else (
    echo PowerShell not found. GitHub sync may not work.
)

echo.
echo Creating directories...
if not exist output (
    mkdir output
    echo Created output directory
)
if not exist log (
    mkdir log
    echo Created log directory
)
if not exist data (
    mkdir data
    echo Created data directory
)

echo.
echo Compiling program...
gcc main.c github_sync.c unit_test.c E2E.c -o output/main.exe 2> log/Compile_error.txt

if %errorlevel% neq 0 (
    echo Compilation failed. Error details:
    echo.
    type log\Compile_error.txt
    echo.
    echo Common solutions:
    echo - Make sure all .c files exist in the current directory
    echo - Check for syntax errors in the source code
    echo - Ensure GCC is properly installed and in PATH
    echo.
    pause
    exit /b 1
)

echo Compilation successful!
echo Running program...

start cmd /c "cd output && main.exe && pause"
exit
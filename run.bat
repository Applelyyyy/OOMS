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
    echo    - winget: winget install MSYS2.MSYS2
    echo.
    pause
    exit /b 1
)

REM Auto-install section (runs only with admin privileges)
echo 🔧 Attempting to install MSYS2.MSYS2 automatically...
echo.

REM Check if winget is available
winget --version >nul 2>&1
if %errorlevel% equ 0 (
    echo Using winget to install MSYS2...
    winget install MSYS2.MSYS2
    if !errorlevel! equ 0 (
        echo MSYS2 installed successfully via winget!
        echo Installing MinGW-w64 GCC compiler in MSYS2...
        
        REM Wait for MSYS2 to be ready
        timeout /t 3 /nobreak >nul
        
        REM Install GCC compiler via pacman in MSYS2
        echo Running pacman to install mingw-w64-ucrt-x86_64-gcc...
        C:\msys64\usr\bin\bash.exe -lc "pacman -S --noconfirm mingw-w64-ucrt-x86_64-gcc"
        
        if !errorlevel! equ 0 (
            echo GCC compiler installed successfully in MSYS2!
            
            REM Add MSYS2 MinGW64 to PATH temporarily
            set "PATH=C:\msys64\mingw64\bin;%PATH%"
            
            echo Refreshing environment variables...
            call refreshenv >nul 2>&1
        ) else (
            echo ⚠️ Warning: GCC installation via pacman failed, but continuing...
        )
        
        goto :verify_install
    ) else (
        echo ❌ winget installation failed
        goto :manual_install_prompt
    )
)

:manual_install_prompt
echo Automatic installation failed.
echo.
echo Please install MinGW-w64 manually:
echo.
echo Option 1 - MSYS2 (Recommended):
echo 1. Download MSYS2 from: https://www.msys2.org/
echo 2. Install MSYS2 to C:\msys64\
echo 3. Open MSYS2 terminal and run: pacman -S --noconfirm mingw-w64-ucrt-x86_64-gcc
echo 4. Add C:\msys64\mingw64\bin to your system PATH
echo.
echo Option 2 - Direct MinGW-w64:
echo 1. Download from: https://code.visualstudio.com/docs/cpp/config-mingw
echo.
echo After installation, run this script again.
pause
exit /b 1

:verify_install
echo Verifying GCC installation...
timeout /t 2 /nobreak >nul

REM Try to find GCC in common MSYS2 locations
set "MSYS2_GCC_PATH=C:\msys64\mingw64\bin\gcc.exe"
if exist "%MSYS2_GCC_PATH%" (
    echo Found GCC in MSYS2 installation!
    set "PATH=C:\msys64\mingw64\bin;%PATH%"
    "%MSYS2_GCC_PATH%" --version
    echo.
    goto :compile
)

REM Refresh PATH and check again
gcc --version >nul 2>&1
if %errorlevel% equ 0 (
    echo GCC verification successful!
    gcc --version
    echo.
    goto :compile
) else (
    echo ❌ GCC still not found after installation.
    echo.
    echo Troubleshooting steps:
    echo 1. Check if MSYS2 is installed in C:\msys64\
    echo 2. Manually run: C:\msys64\usr\bin\bash.exe -lc "pacman -S --noconfirm mingw-w64-ucrt-x86_64-gcc"
    echo 3. Add C:\msys64\mingw64\bin to your system PATH
    echo 4. Restart your command prompt or computer
    echo.
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

start cmd /c "cd output && main.exe"
exit
# 🛒 OOMS - Online Order Management System

OOMS is a command-line based Order Management System written in C. It helps manage customer orders and inventory using CSV files for data storage.

---

## 📁 Folder Structure

```plaintext
OOMS/
├── log/                        # Log error compile
    ├── Compile_error.txt       # Log file
├── data/                       # Contains raw_data.csv and other data files
    ├── *.csv                   # CSV files folder
├── output/                     # Compiled executable folder
    ├── main.exe                # Compiled executable file (Windows)
    ├── main                    # Compiled executable file (Linux/macOS)
├── main.c                      # Main source file
├── github_sync.h               # Git sync header file
├── github_sync.c               # Git sync source file
├── unit_test.c                 # Unit testing file
├── E2E.c                       # End-to-End testing file
├── README.md                   # Project documentation
├── run.bat                     # Compile and run script (Windows)
├── run.sh                      # Compile and run script (Linux/macOS)
```

---

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/Applelyyyy/OOMS.git
cd OOMS
```

### 2. Compile and Run the Program

#### 🪟 Windows (Automatic Installation)

**Method 1: Auto Compile with Dependency Check (Recommended)**

Simply run:
```cmd
run.bat
```

The script will automatically:

- Check if `gcc` is installed
- Install MinGW-w64 if needed (requires user confirmation)
- Check for PowerShell (for GitHub sync functionality)
- Create necessary directories
- Compile and run the program

#### 🐧 Linux/macOS (Automatic Installation)

**Method 1: Auto Compile with Dependency Check (Recommended)**

First, make the script executable and run:
```bash
chmod +x run.sh
./run.sh
```

The Linux script will automatically:

- Detect your Linux distribution (Ubuntu/Debian, Red Hat/CentOS, Fedora, Arch, openSUSE, Alpine)
- Check if `gcc` is installed
- Offer to install GCC and build tools using your system's package manager
- Check for wget/curl (for GitHub sync functionality)
- Create necessary directories
- Compile and run the program

**Method 2: Manual Installation**

If auto installation fails, install dependencies manually:

1. **Install MinGW-w64** (for GCC compiler):
   - Using GCC with MinGW : <https://code.visualstudio.com/docs/cpp/config-mingw>
   - Download from: <https://www.msys2.org/>
   - Or use package managers:

     ```cmd
     # Using Chocolatey
     choco install mingw
     
     # Using winget
     winget install mingw-w64
     ```

2. **Ensure PowerShell is available** (usually pre-installed on Windows 10/11)

3. **Compile manually**:

   ```cmd
   gcc main.c github_sync.c unit_test.c E2E.c -o output/main.exe
   cd output
   main.exe
   ```

#### 🐧 Linux/macOS Manual Installation

If automatic installation doesn't work, install manually:

1. **Install GCC and build tools**:

   ```bash
   # Ubuntu/Debian
   sudo apt-get update && sudo apt-get install build-essential
   
   # Red Hat/CentOS
   sudo yum groupinstall "Development Tools"
   
   # Fedora
   sudo dnf groupinstall "Development Tools and Libraries"
   
   # Arch Linux
   sudo pacman -S base-devel
   
   # openSUSE
   sudo zypper install -t pattern devel_basis
   
   # macOS (with Homebrew)
   brew install gcc
   ```

2. **Install download tools** (optional, for GitHub sync):

   ```bash
   # Most distributions
   sudo apt-get install wget curl  # Ubuntu/Debian
   sudo yum install wget curl      # Red Hat/CentOS
   ```

3. **Compile manually**:

   ```bash
   mkdir -p output log data
   gcc main.c github_sync.c unit_test.c E2E.c -o output/main
   cd output && ./main
   ```

---

## 📋 Prerequisites

### Windows

- **GCC Compiler**: MinGW-w64 (auto-installed by `run.bat`)
- **PowerShell**: For GitHub sync functionality (usually pre-installed)
- **Administrator Rights**: May be required for automatic installation

### Linux

- **GCC Compiler**: Auto-installed by `run.sh` with your package manager
- **Build Tools**: build-essential, Development Tools, etc. (auto-installed)
- **wget/curl**: For GitHub sync functionality (auto-checked)
- **sudo privileges**: May be required for package installation

### macOS

- **GCC Compiler**: Install via Xcode Command Line Tools or Homebrew
- **curl**: Usually pre-installed for GitHub sync functionality

---

## 💻 Platform Support

| Platform | Script | Executable | Auto Install |
|----------|--------|------------|--------------|
| Windows  | `run.bat` | `main.exe` | ✅ MinGW-w64 |
| Linux    | `run.sh` | `main` | ✅ GCC + Build Tools |
| macOS    | `run.sh` | `main` | ⚠️ Manual GCC install |

---

## 🔧 Troubleshooting

### Windows Issues

- **"gcc not found"**: Run `run.bat` as Administrator for auto-installation
- **PowerShell execution policy**: Run `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser`
- **Manual GCC install**: Download from mingw-w64.org

### Linux/macOS Issues

- **"gcc not found"**: Install build-essential or development tools for your distribution
- **Permission denied**: Make sure `run.sh` is executable with `chmod +x run.sh`
- **Package manager not found**: The script supports most major distributions, install GCC manually if unsupported
- **wget/curl not found**: Install with your package manager for GitHub sync functionality
- **Compilation errors**: Ensure all source files are in the same directory and have proper permissions

---

## 📦 Testing

### Unit Tests
Run comprehensive unit tests to validate core functionalities:
```bash
# The unit tests are integrated into the main program
# They will test functions like read_data, save_file, and to_lowercase
```

### End-to-End Tests
Run E2E tests to validate complete workflows:
```bash
# E2E tests validate workflows all Function
# Include stress tests for multiple operations
```

---

## 📋 Version History

**Current Version:** `V1.5.0` *(October 9, 2025)*

---

### 🚀 Major Releases

#### **V1.5.0** - *October 9, 2025*

- **Cross-Platform Support**: Linux and macOS compatibility added
- **Enhanced Scripts**: Created comprehensive `run.sh` for Unix-like systems
- **Auto-Installation**: Smart dependency detection and package manager integration
- **Complete Testing Suite**: Fully functional E2E and Unit Tests
- **Bug Fixes**: All critical errors resolved

#### **V1.0.1** - *October 5, 2025*

- **Testing Framework**: Added comprehensive `unit_test.c`
- **E2E Testing**: Implemented `E2E.c` for complete workflow validation
- **GitHub Integration**: Enhanced `github_sync.c` functionality
- **UI Improvements**: Better error handling and user prompts
- **Build System**: Updated `run.bat` with dependency management

#### **V1.0.0** - *October 4, 2025* 🎉

- **Feature Complete**: All core functions implemented
- **CRUD Operations**: Add, Read, Update, Delete functionality
- **Data Summary**: Complete data analysis and reporting
- **Stable Release**: First production-ready version

---

### 🔧 Development Releases (Beta)

#### **V0.9.x Series** *(September 2025)*

- **V0.9.7**: Enhanced data entry and save functionality
- **V0.9.3**: Menu display improvements (V7) + file listing + UI enhancements
- **V0.9.1**: Menu display updates (V6)
- **V0.9.0**: GitHub synchronization + CSV handling improvements

#### **V0.8.x Series** *(September 2025)*

- **V0.8.0**: Auto file creation + path management + user-friendly interface

#### **V0.6.x Series** *(September 2025)*

- **V0.6.5**: Menu display improvements (V4)
- **V0.6.0**: CSV path configuration + menu display (V3)

#### **V0.5.x Series** *(September 2025)*

- **V0.5.5**: CSV handling improvements + menu display (V2)
- **V0.5.0**: Initial CSV reading functionality + file listing

#### **V0.1.0** *(September 4, 2025)*

- **Initial Release**: Basic menu system and core functions
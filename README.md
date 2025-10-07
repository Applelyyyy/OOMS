# 🛒 OOMS - Online Order Management System

OOMS is a command-line based Order Management System written in C. It helps manage customer orders and inventory using CSV files for data storage.

---

## 📁 Folder Structure

```plaintext
OOMS/
├── log/                        # log error Compile
    ├── Compile_error.txt       # log file
├── data/                       # Contains raw_data.csv and other data files
    ├── *.csv                   # csv folder
├── output/                     # Compiled Folder executable
    ├── main.exe                # Compiled executable file (Windows)
    ├── main                    # Compiled executable file (Linux)
├── main.c                      # Source
├── github_sync.h               # gitsync header 
├── github_sync.c               # gitsync source file
├── unit_test.c                 # Unit testing file
├── E2E.c                       # End-to-End testing file
├── README.md                   # Project documentation
├── run.bat                     # compile and run Program (Windows)
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

**Method 2: Manual Installation**

If auto installation fails, install dependencies manually:

1. **Install MinGW-w64** (for GCC compiler):

   - Download from: https://www.mingw-w64.org/downloads/
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

---

## 📋 Prerequisites

### Windows

- **GCC Compiler**: MinGW-w64 (auto-installed by `run.bat`)
- **PowerShell**: For GitHub sync functionality (usually pre-installed)
- **Administrator Rights**: May be required for automatic installation

---

## 💻 Platform Support

| Platform | Script | Executable | Auto Install |
|----------|--------|------------|--------------|
| Windows  | `run.bat` | `main.exe` | ✅ MinGW-w64 |

---

## 🔧 Troubleshooting

### Windows Issues

- **"gcc not found"**: Run `run.bat` as Administrator for auto-installation
- **PowerShell execution policy**: Run `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser`
- **Manual GCC install**: Download from mingw-w64.org

### Linux Issues

- **Permission denied**: Run `chmod +x run.sh` first
- **Package installation fails**: Ensure you have sudo privileges
- **Unsupported distribution**: Install gcc and wget manually using your package manager

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
# E2E tests validate workflows like adding, deleting, and updating records
# Include stress tests for multiple operations
```

---

## Version

Current Version: V1.0.1-10/05/2025

### Changelog

- **V1.0.1-10/05/2025:** 
  - Added `unit_test.c` for comprehensive unit testing
  - Added `E2E.c` for end-to-end testing workflows
  - Improved GitHub sync functionality in `github_sync.c`
  - Enhanced error handling and UI prompts
  - Updated `run.bat` and `run.sh` for better dependency management
- **V1.0.0-10/04/2025:** Complete all the function and remove data, update data, summary data and Version 1.0.0!
- **V0.9.7-beta-09/25/2025:** new add data function and save
- **V0.9.3-beta-09/24/2025:** update menu display-V7 and Refactor main program structure and add file listing functionality fix bug and better ui in change csv file
- **V0.9.1-beta-09/20/2025:** update menu display-V6
- **V0.9.0-beta-09/18/2025:** GitHub sync functionality; update CSV handling and compilation script
- **V0.8.0-beta-09/18/2025:** Startup Create File and UI Change Path and User-makefriendly
- **V0.6.5-beta-09/16/2025:** update menu display-V4
- **V0.6.0-beta-09/16/2025:** Add change path CSV and logic; update menu display-V3
- **V0.5.5-beta-09/06/2025:** improve CSV handling and menu display-V2
- **V0.5.0-beta-09/06/2025:** Add CSV reading functionality and list CSV
- **V0.1.0-beta-09/04/2025:** Menu and function
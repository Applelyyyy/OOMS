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
├── README.md                   # Project documentation
├── run.bat                     # compile and run Program (Windows)
└── run.sh                      # compile and run Program (Linux)
```

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
   gcc main.c github_sync.c -o output/main.exe
   cd output
   main.exe
   ```

#### 🐧 Linux (Automatic Installation)

**Method 1: Auto Compile with Dependency Check (Recommended)**

Make the script executable and run:
```bash
chmod +x run.sh
./run.sh
```

The script will automatically:
- Check if `gcc` is installed and prompt to install if missing
- Check if `wget` is installed and offer automatic installation
- Detect your Linux distribution and use appropriate package manager
- Support for: Ubuntu/Debian, Red Hat/CentOS, Fedora, Arch, openSUSE
- Create necessary directories
- Compile and run the program

**Method 2: Manual Installation**

If auto installation fails, install dependencies manually:

1. **Install GCC**:
   ```bash
   # Ubuntu/Debian
   sudo apt-get update && sudo apt-get install build-essential
   
   # Red Hat/CentOS
   sudo yum groupinstall "Development Tools"
   
   # Fedora
   sudo dnf groupinstall "Development Tools"
   
   # Arch
   sudo pacman -S base-devel
   
   # openSUSE
   sudo zypper install -t pattern devel_basis
   ```

2. **Install wget** (for GitHub sync):
   ```bash
   # Ubuntu/Debian
   sudo apt-get install wget
   
   # Red Hat/CentOS
   sudo yum install wget
   
   # Fedora
   sudo dnf install wget
   
   # Arch
   sudo pacman -S wget
   
   # openSUSE
   sudo zypper install wget
   ```

3. **Compile manually**:
   ```bash
   gcc main.c github_sync.c -o output/main
   cd output
   ./main
   ```

---

## 📋 Prerequisites

### Windows
- **GCC Compiler**: MinGW-w64 (auto-installed by `run.bat`)
- **PowerShell**: For GitHub sync functionality (usually pre-installed)
- **Administrator Rights**: May be required for automatic installation

### Linux
- **GCC Compiler**: build-essential or Development Tools (auto-installed by `run.sh`)
- **wget**: For GitHub sync functionality (auto-installed by `run.sh`)
- **sudo privileges**: Required for automatic package installation

---

## 💻 Platform Support

| Platform | Script | Executable | Auto Install |
|----------|--------|------------|--------------|
| Windows  | `run.bat` | `main.exe` | ✅ MinGW-w64 |
| Linux    | `run.sh` | `main` | ✅ GCC + wget |

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

## Version

Current Version: V1.0.0-10/04/2025

### Changelog

- **V0.1.0-beta-09/04/2025:** Menu and function
- **V0.5.0-beta-09/06/2025:** Add CSV reading functionality and list CSV
- **V0.5.5-beta-09/06/2025:** improve CSV handling and menu display-V2
- **V0.6.0-beta-09/16/2025:** Add change path CSV and logic; update menu display-V3
- **V0.6.5-beta-09/16/2025:** update menu display-V4
- **V0.8.0-beta-09/18/2025:** Startup Create File and UI Change Path and User-makefriendly
- **V0.9.0-beta-09/18/2025:** GitHub sync functionality; update CSV handling and compilation script
- **V0.9.1-beta-09/20/2025:** update menu display-V6
- **V0.9.3-beta-09/24/2025:** update menu display-V7 and Refactor main program structure and add file listing functionality fix bug and better ui in change csv file
- **V0.9.7-beta-09/25/2025:** new add data function and save
- **V1.0.0-10/04/2025:** Complete all the function and remove data, update data, summary data and Version 1.0.0!!
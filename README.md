# 🛒 OOMS - Online Order Management System

OOMS is a command-line based Order Management System written in C. It helps manage customer orders and inventory using CSV files for data storage.

---

## 📁 Folder Structure

```plaintext
OOMS/
├── log/            # log error Compile
├── data/           # Contains raw_data.csv and other data files
├── output/         # Compiled executable
├── main.c          # Source
├── github_sync.h   # gitsync header 
├── github_sync.c   # gitsync source file
└── README.md       # Project documentation
```

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/Applelyyyy/OOMS.git
cd OOMS
```

### 2. Compile the Program

 There are 2 methods to Compile

- Use auto Compile

```bash
run.exe
```

- manually Compile

### 2.1 Manually  Compile

Ensure the `data/` folder contains `raw_data.csv`.  
If missing, the program will automatically download it from a predefined URL.

### 2.2. Compile the Program

```bash
gcc main.c github_sync.c -o output/main.exe
```

### 2.3. Got to Folder output

```bash
cd output
```

### 2.4. Run the Program

```bash
.\main.exe
```

---

## Version

Current Version: V0.9.0-beta-09/18/2025

### Changelog

- **V0.1.0-beta-09/04/2025:** Menu and function
- **V0.5.0-beta-09/06/2025:** Add CSV reading functionality and list CSV
- **V0.5.5-beta-09/06/2025:** improve CSV handling and menu display-V2
- **V0.6.0-beta-09/16/2025:** Add change path CSV and logic; update menu display-V3
- **V0.6.5-beta-09/16/2025:** update menu display-V4
- **V0.8.0-beta-09/18/2025:** Startup Create File and UI Change Path and User-makefriendly
- **V0.9.0-beta-09/18/2025:** GitHub sync functionality; update CSV handling and compilation script

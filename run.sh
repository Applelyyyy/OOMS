#!/bin/bash


RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${CYAN}=====================================${NC}"
echo -e "${CYAN}    OOMS - Order Management System${NC}"
echo -e "${CYAN}=====================================${NC}"
echo


command_exists() {
    command -v "$1" >/dev/null 2>&1
}


install_gcc() {
    echo -e "${YELLOW}Attempting to install GCC...${NC}"
    

    if command_exists apt-get; then
        echo -e "${BLUE}Detected Debian/Ubuntu system${NC}"
        echo -e "${YELLOW}Installing build-essential...${NC}"
        sudo apt-get update && sudo apt-get install -y build-essential
    elif command_exists yum; then
        echo -e "${BLUE}Detected Red Hat/CentOS system${NC}"
        echo -e "${YELLOW}Installing gcc and development tools...${NC}"
        sudo yum groupinstall -y "Development Tools"
    elif command_exists dnf; then
        echo -e "${BLUE}Detected Fedora system${NC}"
        echo -e "${YELLOW}Installing gcc and development tools...${NC}"
        sudo dnf groupinstall -y "Development Tools and Libraries"
    elif command_exists pacman; then
        echo -e "${BLUE}Detected Arch Linux system${NC}"
        echo -e "${YELLOW}Installing base-devel...${NC}"
        sudo pacman -S --noconfirm base-devel
    elif command_exists zypper; then
        echo -e "${BLUE}Detected openSUSE system${NC}"
        echo -e "${YELLOW}Installing development pattern...${NC}"
        sudo zypper install -y -t pattern devel_basis
    elif command_exists apk; then
        echo -e "${BLUE}Detected Alpine Linux system${NC}"
        echo -e "${YELLOW}Installing build-base...${NC}"
        sudo apk add build-base
    else
        echo -e "${RED}❌ Unknown Linux distribution. Please install GCC manually.${NC}"
        echo -e "${YELLOW}Common commands:${NC}"
        echo -e "  Debian/Ubuntu: sudo apt-get install build-essential"
        echo -e "  Red Hat/CentOS: sudo yum groupinstall 'Development Tools'"
        echo -e "  Fedora: sudo dnf groupinstall 'Development Tools and Libraries'"
        echo -e "  Arch: sudo pacman -S base-devel"
        echo -e "  openSUSE: sudo zypper install -t pattern devel_basis"
        return 1
    fi
    
    return $?
}


echo -e "${BLUE}Checking for GCC compiler...${NC}"
if command_exists gcc; then
    echo -e "${GREEN}✓ GCC found!${NC}"
    gcc --version | head -1
else
    echo -e "${RED}❌ GCC not found.${NC}"
    echo
    echo -e "${YELLOW}GCC is required to compile this program.${NC}"
    read -p "Would you like to install GCC automatically? (y/n): " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        if install_gcc; then
            echo -e "${GREEN}✓ GCC installed successfully!${NC}"
            gcc --version | head -1
        else
            echo -e "${RED}❌ Failed to install GCC. Please install manually.${NC}"
            exit 1
        fi
    else
        echo -e "${YELLOW}Please install GCC and run this script again.${NC}"
        exit 1
    fi
fi


echo -e "${BLUE}Checking for download tools (wget/curl)...${NC}"
if command_exists wget; then
    echo -e "${GREEN}✓ wget found!${NC}"
elif command_exists curl; then
    echo -e "${GREEN}✓ curl found!${NC}"
else
    echo -e "${YELLOW}⚠ Neither wget nor curl found. GitHub sync may not work.${NC}"
    echo -e "${YELLOW}Consider installing wget or curl for full functionality.${NC}"
fi

echo


echo -e "${BLUE}Creating directories...${NC}"
for dir in output log data; do
    if [ ! -d "$dir" ]; then
        mkdir -p "$dir"
        echo -e "${GREEN}Created $dir directory${NC}"
    else
        echo -e "${CYAN}$dir directory already exists${NC}"
    fi
done

echo

gram...${NC}"
echo -e "${YELLOW}gcc main.c github_sync.c unit_test.c E2E.c -o output/main${NC}"

if gcc main.c github_sync.c unit_test.c E2E.c -o output/main 2> log/Compile_error.txt; then
    echo -e "${GREEN}✓ Compilation successful!${NC}"
    

    if [ -f "output/main" ]; then
        echo -e "${GREEN}✓ Executable created: output/main${NC}"

        chmod +x output/main
        
        echo
        echo -e "${CYAN}=====================================${NC}"
        echo -e "${GREEN}    Running OOMS Program...${NC}"
        echo -e "${CYAN}=====================================${NC}"
        echo
        

        cd output && ./main
        
    else
        echo -e "${RED}❌ Executable not found after compilation${NC}"
        exit 1
    fi
    
else
    echo -e "${RED}❌ Compilation failed!${NC}"
    echo
    echo -e "${YELLOW}Error details:${NC}"
    if [ -f "log/Compile_error.txt" ]; then
        cat log/Compile_error.txt
    fi
    echo
    echo -e "${YELLOW}Common solutions:${NC}"
    echo -e "  - Make sure all .c files exist in the current directory"
    echo -e "  - Check for syntax errors in the source code"
    echo -e "  - Ensure GCC is properly installed"
    echo -e "  - Check file permissions"
    echo
    exit 1
fi

#!/bin/bash

echo "Compiling program..."

# Check for wget
if ! command -v wget &> /dev/null; then
    echo "⚠️  wget is not installed. This is required for GitHub sync functionality."
    echo ""
    read -p "Do you want to install wget automatically? (Y/n): " -n 1 -r
    echo
    
    if [[ $REPLY =~ ^[Nn]$ ]]; then
        echo "⚠️  Continuing without wget (GitHub sync may not work)..."
    else
        echo "🔧 Installing wget..."
        
        if command -v apt-get &> /dev/null; then
            sudo apt-get update && sudo apt-get install -y wget
        elif command -v yum &> /dev/null; then
            sudo yum install -y wget
        elif command -v dnf &> /dev/null; then
            sudo dnf install -y wget
        elif command -v pacman &> /dev/null; then
            sudo pacman -S --noconfirm wget
        elif command -v zypper &> /dev/null; then
            sudo zypper install -y wget
        else
            echo "❌ Could not detect package manager. Please install wget manually:"
            echo "  Ubuntu/Debian: sudo apt-get install wget"
            echo "  Red Hat/CentOS: sudo yum install wget"
            echo "  Fedora: sudo dnf install wget"
            echo "  Arch: sudo pacman -S wget"
            echo "  openSUSE: sudo zypper install wget"
            read -p "Press any key to continue..."
        fi
        
        if command -v wget &> /dev/null; then
            echo "✅ wget installed successfully!"
        else
            echo "❌ wget installation failed. GitHub sync may not work."
        fi
    fi
fi

# Create directories
if [ ! -d "output" ]; then
    mkdir output
fi

if [ ! -d "log" ]; then
    mkdir log
fi

# Compile with debug symbols and additional warnings
echo "🔧 Compiling with debug information..."
gcc -g -Wall -Wextra -fsanitize=address main.c github_sync.c unit_test.c E2E.c -o output/main 2> log/Compile_error.txt

# Check compilation
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed. Error in log folder:"
    cat log/Compile_error.txt
    read -p "Press any key to continue..."
    exit 1
fi

echo "✅ Compilation successful. Running program..."

# Run with better error handling and debugging
echo "🚀 Starting OOMS program..."
cd output

# Check if required CSV file exists
if [ ! -f "../raw_data.csv" ]; then
    echo "⚠️  Warning: raw_data.csv not found in parent directory"
fi

# Run the program and capture any runtime errors
./main 2>&1 | tee ../log/runtime.log

# Check exit status
if [ $? -ne 0 ]; then
    echo "❌ Program exited with error. Check log/runtime.log for details"
    echo "💡 Try running with gdb for more debugging info:"
    echo "   cd output && gdb ./main"
fi

cd ..
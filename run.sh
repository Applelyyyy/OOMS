#!/bin/bash

echo "🔧 Compiling program..."

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="Linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macOS"
elif [[ "$OSTYPE" == "cygwin" || "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
    OS="Windows"
else
    OS="Unknown"
fi

echo "📊 Detected OS: $OS"

# Check for wget/curl on Linux/macOS
if [[ "$OS" == "Linux" || "$OS" == "macOS" ]]; then
    if ! command -v wget &> /dev/null && ! command -v curl &> /dev/null; then
        echo "⚠️  Neither wget nor curl is installed. This is required for GitHub sync functionality."
        echo ""
        read -p "Do you want to install wget automatically? (Y/n): " -n 1 -r
        echo
        
        if [[ ! $REPLY =~ ^[Nn]$ ]]; then
            echo "🔧 Installing wget..."
            
            if command -v apt-get &> /dev/null; then
                sudo apt-get update && sudo apt-get install -y wget curl
            elif command -v yum &> /dev/null; then
                sudo yum install -y wget curl
            elif command -v dnf &> /dev/null; then
                sudo dnf install -y wget curl
            elif command -v pacman &> /dev/null; then
                sudo pacman -S --noconfirm wget curl
            elif command -v zypper &> /dev/null; then
                sudo zypper install -y wget curl
            elif command -v brew &> /dev/null; then
                brew install wget curl
            else
                echo "❌ Could not detect package manager. Please install wget or curl manually:"
                echo "  Ubuntu/Debian: sudo apt-get install wget curl"
                echo "  Red Hat/CentOS: sudo yum install wget curl"
                echo "  Fedora: sudo dnf install wget curl"
                echo "  Arch: sudo pacman -S wget curl"
                echo "  openSUSE: sudo zypper install wget curl"
                echo "  macOS: brew install wget curl"
                read -p "Press any key to continue..."
            fi
            
            if command -v wget &> /dev/null || command -v curl &> /dev/null; then
                echo "✅ Download tools installed successfully!"
            else
                echo "❌ Installation failed. GitHub sync may not work."
            fi
        fi
    fi
fi

# Create directories
for dir in "output" "log" "data"; do
    if [ ! -d "$dir" ]; then
        mkdir -p "$dir"
        echo "📁 Created directory: $dir"
    fi
done

# Compile with appropriate flags for each OS
echo "🔧 Compiling with debug information..."

if [[ "$OS" == "Windows" ]]; then
    # Windows compilation (remove fsanitize for better compatibility)
    gcc -g -Wall -Wextra -std=c99 main.c github_sync.c unit_test.c E2E.c -o output/main.exe 2> log/Compile_error.txt
    EXECUTABLE="./main.exe"
elif [[ "$OS" == "Linux" ]]; then
    # Linux compilation with sanitizers
    gcc -g -Wall -Wextra -std=c99 -fsanitize=address -fsanitize=undefined main.c github_sync.c unit_test.c E2E.c -o output/main 2> log/Compile_error.txt
    EXECUTABLE="./main"
elif [[ "$OS" == "macOS" ]]; then
    # macOS compilation
    gcc -g -Wall -Wextra -std=c99 main.c github_sync.c unit_test.c E2E.c -o output/main 2> log/Compile_error.txt
    EXECUTABLE="./main"
else
    # Default compilation
    gcc -g -Wall -Wextra -std=c99 main.c github_sync.c unit_test.c E2E.c -o output/main 2> log/Compile_error.txt
    EXECUTABLE="./main"
fi

# Check compilation
if [ $? -ne 0 ]; then
    echo "❌ Compilation failed. Check log/Compile_error.txt for details:"
    echo "----------------------------------------"
    cat log/Compile_error.txt
    echo "----------------------------------------"
    read -p "Press any key to continue..."
    exit 1
fi

echo "✅ Compilation successful!"

# Set executable permissions on Linux/macOS
if [[ "$OS" != "Windows" ]]; then
    chmod +x output/main
fi

# Check if required CSV file exists
if [ ! -f "../data/raw_data.csv" ]; then
    echo "⚠️  Warning: raw_data.csv not found in data directory"
    echo "📥 The program will attempt to download it from GitHub..."
fi

echo "🚀 Starting OOMS program..."
cd output

# Run with error capture
if [[ "$OS" == "Linux" ]]; then
    # Linux: Use timeout to prevent hanging and better error reporting
    timeout 300s $EXECUTABLE 2>&1 | tee ../log/runtime.log
    EXIT_CODE=$?
    
    if [ $EXIT_CODE -eq 124 ]; then
        echo "⏰ Program timed out after 5 minutes"
    elif [ $EXIT_CODE -ne 0 ]; then
        echo "❌ Program exited with error code: $EXIT_CODE"
        echo "📋 Check log/runtime.log for details"
        echo "💡 Debug with: cd output && gdb $EXECUTABLE"
    fi
else
    # Other systems
    $EXECUTABLE 2>&1 | tee ../log/runtime.log
    EXIT_CODE=$?
    
    if [ $EXIT_CODE -ne 0 ]; then
        echo "❌ Program exited with error code: $EXIT_CODE"
        echo "📋 Check log/runtime.log for details"
    fi
fi

cd ..
echo "🏁 Program execution completed."
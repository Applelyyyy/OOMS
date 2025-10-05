#!/bin/bash


echo "Compiling program..."


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


if [ ! -d "output" ]; then
    mkdir output
fi

if [ ! -d "log" ]; then
    mkdir log
fi


gcc main.c github_sync.c unit_test.c E2E.c -o output/main 2> log/Compile_error.txt


if [ $? -ne 0 ]; then
    echo "❌ Compilation failed. error in log Folder:"
    cat log/Compile_error.txt
    read -p "Press any key to continue..."
    exit 1
fi

echo "Compilation successful. Running program..."
cd output && ./main
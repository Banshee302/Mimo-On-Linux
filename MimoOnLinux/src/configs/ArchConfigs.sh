#!/bin/bash

# Step 1: Show menu and get choice
CHOICE=$(whiptail --title "Architecture Config" \
  --radiolist "Choose your target architecture:" 15 50 4 \
  "ARM" "ARM-based processors" ON \
  "x86" "Intel/AMD x86 processors" OFF \
  "PowerPC" "PowerPC architecture" OFF \
  3>&1 1>&2 2>&3)

# Step 2: Check if user canceled
if [ $? -ne 0 ]; then
    echo "Cancelled."
    exit 1
fi

# Step 3: Write choice to file
echo "$CHOICE" > arch_config

# Step 4: Compile based on config
if [ -f arch_config ]; then
    ARCH=$(cat arch_config)
    case "$ARCH" in
        ARM)
            echo "Compiling for ARM..."
            # Insert ARM compilation command here
            ;;
        x86)
            echo "Compiling for x86..."
            # Insert x86 compilation command here
            ;;
        PowerPC)
            echo "Compiling for PowerPC..."
            # Insert PowerPC compilation command here
            ;;
        *)
            echo "Unknown architecture: $ARCH"
            ;;
    esac
else
    echo "No config file found."
    exit 1
fi

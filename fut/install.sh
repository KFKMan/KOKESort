#!/bin/bash

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    if [[ -f /etc/debian_version ]]; then
        echo "Installing for Debian/Ubuntu..."
        sudo dpkg -i "latest_release.deb"
        sudo apt-get install -f  #Installing dependencies
        echo "Installed"
    elif [[ -f /etc/fedora-release ]]; then
        echo "Installing for Fedora..."
        sudo rpm -ivh "latest_release.rpm"
        echo "Installed"
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    # MacOS
    echo "Installing for MacOS..."
    brew install fusionlanguage/fut/fut
    echo "Installed"
else
    echo "Unsupported Platform"
fi
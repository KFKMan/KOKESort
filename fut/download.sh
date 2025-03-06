#!/bin/bash

USERNAME="fusionlanguage"
REPO="fut"

RELEASE_INFO=$(curl -s https://api.github.com/repos/$USERNAME/$REPO/releases/latest)

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    if [[ -f /etc/debian_version ]]; then
        # Debian/Ubuntu
        URL=$(echo "$RELEASE_INFO" | jq -r '.assets[] | select(.name | test("deb$")) | .browser_download_url')
        echo "Downloading for Debian/Ubuntu: $URL"
        curl -L -o "latest_release.deb" "$URL"
    elif [[ -f /etc/fedora-release ]]; then
        # Fedora
        URL=$(echo "$RELEASE_INFO" | jq -r '.assets[] | select(.name | test("rpm$")) | .browser_download_url')
        echo "Downloading for Fedora: $URL"
        curl -L -o "latest_release.rpm" "$URL"
    fi
fi
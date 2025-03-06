#!/bin/bash

# WHL file directory
whl_dir="./dist"

# Finding latest WHL file
latest_file=$(ls -t "$whl_dir"/*.whl | head -n 1)

# Installing WHL file if found
if [ -n "$latest_file" ]; then
    echo "Latest version: $latest_file"
    pip install --user --force-reinstall "$latest_file"
else
    echo "WHL File not found"
fi

@echo off

if exist "build" (
    echo "Build Folder Found, Deleting..."
    rmdir /s /q "build"
    echo "Build Folder Deleted"
)

call build.bat
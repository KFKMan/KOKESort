@echo off
setlocal

REM === Settings ===
set VCPKG_DIR=.\vcpkg
set VCPKG_EXE=%VCPKG_DIR%\vcpkg.exe

REM === VCPKG is Exists? ===
if not exist "%VCPKG_DIR%" (
    echo [INFO] VCPKG not exists, installing...
    git clone https://github.com/microsoft/vcpkg %VCPKG_DIR%
)

REM === vcpkg bootstrap ===
if not exist "%VCPKG_EXE%" (
    echo [INFO] Compiling VCPKG...
    pushd %VCPKG_DIR%
    call bootstrap-vcpkg.bat
    popd
)

REM === Qt kurulumu ===
echo [INFO] Qt installing...
pushd %VCPKG_DIR%
call vcpkg install qtbase
call vcpkg install qttools
:: We don't need this, call vcpkg integrate install
popd

echo.
echo [OK] Qt Installed.
pause
endlocal

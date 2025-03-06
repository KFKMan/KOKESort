@echo off
setlocal enabledelayedexpansion

set USERNAME=fusionlanguage
set REPO=fut

for /f "delims=" %%i in ('curl -s https://api.github.com/repos/%USERNAME%/%REPO%/releases/latest ^| findstr "browser_download_url"') do (
    set URL=%%i
    set URL=!URL:~41,-2!
)

for /f "delims=" %%i in ('curl -s https://api.github.com/repos/%USERNAME%/%REPO%/releases/latest ^| findstr "browser_download_url" ^| findstr "msi"') do (
    set MSI_URL=%%i
    set MSI_URL=!MSI_URL:~41,-2!
)
echo Downloading: !MSI_URL!
curl -L -o "latest_release.msi" !MSI_URL!
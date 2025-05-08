@echo off
setlocal enabledelayedexpansion

:: WHL dosyasının bulunduğu dizini belirleyin
set "whl_dir=./dist"

:: WHL dosyalarını sıralayıp en son sürümü bulun
set latest_file=
for /f "delims=" %%i in ('dir /b /o:-d "%whl_dir%\*.whl"') do (
    set latest_file=%%i
    goto :found
)

:found
if defined latest_file (
    echo Latest version: %latest_file%
    pip install --force-reinstall "%whl_dir%\%latest_file%"
) else (
    echo WHL File not found
)
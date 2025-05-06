@echo off
REM ─────────────────────────────────────────────────────
REM 1) Eski derleme klasörlerini temizle
IF EXIST build rmdir /s /q build
IF EXIST dist  rmdir /s /q dist
FOR /D %%d IN (*.egg-info) DO rmdir /s /q "%%d"

REM 2) Kaynak ve wheel paketlerini oluştur
python setup.py --verbose sdist bdist_wheel
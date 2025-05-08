@echo off
REM ─────────────────────────────────────────────────────
REM 1) Eski derleme klasörlerini temizle
IF EXIST build rmdir /s /q build
IF EXIST dist  rmdir /s /q dist
IF EXIST KOKESort  rmdir /s /q KOKESort
for /d /r %%d in (*egg-info*) do (
    echo Deleting %%d
    rmdir /s /q "%%d"
)

REM 2) Kaynak ve wheel paketlerini oluştur
python -m pip install nanobind
python -m pip install build
REM python -m build
set DEBUG=1
python setup.py build_ext --inplace
python setup.py bdist_wheel

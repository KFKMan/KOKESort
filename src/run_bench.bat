set SOURCE_FILE=.\benchmark\testdata.txt
set TARGET_FILE=.\testdata.txt

set SOURCE_DIR=.\build\Release
set TARGET_DIR=.\build\benchmarkr2\Release

echo Copying %SOURCE_FILE% to %TARGET_FILE%...
if not exist "%SOURCE_FILE%" (
    echo Error: Source file does not exist!
    exit /b 1
)
copy /Y "%SOURCE_FILE%" "%TARGET_FILE%"

echo Copying files from %SOURCE_DIR% to %TARGET_DIR%...
if not exist "%TARGET_DIR%" mkdir "%TARGET_DIR%"
xcopy /E /Y "%SOURCE_DIR%\*" "%TARGET_DIR%\"
echo Files copied successfully from %SOURCE_DIR% to %TARGET_DIR%

.\build\benchmarkr2\Release\KOKESortBenchmarkR2.exe
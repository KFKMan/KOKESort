set GENERATOR_DIR=.\generator\gen\
set GENERATOR_TARGET_DIR=.\benchres\

echo Copying files from %GENERATOR_DIR% to %GENERATOR_TARGET_DIR%...
if not exist "%GENERATOR_TARGET_DIR%" mkdir "%GENERATOR_TARGET_DIR%"
xcopy /E /Y "%GENERATOR_DIR%\*" "%GENERATOR_TARGET_DIR%\"
echo Files copied successfully from %GENERATOR_DIR% to %GENERATOR_TARGET_DIR%

set SOURCE_DIR=.\build\Release
set TARGET_DIR=.\build\benchmarkr2\Release

echo Copying files from %SOURCE_DIR% to %TARGET_DIR%...
if not exist "%TARGET_DIR%" mkdir "%TARGET_DIR%"
xcopy /E /Y "%SOURCE_DIR%\*" "%TARGET_DIR%\"
echo Files copied successfully from %SOURCE_DIR% to %TARGET_DIR%

set FUNCTIONS_FILE=.\functions.txt
set FUNCTIONS_TARGET_FILE=.\benchres\functions.txt

echo Copying %FUNCTIONS_FILE% to %FUNCTIONS_TARGET_FILE%...
if not exist "%FUNCTIONS_FILE%" (
    echo Error: Source file does not exist!
    exit /b 1
)
copy /Y "%FUNCTIONS_FILE%" "%FUNCTIONS_TARGET_FILE%"

cd .\benchres\



..\build\benchmarkr2\Release\KOKESortBenchmarkR2.exe
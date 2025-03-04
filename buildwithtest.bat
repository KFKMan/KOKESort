mkdir build
cd .\build\
cmake ..\
cmake --build .\ --config Debug
ctest --verbose --output-log ..\ctest_output.txt --build-config Debug
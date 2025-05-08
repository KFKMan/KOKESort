mkdir build
cd .\build\
cmake ..\ -DUSE_TESTS=ON
cmake --build .\ --config Debug
ctest --verbose --output-log ..\ctest_output.txt --build-config Debug
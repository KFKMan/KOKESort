mkdir build
cd .\build\
cmake -DCMAKE_BUILD_TYPE=Release -DUSE_BENCHMARKS_R3=ON ..\
cmake --build .\ --config Release
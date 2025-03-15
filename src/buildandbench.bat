mkdir build
cd .\build\
cmake -DCMAKE_BUILD_TYPE=Release -DUSE_BENCHMARKS=ON -DUSE_BENCHMARKS_R2=ON ..\
cmake --build .\ --config Release
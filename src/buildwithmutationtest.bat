mkdir build
cd build

if exist tests\mutated rmdir /s /q tests\mutated

cmake -DUSE_MUTATION_TESTS=ON ..
cmake --build . --target run_mutator_with_params_external --config Debug
cmake -DUSE_MUTATION_TESTS=ON ..
cmake --build . --config Debug
ctest --verbose --output-log ../ctest_output.txt --build-config Debug
git clone https://github.com/ninja-build/ninja.git
Set-Location ninja
git checkout release

cmake -B build-cmake
cmake --build build-cmake

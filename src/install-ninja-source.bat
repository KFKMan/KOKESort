git clone https://github.com/ninja-build/ninja.git
cd ninja
git checkout release

cmake -Bbuild-cmake
cmake --build build-cmake
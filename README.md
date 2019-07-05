## Testing

For exercises that have unit tests, the project must be built with the approprate test cpp file. This can be done by using `cmake_tests` instead of `cmake`. For example, from the build directory:
```
cmake_tests ..
make
./CppND-Route-Planning-Project -f ./map.osm
```

Those commands will build the code with the appropriate tests exercise. The tests can then be run from the `build` directory as follows:
```
../bin/test
```
Not all exercises have tests. The table is given below with the complete list of exercises with tests for reference:

| Exercise Name               | 
|-----------------------------|
| The RouteModel Class        |
| The Node Class              |
| Create RouteModel Nodes     |
| Write the Distance Function |
| Create Road to Node Hashmap |
| Write FindNeighbors         |
| Find the Closest Node       |
| Write the A* Search Stub    |
| Finish A* Search            |

This program relay on IO2D Library:
```
git clone --recurse-submodules https://github.com/cpp-io2d/P0267_RefImpl
cd P0267_RefImpl
mkdir Debug
cd Debug
cmake --config Debug "-DCMAKE_BUILD_TYPE=Debug" ..
cmake --build .
make
make install
```
System Library requirement:
```
Install GCC:  apt install build-essential
Install Cairo:  apt install libcairo2-dev
Install graphicsmagick:  apt install libgraphicsmagick1-dev
Install libpng:  apt install libpng-dev libcurl4-gnutls-dev
```
##Cmake requirement>=3.13
libcurl4-gnutls-dev is used for compile cmake to use https download sth.
```
https://gitlab.kitware.com/cmake/cmake
./bootstrap && make && sudo make install
```

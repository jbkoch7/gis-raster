# gis-raster
A conceptual C++ GIS Raster class.

# Build
There is currently a simple test application that can be built with CMake. <br />
It provides a bare-bones use case. <br />
Unit tests may be added in the future if time permits.

## Linux
```
mkdir <build-dir>
cd <build-dir>
cmake .. -DCMAKE_INSTALL_PREFIX=<install-dir>
make
make install
```

## Windows
```
mkdir <build-dir>
cd <build-dir>
cmake .. -G "Visual Studio 15 2017 Win64" -DCMAKE_INSTALL_PREFIX=<install-dir>
open gis-raster.sln or use msbuild or use devenv.com
```

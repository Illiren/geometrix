# Geometrix
A header-only C++ mathematical libary, containing geometry, linear algebra and 
trigonometry functions
## Features
* TODO
## Description
* TODO
## Dependencies
1. A C++ compiler that supports C++20 standart.
The following compilers should work:

  * [gcc 10+](https://gcc.gnu.org/)

  * [clang 10+](https://clang.llvm.org/)

2. [CMake 3.12+](https://cmake.org/)
## Usage
* TODO
### Build
Target platforms are Windows and Linux. You can build main target via CMake
From build directory:
```
 $ mkdir build
 $ cd build
 $ cmake %PATH to the main CMakeLists.txt% -DCMAKE_CXX_COMPILER:STRING=/usr/bin/clang++-11 -DCMAKE_BUILD_TYPE:String=Release
 $ make
``` 
### Install
After done building (from build directory) rebuild specifying install path:
```
 $ cmake %PATH to the main CMakeLists.txt% -DCMAKE_INSTALL_PREFIX="PATH"
 $ cmake --build . --config Release --target install
```
Then to link the built library to your project with cmake use the following cmake lines:
```
find_package(geometrix CONFIG REQUIRED)
target_link_libraries(example_project geometrix::geometrix)
```

## Testing
* Add Description of testing process


### Building the tests
Example:
```
 $ mkdir build
 $ cd build
 $ cmake %PATH to the main CMakeLists.txt% -DCMAKE_CXX_COMPILER:STRING=/usr/bin/clang++-11
 $ make
```

### Running the tests
To run built tests:
```
 $ make test
```

## TODO: 
* Add Unit tests for all modules
* TODO add TODOs

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

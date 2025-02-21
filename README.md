# MatrixCPP

Educational project from K. Vladimirov's course at MIPT.
C++ general matrix implementation using my own vector written almost from scratch.

## Dependencies

Ensure that you installed with your package manager the following:

* Git
* CMake
* C++ compiler + libstdc++ with support of at least C++17
* Python 3
* `pip`

Optional:

* Doxygen (to generate docs)

## Build

After installing dependencies listed above, follow these steps:

1. Cloning.

```sh
git clone https://github.com/victorbaldin56/MatrixCPP.git
cd MatrixCPP
```

1. Configuring build.

```sh
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

1. Building.

```sh
cmake --build . -j
```

## Integration into existing CMake project

If you want to use MatrixCPP as just header-only library
you, write in your `CMakeLists.txt` the following:

```cmake
...
add_subdirectory(MatrixCPP)
target_link_libraries(${YOUR_TARGET} matrix)
...
```

## Usage

To view docs for source code, run

```sh
doxygen
```

Also there is an example program called `driver` computing determinant of
input matrix. For instance:

```sh
$ ./build/driver/driver
2 1 0 0 1
1
```

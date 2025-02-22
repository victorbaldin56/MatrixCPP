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

1. Clone:

   ```sh
   git clone https://github.com/victorbaldin56/MatrixCPP.git
   cd MatrixCPP
   ```

1. Create virtual environment for Python:

   ```sh
   python3 -m venv .venv
   source .venv/bin/activate
   ```

1. Install Conan:

   ```sh
   pip3 install conan
   conan profile detect --force
   ```

1. Install Conan requirements and toolchain:

   ```sh
   conan install . --output-folder=build --build=missing
   ```

1. Configure CMake:

   ```sh
   cmake .. --preset conan-release
   ```

1. Build:

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

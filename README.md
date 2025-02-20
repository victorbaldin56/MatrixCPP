# MatrixCPP
Educational project from K. Vladimirov's course.
C++ general matrix implementation using my own vector written almost from scratch.

# Dependencies
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
2. Creating virtual environment and installing Conan with additional packages.
```sh
python3 -m venv .venv;
source .venv/bin/activate;
pip install -r requirements.txt;
conan profile detect --force;
conan install . -s build_type=Release --output-folder=build --build=missing
```

3. Configuring build.
```sh
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

4. Building.
```sh
cmake --build . -j
```

## Usage
To view docs for source code, run
```sh
doxygen
```

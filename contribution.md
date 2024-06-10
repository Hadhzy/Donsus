- # check contrib folder
There are a lot of useful llvm examples there - helping you to understand our compiler better.

- To run the project in DEBUG mode so the TEST API runs build the project in the following way:
```
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=DEBUG ..
make

```
# Building Donsus From Source

## Windows
### MSVC
To compile Donsus on Windows a few things need to be done:
 - Install llvm
 - Download Visual Studio
 - Release build
 - Linking
 - Cmake Generator

#### llvm:
LLVM can be both installed from the [source](https://llvm.org/docs/GettingStarted.html) or through a [package manager](https://learn.microsoft.com/en-us/vcpkg/users/examples/selecting-llvm-features).
Please read the documentation properly. 
Normally the path of the installation should be added to PATH as an environment variable.
E.g #  LLVM_ROOT_DIR       - The root directory of the LLVM installation.
                        llvm-config is searched for in ${LLVM_ROOT_DIR}/bin.

Find out more [here](https://github.com/ldc-developers/ldc/blob/master/cmake/Modules/FindLLVM.cmake).

#### Visual Studio
Visual studio can be downloaded from [here](https://visualstudio.microsoft.com/downloads/).
Make sure that it's properly added to the PATH and there are no other compilers in the PATH variable.
If other compilers found - cmake won't be able to pick the correct compiler. Generally speaking, when one decides to 
build Donsus should always look for the print message that is displayed when configuring cmake. This message tells what compiler
is picked by cmake. 

#### Release build
As of now Donsus can be only built in Release mode on Windows. This will be resolved in the future.
`cmake -DCMAKE_BUILD_TYPE=RELEASE` - recognise the case sensitivity. 
#### Linking
Now, that Donsus can be compiled let's use the fresh donsus executable to compile some Donsus files.
Once the object file is built with the platform oriented extension `.obj` on Windows, Donsus will attempt to link the
object files together to obtain the executable. If Donsus is built with MSVC - by default MSCV'S linker will be used.

The linker `link.exe` must be in the Path and should be downloaded from: https://visualstudio.microsoft.com/downloads/?q=build+tools (build-tools).
#### Cmake Generator
Ninja is the preferred generator for Donsus as it provides `compile_comamnads.json` which shows what commands were executed.

## Linux

### gcc
To compile Donsus with gcc on Linux llvm must be installed as a requirement.

### clang
To compile Donsus with clang on Linux llvm must be installed as a requirement.
Make sure when installing clang from the source clang is enabled:
e.g `cmake -GNinja -DLLVM_ENABLE_PROJECTS=clang`

## MacOs
Not supported yet

#  Building the docs:

Requirements: 
- furo   (theme)
- Sphinx (building tool)


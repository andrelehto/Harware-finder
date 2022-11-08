# Hardware-finder

Hardware-finder provides the ability to gather hardware-info from any Windows machine.

## Contents

- [Hardware-finder](#hardware-finder)
  - [Contents](#contents)
  - [Dependencies](#dependencies)
  - [Build `Hardware-finder`](#build-hardware-finder)
  - [Using `Hardware-finder`](#using-hardware-finder)


## Dependencies

* [CMake: Version 3.25.0-rc3](https://cmake.org/download/)
* [Qt: Version 6.4.0](https://www.qt.io/download-qt-installer?hsCtaTracking=99d9dd4f-5681-48d2-b096-470725510d34%7C074ddad0-fdef-4e53-8aa8-5e8a876d6ab4)

## Build `Hardware-finder`

Requirements: git, cmake, c++ compiler (gcc, clang, MSVC)

1. Download repository:

   ```terminal
    git clone https://github.com/andrelehto/Harware-finder.git
   ```

2. Build using cmake:

   ```terminal
    mkdir build
    cd build
    cmake ..

    # Debug
    cmake --build . --config Debug --target ALL_BUILD -j 10 --

    # Release
    cmake --build . --config Release --target ALL_BUILD -j 10 --
   ```

## Using `Hardware-finder`

Hardware-finder is run as a CLI-tool. To recieve the available commands run:
```terminal
.\Harware-finder.exe -h
```

In order to run the tool, run:

```terminal
.\Hardware-finder.exe -t <target file> #absolute or relative path
```
or ...
```terminal
.\Hardware-finder.exe --target-file <target file> #absolute or relative path
```

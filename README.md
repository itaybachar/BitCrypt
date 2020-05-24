# Bit Crypt

Bit Crypt is an open source, cross-platform file encryption software. Using Argon 2 and AES to encrypt and decrypt selected files.

## Getting Started

### Prerequisites

To compile this code, use g++ or MinGW if on Windows

### Building Sources

Download and build [wxWidgets](https://www.wxwidgets.org/downloads/) using their online instructions for your OS

```
cd build
cmake .. (add -G"MinGW Makefiles" flag if on Windows)
make
```
Executable will be placed in bin folder

### Installing


Enter the build folder

```
cd build
```

Build the CMake file

```
cmake .. (add -G"MinGW Makefiles" flag if on Windows)
```

Install Executable

```
sudo make install
```

### Unintalling

After installing the project a cmake_install.cmake file will be created in the Build folder.

Build CMake the same way as installing.

Finally uninstall using

```
sudo make uninstall
```

## Built With

* [CMake](http://www.cmake.org) - Compilation Control

## Contributing

* [Argon2](https://github.com/p-h-c/phc-winner-argon2) - Password hashing library used

## Authors

* **Itay Bachar** - [itaybachar](https://github.com/itaybachar)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to Argon2 authors


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

### Installing using Binaries
#### Linux
1)Unzip the BitCrypt.tar file anywhere on your computer.
2)Follow instructions to install the program.

#### MacOS
1)Follow the steps from this [article](https://support.apple.com/guide/mac-help/open-a-mac-app-from-an-unidentified-developer-mh40616/mac) to allow unidentified developers.

2)extract the zip file and move BitCrypt.app into your local Applications folder

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

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Acknowledgments

* Hat tip to Argon2 authors


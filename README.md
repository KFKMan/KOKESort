KOKE Sort (also known as Deniz Sort, Dahman Sort)

İstanbul Gelişim University Computer Engineering First Year Second Semester Final Project.

**PERFORMANCE**

**USING SCRIPTS**

In Windows you can directly use (if requiretments installed) scripts with extension "**.bat**" for example install-ninja-source**.bat**

In Linux/MacOS you need to use scripts with extensions "**.sh**" (for example install-ninja-source**.sh**) with run permission, you can give run permission with `chmod +x filename` command (filename must be replaced with scripts fullname) for example `chmod +x install-ninja-source.sh`

**PROJECT PARTS**

* Library
* Test App
* Example App
* Restaurant App

**GENERAL REQUIRETMENTS**

All of the projects (library, test, example app) using CMake for Cross Platform Support as default.

As default there is a CMake configuration providen but you don't limited to use that, this steps is for who want to use providen configuration.

1- Install CMake: You can install it from (cmake.org)

2- Install Supported CMake Tool like Ninja (ninja-build.org) and configure it: For Ninja there's two option you have you can install pre-compiled binary and add it to CMake Path or you can build from source (you can use install-ninja-source.bat or install-ninja-source.sh, note: they need git installed).

**LIBRARY REQUIRETMENTS**

The library is pure C code without any other library (only standart libraries) dependency with C90 support.

**TEST REQUIRETMENTS**

CMocka Installation;

For Linux (Ubuntu / Debian);

```shell
sudo apt-get update
sudo apt-get install cmocka libcmocka-dev
```

For Mac (Homebrew);

```shell
brew install cmocka
```

For Windows;

Option 1, VCPKG;

    VCPKG installation;

```shell
git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.bat

```

    CMocka Installation via VCPKG;

```
./vcpkg install cmocka

```

**Contributors**

[@KFKMan](https://github.com/KFKMan) (Kaan Faruk Kınalı)

[@leokerem7](https://github.com/leokerem7) (Mehmet Kerem Aslan)

[@EmirageCS](https://github.com/EmirageCS) (Emirhan Uçan)

Big thanks to [@dahmansphi](https://github.com/dahmansphi) (Deniz Dahman)

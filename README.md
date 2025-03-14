KOKE Sort (also known as Deniz Sort, Dahman Sort)

İstanbul Gelişim University Computer Engineering First Year Second Semester Final Project.

**Project Overview**

KOKE Sort is a **cross-platform** corresponding library written in **C** as a modular sort. Developed as a capstone project, it showcases efficient algorithm implementation along with multi-language interop, in addition to demonstrating clean code, test frameworks, and build automation practices.

**PERFORMANCE**

**USING SCRIPTS**

In Windows you can directly use (if requiretments installed) scripts with extension "**.bat**" for example install-ninja-source**.bat**

In Linux/MacOS you need to use scripts with extensions "**.sh**" (for example install-ninja-source**.sh**) with run permission, you can give run permission with `chmod +x filename` command (filename must be replaced with scripts fullname) for example `chmod +x install-ninja-source.sh`

**PROJECT PARTS**

* Library (src)
* Test App (src/test)
* Example App
* Restaurant App
* Fusion (fut)
* OOP based Library (src/HighLevel/KOKESort/KOKESort) with Tests (src/HighLevel/KOKESort/UnitTests) and Example Appe (src/HighLevel/KOKESort/KOKESort.Example)
* C# Interop Example (src/HighLevel/KOKESort/KOKESort.ExampleInterop)
* Python Wrapper (src/pybindwrapper): for using it in python you need to have KOKESort build files in the same directory.

**USAGE**

For using python wrapper (use scripts in the src/pybindwrapper);

1- package.bat/sh => Creating .whl package

2- install.bat/sh => Installing .whl package

3- test/test.bat/sh => Copying KOKESort build files and running test file (test.py)

**GENERAL REQUIRETMENTS**

There's no general requiretments but most of the scripts/projects using **CMake**, **CTest**, **Git, Curl**.

As default there is a CMake configuration providen but you don't limited to use that, this steps is for who want to use providen configuration.

1- Install CMake: You can install it from (cmake.org)

2- Install Supported CMake Tool like Ninja (ninja-build.org) and configure it: For Ninja there's two option you have you can install pre-compiled binary and add it to CMake Path or you can build from source (you can use install-ninja-source.bat or install-ninja-source.sh).

**LIBRARY REQUIRETMENTS**

The library is pure C code without any other library (only standart libraries) dependency with C90 support.

**TEST REQUIRETMENTS**

We are using CTest which mostly come with CMake.

**PYBINDWRAPPER REQUIRETMENTS**

It's need pybind11, for **default it's installing it automatically**.

**OOP BASED LIBRARY REQUIRETMENTS**

In Library, Example, UnitTests.Domain there is no requiretments other than language requiretments.

In UnitTests it's need NUnit, for **default it's installing it automatically.**

**FUSION REQUIRETMENTS**

It's only need it's language requiretments (btw Fusion doesn't support libraries) which can be download from official repositories (https://github.com/fusionlanguage/fut/) release page (you can use download.bat/sh for downloading and install.bat/sh for installing but it's **not recommended**).

**Contributors**

[@KFKMan](https://github.com/KFKMan) (Kaan Faruk Kınalı)

[@leokerem7](https://github.com/leokerem7) (Mehmet Kerem Aslan)

[@EmirageCS](https://github.com/EmirageCS) (Emirhan Uçan)

Big thanks to [@dahmansphi](https://github.com/dahmansphi) (Deniz Dahman)

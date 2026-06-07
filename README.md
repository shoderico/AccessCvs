# AccessCvs

AccessCvs provides integration tools and add-ins for version control (CVS/Git) with Microsoft Access, built using Qt.

# Supported Access Version

- 2007 32bit
- 2010 32bit  / 64bit?
- 2013 32bit  / 64bit
- 2016 32bit  / 64bit?
- 2019 32bit  / 64bit?
- 2021 32bit? / 64bit?
- 2024 32bit? / 64bit?

? : not tested

## License

This project is licensed under the GNU Lesser General Public License v3 (LGPL v3).

## Requirements

- Compilers:
  - MSVC 2013  : Qt 5.4.0, 5.7.1?, 5.8.0?
  - MSVC 2015? : Qt ?
  - MSVC 2017  : Qt 5.14.2
  - MSVC 2019? : Qt 5.15.2?

? : not tested

## Setup

1. Clone `qt-solutions` into the sibling folder (same directory level as the `AccessCvs` folder).
   ```
   git clone https://github.com/shoderico/AccessCvs.git
   git clone https://github.com/qtproject/qt-solutions.git
   ```

2. Run the following command from the `qtwinmigrate` directory inside `qt-solutions`:
   ```
   cd qt-solutions/qtwinmigrate
   configure.bat -library
   ```

## Building the Installer

The installer is generated at:

```
<BUILD_ROOT>\installer\src\release\AccessCvs_<Version>_<x86|x64>_<QtVersion>_<Compiler>_release_<Timestamp>.exe
```

## Pre-build binaries

Download from the Releases section.
Install the vcredist package matching your build environment beforehand.

## Project Structure

This is a multi-project Qt solution (`AccessCvs.pro`) containing subprojects for:

- Access add-ins and models

See the `.pro` files and subdirectories for details.

## Project Goal

- Enable exporting and importing of Microsoft Access files in text format.
- Allow AI-assisted development for projects using Microsoft Access.
- Support source code management and version control for Access-based applications.

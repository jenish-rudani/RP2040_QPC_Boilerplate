# QP/C Framework Boilerplate for RP2040

This repository contains boilerplate code for setting up the QP/C framework on the RP2040 microcontroller. 

## Features

- **Single Core Only**: This setup is designed for single-core operation.
- **QSpy**: Work in Progress (WIP).

## Getting Started

### Prerequisites

- Ensure you have the necessary tools and SDKs installed for building projects on the RP2040.
- Set the `QP_DEFAULT_BASE` in the `qp_import.cmake` file to point to your QP/C framework base directory.

### Building the Project

1. Clone the repository:
    ```sh
    git clone <repository-url>
    cd <repository-directory>
    ```

2. Set the `QP_DEFAULT_BASE` in [qp_import.cmake](http://_vscodecontentref_/1):
    ```cmake
    # qp_import.cmake
    set(QP_DEFAULT_BASE "/path/to/your/qpc")
    ```

3. Build the project using Raspberry Pico VSCode Extension:


## Repository Structure

- [app.c](http://_vscodecontentref_/2), [bsp.c](http://_vscodecontentref_/3), [bsp.h](http://_vscodecontentref_/4), [main.c](http://_vscodecontentref_/5), [pico_app.h](http://_vscodecontentref_/6): Source files for the application and board support package.
- [CMakeLists.txt](http://_vscodecontentref_/7): CMake configuration file for building the project.
- [qp_import.cmake](http://_vscodecontentref_/8): CMake script for importing the QP/C framework.

## Notes

- This setup is for single-core operation only.
- QSpy support is currently a work in progress.
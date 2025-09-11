# Project Overview

This is a C++ header-only library template project named "mywheel-cpp". It emphasizes modern C++ and CMake practices, providing a clean separation between the library and executable code. The project is configured for continuous integration, code coverage reporting, and automated documentation generation.

**Key Technologies:**

*   **Language:** C++ (likely C++14 or newer)
*   **Build Systems:** CMake, xmake
*   **Testing:** doctest, CTest
*   **CI/CD:** GitHub Actions
*   **Code Coverage:** Codecov
*   **Documentation:** Doxygen, GitHub Pages
*   **Dependency Management:** CPM.cmake

**Architecture:**

The project follows a modular structure:

*   `include/mywheel/`: Contains the header-only library code.
*   `test/`: Contains the test suite for the library.
*   `standalone/`: Contains a sample executable that uses the library.
*   `documentation/`: Contains the Doxygen configuration for generating documentation.
*   `all/`: A convenience directory to build all targets at once.

# Building and Running

The project can be built using either CMake or xmake.

## CMake

### Build and Run Everything (Recommended for Development)

```bash
cmake -S all -B build
cmake --build build
```

### Run Tests

```bash
./build/test/MyWheelTests
```

### Run Standalone Executable

```bash
./build/standalone/MyWheel --help
```

### Build Documentation

```bash
cmake -S documentation -B build/doc
cmake --build build/doc --target GenerateDocs
```

## xmake

### Build the Project

```bash
xmake
```

### Run Tests

```bash
xmake run test_mywheel
```

# Development Conventions

*   **Coding Style:** The project uses `clang-format` for C++ code and `cmake-format` for CMake files to enforce a consistent coding style. Use the `format` and `fix-format` targets to check and apply the style.
*   **Testing:** The project uses the `doctest` framework for testing. Tests are located in the `test/source/` directory. The test suite is run as part of the CI pipeline.
*   **Dependencies:** External dependencies are managed using `CPM.cmake`. Add new dependencies to the `CMakeLists.txt` file.
*   **Continuous Integration:** The project uses GitHub Actions for CI. Workflows for building, testing, and documentation are defined in the `.github/workflows/` directory.
*   **Documentation:** The documentation is generated using Doxygen. Add Doxygen-style comments to the code to include it in the documentation.

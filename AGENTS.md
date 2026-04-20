# AGENTS.md - Agentic Coding Guidelines for mywheel-cpp

## Overview

This is a header-only C++ library project providing data structures (Lict, Robin, dllist, bpqueue, etc.). The codebase follows Modern CMake practices and uses doctest for testing.

## Build Commands

### CMake (Primary Build System)

```bash
# Configure and build all subprojects
cmake -S all -B build
cmake --build build

# Build and run tests only
cmake -S test -B build/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# Run test executable directly
./build/test/MyWheelTests

# Run a single test case (doctest supports filtering)
./build/test/MyWheelTests -tc="Test Lict"
./build/test/MyWheelTests -tc="Test Lict operator*"
```

### xmake (Alternative)

```bash
# Build
xmake

# Build and run tests
xmake -t

# Run tests
xmake run test_mywheel

# Run a single test
xmake test_mywheel -tc "Test Lict"
```

## Code Formatting

### clang-format

```bash
# View changes
cmake -S test -B build/test
cmake --build build/test --target format

# Apply fixes
cmake --build build/test --target fix-format
```

Required tooling:
- `clang-format==18.1.2`
- `cmake_format==0.6.13`
- `pyyaml`

Install via pip:
```bash
pip install clang-format==18.1.2 cmake_format==0.6.13 pyyaml
```

## Code Style Guidelines

### Based On

- **Style**: Google with modifications (see `.clang-format`)
- **Column Limit**: 100 characters
- **Indent Width**: 4 spaces
- **Access Modifier Offset**: -2
- **Break Before Braces**: Attach
- **Namespace Indentation**: All

### clang-format Config (.clang-format)

```yaml
BasedOnStyle: Google
AccessModifierOffset: "-2"
AlignTrailingComments: "true"
AllowAllParametersOfDeclarationOnNextLine: "false"
AlwaysBreakTemplateDeclarations: "No"
BreakBeforeBraces: Attach
ColumnLimit: "100"
ConstructorInitializerAllOnOneLineOrOnePerLine: "true"
IncludeBlocks: Regroup
IndentPPDirectives: AfterHash
IndentWidth: "4"
NamespaceIndentation: All
BreakBeforeBinaryOperators: All
BreakBeforeTernaryOperators: "true"
```

### include Order (clang-format groups)

1. Main project header (`<mywheel/...>`)
2. Related header (`"dllink.hpp"`)
3. CPMDependencies (`<py2cpp/...>`)
4. C++ Standard Library (`<vector>`, `<utility>`, etc.)
5. C Standard Library

### Naming Conventions

| Element | Convention | Example |
|---------|-----------|---------|
| Classes | PascalCase | `class RobinSlNode`, `class Lict` |
| Namespaces | lowercase | `namespace py`, `namespace fun` |
| Functions | snake_case | `begin()`, `values()` |
| Member variables | snake_case with underscore | `_lst`, `_rng` |
| Type aliases | snake_case | `key_type`, `value_type` |
| Template params | PascalCase | `template <typename T>` |

### Types

- Use C++17 features
- Prefer `auto` for return types when clear
- Always use `[[nodiscard]]` on query methods like `contains()`, `size()`
- Use trailing return types where appropriate

```cpp
[[nodiscard]] auto size() const noexcept -> size_t;
```

### Access Specifiers

Order in classes:
1. `public:` (types, constants)
2. `private:` (implementation details)

### Braces and Spacing

```cpp
// Good
if (condition) {
    do_something();
} else {
    do_other();
}

// Good - braced init for containers
auto S = py::Lict<double>{std::vector<double>{0.6, 0.7, 0.8}};
```

### Error Handling

- Use `std::vector::at()` for bounds-checked access (throws `std::out_of_range`)
- Use `assert` for invariants
- Document exceptions in Javadoc-style comments

### Comments

Use Javadoc-style with `@`, `@param`, `@return`:

```cpp
/**
 * @brief Brief description
 *
 * Extended description with examples:
 * ```cpp
 * auto a = Lict({1, 4, 3, 6});
 * a.contains(2)  // true
 * ```
 *
 * @param key The key parameter
 * @return The value
 */
```

## Project Structure

```
include/mywheel/    # Header files (.h, .hpp)
test/source/     # Test files
standalone/    # Standalone executable
cmake/         # CMake utilities
```

### Header Organization

- Header-only library (INTERFACE target)
- Public includes: `#include <mywheel/xxx.hpp>`
- Namespace: `py`, `fun`, or relevant domain namespace
- All public headers in `include/mywheel/`

## Dependencies

### CPM.cmake

Dependencies are managed via CPM.cmake. Add new dependencies in root `CMakeLists.txt`:

```cmake
CPMAddPackage("gh:owner/repo@version")
```

### External Libraries

- **Py2Cpp**: Required runtime dependency (`Py2Cpp 1.4`)
- **doctest**: Test framework
- **cxxopts**: CLI parsing (standalone)

## Compiler Flags

### GCC/Clang
```bash
-Wall -Wpedantic -Wextra -Werror
```

### MSVC
```bash
/W4 /WX /wd4819 /wd4996
/EHsc
```

## Testing

### Running Tests

```bash
# All tests via CTest
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test

# Direct executable
./build/test/MyWheelTests

# With coverage
cmake -S test -B build/test -DENABLE_TEST_COVERAGE=1
cmake --build build/test
./build/test/MyWheelTests --s=yes
```

### Test Framework

- Framework: doctest
- Test macros: `TEST_CASE`, `CHECK`, `CHECK_EQ`
- Use `static_assert` for compile-time verification

```cpp
#include <doctest/doctest.h>
#include <mywheel/lict.hpp>

TEST_CASE("Test Lict") {
    const auto S = py::Lict<double>{std::vector<double>{0.6, 0.7, 0.8}};
    CHECK_EQ(S.size(), 3U);
    CHECK(S.contains(1));
}
```

## Static Analysis

```bash
# clang-tidy
cmake -S test -B build/test -DUSE_STATIC_ANALYZER=clang-tidy

# Include What You Use (IWYU)
cmake -S test -B build/test -DUSE_STATIC_ANALYZER=iwyu

# cppcheck
cmake -S test -B build/test -DUSE_STATIC_ANALYZER=cppcheck
```

## Git Conventions

- Commit messages: descriptive, imperative mood
- Branch naming: feature/description or fix/description
- Use atomic commits for logical changes
# IFLOW.md - mywheel-cpp 项目上下文

## 项目概述

`mywheel-cpp` 是一个现代 C++ 模板项目，基于 ModernCppStarter 模板创建。这是一个头文件专用库（header-only library），提供了一系列数据结构和算法的实现，主要用于支持像 Fiduccia-Mattheyses 算法这样的应用场景。该项目遵循现代 CMake 实践，使用 CPM.cmake 进行依赖管理，并包含完整的测试套件、代码格式化、文档生成等功能。

## 核心特性

- **头文件专用库**: 所有代码都在头文件中实现，便于集成
- **现代 CMake 实践**: 使用符合现代标准的 CMake 配置
- **完整的 CI/CD**: 包含 GitHub Actions 工作流、代码覆盖率等功能
- **代码质量保证**: 通过 clang-format 和 cmake-format 实现代码格式化

## 主要数据结构

### 1. RepeatArray
重复数组类，存储一个值的多个副本，但不实际存储多个元素，节省内存。

### 2. ShiftArray
偏移数组类，允许使用偏移索引访问容器元素。

### 3. Dllink / Dllist
双链表节点和列表实现，用于高效的数据结构操作。

### 4. BPQueue (Bounded Priority Queue)
有界优先队列，使用整数键 [a..b] 实现，通过桶（数组）和双链表实现，键的范围较小时效率很高。

### 5. Lict
类似字典的数据结构，使用 std::vector 适配实现。

### 6. MapAdapter
容器适配器，提供类似字典的接口访问容器元素。

### 7. Robin
循环调度器实现，用于实现轮询算法。

## 依赖关系

- **Py2Cpp 库**: 项目可能依赖 Py2Cpp 库
- **CPM.cmake**: 用于依赖管理
- **doctest**: 用于测试

## 构建和测试

### 构建独立目标
```bash
cmake -S standalone -B build/standalone
cmake --build build/standalone
./build/standalone/MyWheel --help
```

### 运行测试
```bash
cmake -S test -B build/test
cmake --build build/test
CTEST_OUTPUT_ON_FAILURE=1 cmake --build build/test --target test
```

### 一次性构建所有内容
```bash
cmake -S all -B build
cmake --build build
```

## 开发约定

- 使用 C++14 标准
- 代码遵循 clang-format 格式化规范
- 使用 doctest 框架编写测试
- 通过 CPM.cmake 管理外部依赖

## 项目结构

- `include/mywheel/`: 头文件实现
- `test/`: 测试代码
- `standalone/`: 独立可执行文件示例
- `cmake/`: CMake 模块和配置
- `documentation/`: 文档配置

此项目特别适合需要高效数据结构实现的 C++ 项目，尤其是在实现图算法（如 Fiduccia-Mattheyses 算法）等场景中。
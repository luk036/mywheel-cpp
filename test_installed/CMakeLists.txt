cmake_minimum_required(VERSION 3.14)
project(MyWheelInstalledTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find dependencies first - MyWheelConfig.cmake references Py2Cpp::Py2Cpp
find_package(Py2Cpp REQUIRED)
find_package(MyWheel REQUIRED)

add_executable(mywheel_installed_test main.cpp)
target_link_libraries(mywheel_installed_test PRIVATE MyWheel::MyWheel Py2Cpp::Py2Cpp)

enable_testing()
add_test(NAME mywheel_installed_test COMMAND mywheel_installed_test)

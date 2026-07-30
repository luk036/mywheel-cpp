cmake_minimum_required(VERSION 3.14...3.22)

project(MyWheelStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----
# Guard: when used via add_subdirectory from test_installed, targets are already available
if(NOT TARGET MyWheel::MyWheel)
  include(../cmake/CPM.cmake)
endif()

if(NOT TARGET cxxopts::cxxopts)
  include(../cmake/CPM.cmake)
  CPMAddPackage(
    GITHUB_REPOSITORY jarro2783/cxxopts
    VERSION 3.2.1
    OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
  )
endif()

if(NOT TARGET MyWheel::MyWheel)
  CPMAddPackage(NAME MyWheel SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)

add_executable(${PROJECT_NAME} ${sources})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "MyWheel")

target_link_libraries(${PROJECT_NAME} MyWheel::MyWheel cxxopts::cxxopts)

# This project requires CMake v3.20 or higher.
cmake_minimum_required(VERSION 3.20)

# It is C++ project.
project(FishCode CXX)

# It is FishCode (fishcode) v1.0.0.
set(CMAKE_PROJECT_VERSION_MAJOR 1)
set(CMAKE_PROJECT_VERSION_MINOR 0)
set(CMAKE_PROJECT_VERSION_PATCH 0)

# The project requires C++20 standard.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# This project doesn't use any platform-specific extensions.
set(CMAKE_CXX_EXTENSIONS OFF)

# Find wxWidgets.
find_package(wxWidgets REQUIRED COMPONENTS core base)

# Configure wxWidgets package.
if(wxWidgets_USE_FILE)
    include(${wxWidgets_USE_FILE})
endif()

# The main executable and its dependecies.
add_executable(fishcode
    "sources/block.cpp"
    "sources/block.hpp"
    "sources/error.cpp"
    "sources/error.hpp"
    "sources/file.cpp"
    "sources/file.hpp"
    "sources/fishcode.cpp"
    "sources/fishcode.hpp"
    "sources/key.cpp"
    "sources/key.hpp"
    "sources/password.cpp"
    "sources/password.hpp"
    "sources/strings.hpp"
)

# Link external libraries.
target_link_libraries(fishcode ${wxWidgets_LIBRARIES})


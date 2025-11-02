cmake_minimum_required(VERSION 3.10)
project(GoGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WIN32_EXECUTABLE OFF)  # ✅ Console mode (use main())

# Include header folders
include_directories(include)

# Collect all .cpp files
file(GLOB_RECURSE SOURCES "src/backend/*.cpp" "src/frontend/*.cpp")

add_executable(GoGame ${SOURCES})

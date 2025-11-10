cmake_minimum_required(VERSION 3.10)

project(GoGame)
set(CMAKE_CXX_STANDARD 17)

# ================================
# SFML location (relative to project)
# ================================
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML")

# Header files from SFML
include_directories(${SFML_ROOT}/include)

# Library files (.lib or .a)
link_directories(${SFML_ROOT}/lib)

# ================================
# Your source files
# ================================
file(GLOB SRC_FILES
    src/*.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# ================================
# Link SFML libraries
# ================================
target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
)

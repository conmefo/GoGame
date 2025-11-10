cmake_minimum_required(VERSION 3.10)

project(GoGame VERSION 1.0 LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ================================
# SFML setup
# ================================
# Path to local SFML
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML")

# Include SFML headers
include_directories(${SFML_ROOT}/include)

# Link SFML libraries
link_directories(${SFML_ROOT}/lib)

# ================================
# Source files
# ================================
file(GLOB SRC_FILES
    src/*.cpp
)

# ================================
# Create executable
# ================================
add_executable(${PROJECT_NAME} ${SRC_FILES})

# ================================
# Link SFML libraries
# ================================
# For Windows: debug and release versions
if (WIN32)
    target_link_libraries(${PROJECT_NAME}
        debug sfml-graphics-d
        debug sfml-window-d
        debug sfml-system-d
        optimized sfml-graphics
        optimized sfml-window
        optimized sfml-system
    )
else()
    # Linux/macOS
    target_link_libraries(${PROJECT_NAME}
        sfml-graphics
        sfml-window
        sfml-system
    )
endif()
set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6 COMPONENTS system window graphics audio REQUIRED)


# Optional: copy SFML DLLs for Windows (if needed)
if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SFML_ROOT}/bin" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()

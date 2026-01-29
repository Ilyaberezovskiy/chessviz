cmake_minimum_required(VERSION 3.15)
project(chessviz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Include directories
include_directories(src)

# Source files
set(SOURCES
    src/main.cpp
    src/chess/board.cpp
    src/chess/game.cpp
    src/chess/rules.cpp
    src/ui/window.cpp
    src/ui/board_renderer.cpp
    src/ui/piece_renderer.cpp
    src/ui/input_handler.cpp
)

add_executable(chessviz ${SOURCES})

# Link SFML
target_link_libraries(chessviz sfml-graphics sfml-window sfml-system)

# Copy assets to build directory
# This is a bit tricky for cross-platform, but for Linux/macOS:
add_custom_command(TARGET chessviz POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

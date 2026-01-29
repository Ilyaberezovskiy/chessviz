cmake_minimum_required(VERSION 3.10)
project(ChessGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)

if(NOT SFML_FOUND)
    message(STATUS "SFML not found, fetching from source...")
    include(FetchContent)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
    )
    FetchContent_MakeAvailable(SFML)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/ChessGame.cpp
    src/ChessBoard.cpp
    src/Piece.cpp
    src/GameState.cpp
    src/MoveValidator.cpp
    src/Renderer.cpp
)

# Create executable
add_executable(chess ${SOURCES})

# Link SFML
target_link_libraries(chess sfml-graphics sfml-window sfml-system)

# Copy assets to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

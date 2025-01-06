cmake_minimum_required(VERSION 3.10)
project(Parser)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create list of source files
set(SOURCES
    src/main.cpp
    src/parser.cpp
    src/lexer.cpp
)

# Add executable
add_executable(parser ${SOURCES})

# Add include directories
target_include_directories(parser PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
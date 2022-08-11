cmake_minimum_required(VERSION 3.20.0)

project(example)

# Needed to make the language server work correctly (clangd)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEPS_PATH ${PROJECT_SOURCE_DIR}/libs)

# Dependencies

# Executable
add_subdirectory(${PROJECT_SOURCE_DIR}/src)


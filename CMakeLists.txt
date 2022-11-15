cmake_minimum_required(VERSION 3.20.0)

project(renderer)

# Needed to make the language server work correctly (clangd)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(DEPS_PATH ${PROJECT_SOURCE_DIR}/libs)

# Dependencies

# Executable
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${DEPS_PATH}/glm)
add_subdirectory(${DEPS_PATH}/glfw)
add_subdirectory(${DEPS_PATH}/glad)
add_subdirectory(${DEPS_PATH}/stb_image)

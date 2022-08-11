# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
	main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
# target_link_libraries(${PROJECT_NAME <libs_to_link>)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)


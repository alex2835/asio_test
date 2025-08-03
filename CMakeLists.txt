cmake_minimum_required(VERSION 3.10.0)
project(AsioTest)

set(CMAKE_CXX_STANDARD 23)
# include(${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake)
find_package(boost REQUIRED COMPONENTS asio beast)

add_library(common_deps INTERFACE)
target_link_libraries(common_deps INTERFACE
    boost::boost
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

add_executable(server src/httpserver.cpp)
target_link_libraries(server PRIVATE common_deps)

add_executable(generator src/generator.cpp)
target_link_libraries(generator PRIVATE common_deps)

add_executable(task src/task.cpp)
target_link_libraries(task PRIVATE common_deps)

cmake_minimum_required(VERSION 3.10.0)
project(AsioTest)

set(CMAKE_CXX_STANDARD 23)
find_package(boost REQUIRED COMPONENTS asio beast)

add_library(common_deps INTERFACE)
target_link_libraries(common_deps INTERFACE
    boost::boost
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

add_executable(test src/libc_test.cpp)
target_link_libraries(test PRIVATE common_deps)


add_executable(server src/httpserver.cpp)
target_link_libraries(server PRIVATE common_deps)


add_executable(generator src/generator.cpp)
target_link_libraries(generator PRIVATE common_deps)


add_executable(task src/task.cpp)
target_link_libraries(task PRIVATE common_deps)


add_executable(thread_switching src/thread_switching.cpp)
target_link_libraries(thread_switching PRIVATE common_deps)

add_executable(asio_cpu_test src/asio_cpu_test.cpp)
target_link_libraries(asio_cpu_test PRIVATE common_deps)

add_executable(canselation_state src/canselation_state.cpp)
target_link_libraries(canselation_state PRIVATE common_deps)

add_executable(channels1 src/channels1.cpp)
target_link_libraries(channels1 PRIVATE common_deps)

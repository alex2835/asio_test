cmake_minimum_required(VERSION 3.10.0)
project(AsioTest)

# Declare fmt dependency
# Use Conan generated files
find_package(boost REQUIRED COMPONENTS asio beast)


set(CMAKE_CXX_STANDARD 23)
add_executable(AsioTest src/main.cpp)

target_link_libraries(AsioTest
    PRIVATE
        boost::boost
)
cmake_minimum_required(VERSION 3.10)
project(HttpServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

add_executable(http_server http_server_asio.cpp)

target_link_libraries(http_server Boost::system Threads::Threads)

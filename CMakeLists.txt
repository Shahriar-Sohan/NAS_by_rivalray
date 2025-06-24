cmake_minimum_required(VERSION 3.10)
project(nas_backend)

set(CMAKE_CXX_STANDARD 17)

find_package(Drogon REQUIRED)

add_executable(nas_backend src/main.cpp)
target_link_libraries(nas_backend PRIVATE Drogon::Drogon)
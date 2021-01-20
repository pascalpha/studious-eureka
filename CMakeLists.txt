cmake_minimum_required(VERSION 3.16)
project(studious-eureka)
set(CMAKE_CXX_STANDARD 17)

add_library(eureka INTERFACE)
target_include_directories(eureka INTERFACE src)

add_executable(test-main test/main.cc)
target_link_libraries(test-main eureka)
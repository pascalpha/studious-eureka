cmake_minimum_required(VERSION 3.10)
project(studious-eureka VERSION 1.0.1 DESCRIPTION "header-only library that mimics STL")
set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/src/eureka/functional/reference_wrapper.h
        ${CMAKE_CURRENT_LIST_DIR}/src/eureka/memory/allocator.h
        )

add_library(eureka INTERFACE)
target_sources(eureka INTERFACE ${SOURCES})
target_include_directories(eureka INTERFACE src)

add_subdirectory(test)
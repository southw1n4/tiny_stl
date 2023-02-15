cmake_minimum_required(VERSION 3.5)

project(tiny_stl_test)
set(CMAKE_CXX_STANDARD 14)

include_directories(
    ${PROJECT_SOURCE_DIR}/tiny_stl
    ${PROJECT_SOURCE_DIR}/test
)


add_executable(${PROJECT_NAME}
    test/test.cc
)

target_link_libraries(${PROJECT_NAME}
    gtest
    pthread
)



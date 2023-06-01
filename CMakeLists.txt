cmake_minimum_required(VERSION 3.10)

project(tablescript)

set(CMAKE_BUILD_TYPE Release)


find_package(Lua REQUIRED)
include_directories(/usr/local/include ${LUA_INCLUDE_DIR})

file(GLOB SOURCES "src/*.cpp")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 20)

add_executable(tablescript ${SOURCES})
target_link_libraries(tablescript ${LUA_LIBRARIES})

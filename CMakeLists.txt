cmake_minimum_required(VERSION 3.16)

project(ChatApplication)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(server
    src/server.cpp
    server/main.cpp
)

add_executable(client
    client/main.cpp
    src/client.cpp
)

target_compile_options(server PRIVATE
    -Wall
    -Wextra
    -pedantic
)


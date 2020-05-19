# cmake_minimum_required(VERSION <specify CMake version here>)
project(Linux)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS -Wall -pedantic)
find_package(Threads REQUIRED)

include_directories(include)
include_directories(services/include)

add_executable(client
        include/client.h
        include/linuxsocket.h
        services/include/pipeService.h
        services/include/echoService.h
        services/include/soundService.h
        services/src/pipeService.c
        services/src/echoService.c
        services/src/soundService.c
        client.c
        src/linuxsocket.c)
target_link_libraries(client asound)

add_executable(serverM
        main.c
        include/linuxsocket.h
        include/registry.h
        services/include/pipeService.h
        services/include/echoService.h
        services/include/soundService.h
        services/src/pipeService.c
        services/src/echoService.c
        services/src/soundService.c
        src/linuxsocket.c
        src/registry.c)
target_link_libraries(serverM asound)

add_executable(serverT
        main.c
        include/linuxsocket.h
        include/registry.h
        services/include/pipeService.h
        services/include/echoService.h
        services/include/soundService.h
        src/linuxsocket.c
        src/registry.cpp
        services/src/pipeService.c
        services/src/echoService.c
        services/src/soundService.c)

target_link_libraries(serverT asound)
target_link_libraries(serverT pthread)
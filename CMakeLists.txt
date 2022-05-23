cmake_minimum_required(VERSION 3.19)

project(dgraphclient C CXX)

set(PROJECT_NAME "dgraph-cpp")
set(PROJECT_VERSION "0.0.1")
set(CMAKE_PREFIX_PATH "/home/ali/.local")


include_directories(include protos)
#include_directories("protos")

file(GLOB SOURCES
        protos/*.cc
        src/*.cpp)


# src/dgraphclient.cpp
add_library(${PROJECT_NAME} SHARED ${SOURCES})


project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})



find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(re2 CONFIG REQUIRED)
find_package(c-ares CONFIG REQUIRED)



message(STATUS "Using gRPC ${gRPC_VERSION}")

 

target_link_libraries(${PROJECT_NAME} PRIVATE gRPC::gpr gRPC::upb  gRPC::grpc++ gRPC::grpc gRPC::grpc++_reflection)
target_link_libraries(${PROJECT_NAME} PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
target_link_libraries(${PROJECT_NAME} PRIVATE re2::re2 c-ares::cares)



add_definitions(-latomic)
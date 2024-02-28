cmake_minimum_required(VERSION 3.0)
project(gstore)
set(CMAKE_BUILD_TYPE Debug)
set(CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenSSL REQUIRED)

if(OPENSSL_FOUND)
  message(STATUS "OpenSSL found")
else()
  message(FATAL_ERROR "OpenSSL not found")
endif()

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(gtest)
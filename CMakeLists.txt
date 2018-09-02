cmake_minimum_required(VERSION 3.10)
project(simple-http-server)

include_directories(/)
set (CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES src/Server.cpp src/HTTPRequest.cpp src/Listener.cpp src/Worker.cpp src/HeaderBuilder.cpp src/HeaderBuilder.h src/FileRequest.cpp src/FileRequest.h src/RequestSolver.cpp src/RequestSolver.h src/CgiBinRequest.cpp src/CgiBinRequest.h)

add_executable(simple-http-serve2r ${SOURCE_FILES})
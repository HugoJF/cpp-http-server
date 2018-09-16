cmake_minimum_required(VERSION 3.10)
project(simple-http-server)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package (Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/inc/)
set (CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES src/Server.cpp src/HTTPRequest.cpp src/Listener.cpp src/Worker.cpp src/HeaderBuilder.cpp src/FileRequest.cpp src/RequestSolver.cpp src/CgiBinRequest.cpp)

add_executable(simple-http-server ${SOURCE_FILES})

target_link_libraries(simple-http-server Threads::Threads)
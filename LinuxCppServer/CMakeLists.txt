cmake_minimum_required(VERSION 3.16)
project(LinuxCppServer)

set(CMAKE_CXX_STANDARD 11)

include_directories(G:\\boost_1_73_0)
link_directories(G:\\boost_1_73_0\\libs)

link_libraries(ws2_32)
link_libraries(wsock32)

add_executable(LinuxCppServer main.cpp main.h MultiThread_Study.h MultiThread_Study.cpp BoostAsio_Study.cpp BoostAsio_Study.h)

cmake_minimum_required(VERSION 3.15)
project(ftp_server)

set(CMAKE_BUILD_TYPE "Debug")
aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRCS)
set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
include_directories(${PROJECT_SOURCE_DIR}/include)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(ftp_server ${DIR_SRCS} )
target_link_libraries(ftp_server libevent.so libz.so pthread)
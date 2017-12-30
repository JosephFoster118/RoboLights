cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

set(PROJECT_NAME RoboLights)
message(STATUS "************  ${PROJECT_NAME} ************")
project(${PROJECT_NAME})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(PORT_TYPE POSIX)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(PORT_TYPE WIN)
	add_definitions( /wd4996 )
endif()

file (GLOB SRCS "src/*.cpp")
file (GLOB HDRS "include/*.h")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
link_directories(${LIBRARY_OUTPUT_PATH})
message(STATUS "Directory is ${PROJECT_BINARY_DIR}")
message(STATUS "Binary directory is ${EXECUTABLE_OUTPUT_PATH}")
message(STATUS "Library directory is ${LIBRARY_OUTPUT_PATH}")

include_directories(include)

add_executable(${PROJECT_NAME} ${HDRS} ${SRCS})

link_directories(${LIBRARY_OUTPUT_PATH})
find_library(LUA lua PATH lib)
find_library(DL dl PATH lib)
find_package (Threads)

target_link_libraries(${PROJECT_NAME} lua)
target_link_libraries(${PROJECT_NAME} dl)
target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})


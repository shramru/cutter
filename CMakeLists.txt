cmake_minimum_required(VERSION 2.8)
project(cutter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(CUDA_TOOLKIT_ROOT_DIR "/usr/local/cuda")
set(OpenCV_DIR "/usr/local/opencv3/share/OpenCV")

find_package(OpenCV 3 REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories( ${OpenCV_INCLUDE_DIRS} )

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME}  ${OpenCV_LIBS})
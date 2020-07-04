cmake_minimum_required(VERSION 3.15.3)
project(RaulTestQt)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCL_TARGET_OPENCL_VERSION=220")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCL_TARGET_OPENCL_VERSION=220")

find_package(Threads REQUIRED)

find_package(OpenCL REQUIRED)
include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE OPENCL_FILES "src/*.cl")
file(GLOB_RECURSE OLD_CL_FILES "${CMAKE_CURRENT_BINARY_DIR}/*.cl")
file(REMOVE "${OLD_CL_FILES}")
foreach(OPENCL_FILE ${OPENCL_FILES})
    file(RELATIVE_PATH OPENCL_FILE_RELATIVE "${CMAKE_SOURCE_DIR}/src" ${OPENCL_FILE})
    configure_file(${OPENCL_FILE} "${CMAKE_CURRENT_BINARY_DIR}/${OPENCL_FILE_RELATIVE}" COPYONLY)
endforeach()

file(GLOB_RECURSE SRCS
        "src/*.hpp"
        "src/*.cpp"
        "src/*.ui"
        )
add_executable(ui ${SRCS})
target_link_libraries(ui Qt5::Widgets Qt5::Core Qt5::Network ${OpenCL_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

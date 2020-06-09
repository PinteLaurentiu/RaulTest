cmake_minimum_required(VERSION 3.15.3)
project(RaulTestQt)

find_package(Threads REQUIRED)
find_package(OpenCL REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

include_directories(${OpenCL_INCLUDE_DIRS})
link_directories(${OpenCL_LIBRARY})
file(GLOB_RECURSE BACKEND_SRCS
        "src/backend/*.hpp"
        "src/backend/*.cpp"
        )
add_library(backend ${BACKEND_SRCS})
target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(backend ${OpenCL_LIBRARY})
file(GLOB_RECURSE BACKEND_OPENCL_FILES "src/backend/*.cl")
foreach(BACKEND_OPENCL_FILE ${BACKEND_OPENCL_FILES})
    file(RELATIVE_PATH OPENCL_FILE_RELATIVE "${CMAKE_SOURCE_DIR}/src/backend" ${BACKEND_OPENCL_FILE})
    configure_file(${BACKEND_OPENCL_FILE} "${CMAKE_CURRENT_BINARY_DIR}/${OPENCL_FILE_RELATIVE}" COPYONLY)
endforeach()

file(GLOB_RECURSE UI_SRCS
        "src/ui/*.hpp"
        "src/ui/*.cpp"
        "src/ui/*.ui"
        )
add_executable(ui ${UI_SRCS})
target_link_libraries(ui Qt5::Widgets backend ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(ui PRIVATE src/backend)

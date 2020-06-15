cmake_minimum_required(VERSION 3.15.3)
project(RaulTestQt)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Threads REQUIRED)
find_package(OpenCL REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
#find_package(MySQL++ REQUIRED)
#find_package(MySQLclient REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCL_TARGET_OPENCL_VERSION=220 -DMYSQLPP_MYSQL_HEADERS_BURIED")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DCL_TARGET_OPENCL_VERSION=220 -DMYSQLPP_MYSQL_HEADERS_BURIED")

option(EDITMODE "Build with edit mode" OFF)

if (${EDITMODE})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DEDITMODE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DEDITMODE")
endif()

include_directories(${OpenCL_INCLUDE_DIRS})
#include_directories(${MySQL++_INCLUDE_DIRS})
#include_directories(${MySQLclient_INCLUDE_DIR})
link_directories(${OpenCL_LIBRARY})
file(GLOB_RECURSE BACKEND_SRCS
        "src/backend/*.hpp"
        "src/backend/*.cpp"
        )
add_library(backend ${BACKEND_SRCS})
target_include_directories(backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(backend Qt5::Core Qt5::Sql ${OpenCL_LIBRARY} ${MySQL++_LIBRARIES} ${MySQLclient_LIBRARIES})
file(GLOB_RECURSE BACKEND_OPENCL_FILES "src/backend/*.cl")
file(GLOB_RECURSE OLD_CL_FILES "${CMAKE_CURRENT_BINARY_DIR}/*.cl")
file(REMOVE "${OLD_CL_FILES}")
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

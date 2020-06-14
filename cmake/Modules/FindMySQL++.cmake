# - Try to find MySQL++
# Once done, this will define
#
#  MySQL++_FOUND - system has MySQL++
#  MySQL++_INCLUDE_DIRS - the MySQL++ include directories
#  MySQL++_LIBRARIES - link these to use MySQL++

include(LibFindMacros)

# # dependencies
# libfind_package(MySQL++ MySQLclient)

# Include dir
find_path(MySQL++_INCLUDE_DIR
  NAMES mysql++/mysql++.h
  PATHS ENV PATH PATH_SUFFIXES include mysql++
)

# Finally the library itself
find_library(MySQL++_LIBRARY
  NAMES mysqlpp
  PATHS ENV PATH PATH_SUFFIXES lib
)

libfind_process(MySQL++)

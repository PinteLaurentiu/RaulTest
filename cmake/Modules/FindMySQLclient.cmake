# - Try to find MySQLclient
# Once done, this will define
#
#  MySQLclient_FOUND - system has MySQL++
#  MySQLclient_INCLUDE_DIRS - the MySQL++ include directories
#  MySQLclient_LIBRARIES - link these to use MySQL++

include(LibFindMacros)

# Include dir
find_path(MySQLclient_INCLUDE_DIR
  NAMES mysql/mysql.h
  PATHS ENV PATH PATH_SUFFIXES include mysql
)

# Finally the library itself
find_library(MySQLclient_LIBRARY
  NAMES mysqlclient
  PATHS ENV PATH PATH_SUFFIXES lib mysql
)

libfind_process(MySQLclient)

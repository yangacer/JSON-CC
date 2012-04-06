cmake_minimum_required (VERSION 2.8)
project (json)

include ( rpath.cmake )
include_directories ( /usr/local/include )

add_library ( json SHARED json_inst.cpp util.cpp)

add_executable ( test main.cpp )
target_link_libraries ( test json )

install ( TARGETS json DESTINATION lib )
install ( FILES json.hpp util.hpp DESTINATION include/json )

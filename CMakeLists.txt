cmake_minimum_required (VERSION 2.8)
project (json)

include_directories ( /usr/local/include )

add_library ( json SHARED json_inst.cpp )

add_executable ( test main.cpp )
target_link_libraries ( test json )

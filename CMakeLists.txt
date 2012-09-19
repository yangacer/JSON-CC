cmake_minimum_required (VERSION 2.8)
project (json)

find_package ( Boost 1.45.0 COMPONENTS
               serialization)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

include ( rpath.cmake )
#include_directories ( /usr/local/include )


if(CMAKE_COMPILER_IS_GNUCXX)
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null")
endif()

add_library ( json SHARED parser_inst.cpp util.cpp)

add_executable ( test main.cpp )
target_link_libraries ( test json )

add_executable ( savlod serialize.cpp)
target_link_libraries ( savlod ${Boost_LIBRARIES} json)

install ( TARGETS json DESTINATION lib )
install ( FILES json.hpp parser.hpp parser_def.hpp util.hpp DESTINATION include/json )

cmake_minimum_required (VERSION 2.8)
project (json)

find_package ( Boost 1.45.0 COMPONENTS
               serialization
               regex)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories (${Boost_LIBRARY_DIRS} )
endif()

include ( rpath.cmake )
include ( use_c++11.cmake )

if(CMAKE_COMPILER_IS_GNUCXX)
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null")
endif()

add_library ( json SHARED parser_inst.cpp util.cpp filter.cpp)
target_link_libraries ( json boost_regex )

add_subdirectory ( tests )

install ( TARGETS json DESTINATION lib )
install ( FILES json.hpp parser.hpp parser_def.hpp util.hpp DESTINATION include/json )

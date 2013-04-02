cmake_minimum_required (VERSION 2.8)
project (json)

find_package ( Boost )

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
#link_directories (${Boost_LIBRARY_DIRS} )
endif()

include ( rpath.cmake )
include ( use_c++11.cmake )

if(CMAKE_COMPILER_IS_GNUCXX)
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null")
endif()

include_directories ( json )

add_library ( json SHARED parser_inst.cpp util.cpp print.cpp accessor.cpp) #filter.cpp)
target_link_libraries ( json ${Boost_LIBRARIES} )

add_executable ( jsav jsav.cpp )
target_link_libraries ( jsav json )

ENABLE_TESTING()

add_subdirectory ( tests )

option ( JSON_INSTALL_RUNTIME ON )
option ( JSON_INSTALL_HEADERS OFF )

if( JSON_INSTALL_RUNTIME )
  install ( TARGETS jsav DESTINATION bin )
  install ( TARGETS json DESTINATION lib )
endif ()
if( JSON_INSTALL_HEADERS )
  install ( DIRECTORY json  DESTINATION include )
endif ()

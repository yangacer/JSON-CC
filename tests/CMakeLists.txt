
include_directories ( ${PROJECT_SOURCE_DIR} )

add_executable ( parser parser.cpp )
target_link_libraries ( parser json )

add_executable ( savlod serialize.cpp )
target_link_libraries ( savlod boost_serialization json)

#add_executable ( filter filter.cpp )
#target_link_libraries ( filter json )

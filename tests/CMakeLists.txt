
include_directories ( ${PROJECT_SOURCE_DIR} )

add_executable ( parser parser.cpp )
target_link_libraries ( parser json )

#add_executable ( savlod serialize.cpp )
#target_link_libraries ( savlod boost_serialization json)

add_executable ( access access.cpp )
target_link_libraries ( access json )

add_test ( parser_test parser ${PROJECT_SOURCE_DIR}/tests/test.json )

add_executable ( unit unit.cpp )
target_link_libraries ( unit boost_unit_test_framework json)

#add_executable ( filter filter.cpp )
#target_link_libraries ( filter json )

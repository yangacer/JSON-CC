#
# Add proper flags for compiling code in C++11.
# TODO Add more compiler detection.

if(CMAKE_COMPILER_IS_GNUCXX)
  message ("GNUCXX(g++) is detected. Apply C++11 flags.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message ("GNUCXX(g++) is the only compler supported currently")
endif()


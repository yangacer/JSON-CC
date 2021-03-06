#
# Add proper flags for compiling code in C++11.
# TODO Add more compiler detection.

if(CMAKE_COMPILER_IS_GNUCXX)
  message ("GNUCXX(g++) is detected. Apply C++11 flags.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
elseif(${CMAKE_CXX_COMPILER} MATCHES "Visual Studio 10")
  message ("MSVC 10 is detected. C++11 is supported")
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
  message ("Clang is detected. Apply C++11 flags.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
else()
  message ("Unsupported compiler")
endif()


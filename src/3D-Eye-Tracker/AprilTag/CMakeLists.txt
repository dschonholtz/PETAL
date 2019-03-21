project(AprilTag-crossplatform)

cmake_minimum_required ( VERSION 2.8 )

if(WIN32 AND MSVC)
  add_subdirectory(pthreads-win32)
endif()
add_subdirectory(apriltag-2016-12-01)

find_package(OpenCV)
if(OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIRS})
  set(HAVE_OPENCV ON)
else()
  set(HAVE_OPENCV OFF)
endif()

configure_file(config.h.in config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# set the list of source files
set(test_cpp
  #${CMAKE_CURRENT_SOURCE_DIR}/example/apriltag_demo.c
  ${CMAKE_CURRENT_SOURCE_DIR}/example/opencv_demo.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/example/opencv_demo_offline.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/example/demo_offline_simple.cc
)

# Copy test image
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/example/AprilTag.pgm ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
add_definitions(-DAPRIL_TAG_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/example/AprilTag.pgm")

foreach(cpp ${test_cpp})
  get_filename_component(target ${cpp} NAME_WE)
  add_executable(${target} ${cpp})
  if(OpenCV_FOUND)
    target_link_libraries(${target} APRILTAG_LIBRARY ${OpenCV_LIBS})
  else()
    target_link_libraries(${target} APRILTAG_LIBRARY)
  endif()
endforeach()

enable_testing()
set(test_name test-apriltag)
#set(test_name "test-apriltag-${target}")
add_test(NAME ${test_name} COMMAND demo_offline_simple)

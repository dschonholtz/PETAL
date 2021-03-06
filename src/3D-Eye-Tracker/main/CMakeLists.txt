

include_directories (
#  "${PROJECT_SOURCE_DIR}"
  "../external/DirectShowFrameGrabber"
  ${OpenCV_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
  ${CERES_INCLUDE_DIRS}
  ${spii_INCLUDE_DIRS}
  ${FANN_INCLUDE_DIRS}
  "../singleeyefitter"
  "../AprilTag/apriltag-2016-12-01"
  "../AprilTag/apriltag-2016-12-01/common"
)


file (GLOB SRCS 
  "*.cpp" 
  "../external/DirectShowFrameGrabber/*.cpp"
  "../external/DirectShowFrameGrabber/*.c"
  "GUIPub.rc"
  "GUIPub.ico"
  # "external/DirectShowFrameGrabber/*.idl"pupillabs
  )

file (GLOB HEADERS 
  *.h *.hpp 
  "../external/DirectShowFrameGrabber/*.h"
  )

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
add_executable( main ${SRCS} ${HEADERS} )
target_link_libraries( main 
		       ${OpenCV_LIBS}
		       ${Boost_FILESYSTEM_LIBRARY}
		       ${Boost_SYSTEM_LIBRARY}
		       ${Boost_LIBRARIES}
           ${spii_LIBRARIES}
           ${FANN_LIBRARIES}
           "singleeyefitter"
		   APRILTAG_LIBRARY
           )

cmake_minimum_required(VERSION 2.8)
project(HeadTracker)

add_subdirectory(AprilTag)

set(OpenCV_DIR "C:/local/opencv-3.1.0/build")
find_package(OpenCV REQUIRED)
file(GLOB OPENCV_DLLS
  "${OpenCV_DIR}/x64/vc14/bin/*.dll"
)
file(COPY ${OPENCV_DLLS} DESTINATION ${PROJECT_BINARY_DIR})

include_directories(
	${OpenCV_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/AprilTag/apriltag-2016-12-01)

file(GLOB SRCS "*.cpp")
file(GLOB HEADERS "*.h")

add_executable(HeadTracker ${SRCS} ${HEADERS})
target_link_libraries(HeadTracker
	${OpenCV_LIBS}
	APRILTAG_LIBRARY
	)
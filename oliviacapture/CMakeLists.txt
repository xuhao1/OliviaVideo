PROJECT (OliviaCapture)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_COMPILER g++)
SET(SRC_LIST oliviacapture/capcamera.cpp 
 oliviacapture/display.cpp 
 oliviacapture/encoder.cpp  
 oliviacapture/main.cpp 
 oliviacapture/videostream.cpp 
 )


find_package( OpenCV REQUIRED )
find_package(PkgConfig REQUIRED)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(AVCODEC REQUIRED libavcodec)

INCLUDE_DIRECTORIES(oliviacapture/ /usr/local/include/ )

set(CMAKE_CXX_FLAGS "-std=c++11")

ADD_EXECUTABLE(oliviacapture ${SRC_LIST})
target_link_libraries(oliviacapture ${OpenCV_LIBS} ${FFMPEG_LIB} ${AVUTIL_LIBRARIES} ${AVCODEC_LIBRARIES} )


cmake_minimum_required(VERSION 3.0)

# set project name from current directory
get_filename_component(BASENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${BASENAME})

set (CMAKE_CXX_STANDARD 11)

aux_source_directory(src SRC_FILES  )
aux_source_directory(x264/src X264_SRC_FILES  )
aux_source_directory(jpg/src JPG_SRC_FILES  )
add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${X264_SRC_FILES} ${JPG_SRC_FILES} )
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/inc"  "${CMAKE_CURRENT_LIST_DIR}/sw/redis++")

add_executable (${PROJECT_NAME}test main.cpp)
target_link_libraries (${PROJECT_NAME}test ${PROJECT_NAME} 
opencv_core opencv_video opencv_videoio opencv_highgui  opencv_imgcodecs opencv_imgproc
boost_system  boost_filesystem jpeg  /home/demo/v4l2rtspserver/lib/libx264.so.164
/home/demo/v4l2rtspserver/lib/libredis++.so
/home/demo/v4l2rtspserver/lib/libhiredis.so

) 

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package (Threads)
target_link_libraries (${PROJECT_NAME} Threads::Threads) 

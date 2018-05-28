cmake_minimum_required (VERSION 2.8.12)
project (ConfigIPCAM)
find_package(OpenCV REQUIRED)

include_directories(../include)
add_definitions("-std=c++11")


add_executable(showCam showCam.cpp)
target_link_libraries(showCam ${OpenCV_LIBS} stdc++fs)

add_executable(synSecond synSecond.cpp)
target_link_libraries(synSecond ${OpenCV_LIBS} curl)

add_executable(ircut ircut.cpp)
target_link_libraries(showCam ${OpenCV_LIBS} curl)

add_executable(capIRcut capIRcut.cpp)
target_link_libraries(showCam ${OpenCV_LIBS} curl stdc++fs)

add_executable(capPattern capPattern.cpp)
target_link_libraries(showCam ${OpenCV_LIBS} curl stdc++fs)

add_executable(capPatIRcut capPatIRcut.cpp)
target_link_libraries(showCam ${OpenCV_LIBS} curl stdc++fs)

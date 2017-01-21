cmake_minimum_required(VERSION 2.8.3)
project(bprobot)

find_package(
    catkin
    REQUIRED
    COMPONENTS
    roscpp
    rosconsole
    sensor_msgs
    std_msgs
    message_generation
#    hiredis
)

add_message_files(
    FILES
    F_LIVING_STATUS.msg
    D_1_A_IFS_INPUT.msg
    G_1_A_SENSOR_INPUT.msg
    G_2_A_IFS_INPUT.msg
    F_OPT_LIVING.msg
)

generate_messages(
    DEPENDENCIES
    std_msgs
)

catkin_package(
    CATKIN_DEPENDS
    message_runtime
)

include_directories(
    ${catkin_INCLUDE_DIRS}
)

SET(CMAKE_CXX_FLAGS "-std=c++11")
SET(LIB_REDIS "libhiredis.so")

#find_library()

#TARGET_LINK_LIBRARIES(hiredis)

#include_directories(
#    /usr/local/include/
#)
#LINK_DIRECTORIES(
#    bprobot hiredis
#)
#INCLUDE_DIRECTORIES ("${HIREDIS_DIR}/include")

#set(CMAKE_EXE_LINKER_FLAGS "-lhiredis")

#add_subdirectory(F/redis-cplusplus-client-master)

add_executable(
    liuchang

    F/main.cpp

    F/livingstatus/livingstatus.hpp
    F/livingstatus/livingstatus_callback.cpp
    F/livingstatus/livingstatus_depends.cpp
    F/livingstatus/livingstatus_redis.cpp

    F/interpolation/interpolation.hpp
    F/interpolation/interpolation.cpp

    F/track/track.hpp
    F/track/track.cpp

    F/controller.hpp
    F/controller.cpp
)


target_link_libraries(
    liuchang
    ${LIB_REDIS}
    ${catkin_LIBRARIES}
)

add_executable(
    test_send
    F/sender.cpp
)

target_link_libraries(
    test_send
    ${catkin_LIBRARIES}
)

add_executable(
    test_listen
    F/listener.cpp
)
target_link_libraries(
    test_listen
    ${catkin_LIBRARIES}
)

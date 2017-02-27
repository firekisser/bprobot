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
#    OpenCV what the fuck???


    #for points and OpenCV~~
    laser_geometry
    tf
    visualization_msgs
    image_geometry
    dynamic_reconfigure

)


add_message_files(

    FILES

    msg_F_OPT_LIVING.msg        #listen for trigger
    msg_F_LIVING_STATUS.msg     #publish

    msg_D_1_A_IFS_INPUT.msg     #graphic
    msg_G_1_A_IFS_INPUT.msg     #pyroelectricity
    msg_G_1_A_SENSOR_INPUT.msg  #arm X头部垂直高度坐标_Y头部左右水平坐标_Z头部点抬头_K头部左右看角度
    msg_G_2_A_IFS_INPUT.msg     #a -1表示该模块在关闭状态 其他为被唤醒的说话人的角度（增强角度）
    msg_K_1_A_IFS_INPUT.msg     #a 说话人角度

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

add_executable(

    liuchang
#--------------------------------------------------------#
    #main and controller
    F/main.cpp
    F/controller.hpp
    F/controller.cpp
    F/testdata.hpp
#--------------------------------------------------------#
    #F/livingstatus root
    F/livingstatus/livingstatus.hpp
    F/livingstatus/livingstatus_datamerge.cpp
    F/livingstatus/livingstatus_depends.cpp
    F/livingstatus/livingstatus_redis.cpp
    F/livingstatus/detectinfo.hpp
    F/livingstatus/detectinfo.cpp
#--------------------------------------------------------#
    #F/livingstatus/graphic
    F/livingstatus/graphic/graphic.hpp
    F/livingstatus/graphic/graphic.cpp
    F/livingstatus/graphic/graresult.hpp
    F/livingstatus/graphic/graresult.cpp
    F/livingstatus/graphic/graresulthuman.hpp
    F/livingstatus/graphic/graresulthuman.cpp
    F/livingstatus/graphic/graresultface.hpp
    F/livingstatus/graphic/graresultface.cpp
    F/livingstatus/graphic/graresultpet.hpp
    F/livingstatus/graphic/graresultpet.cpp
#--------------------------------------------------------#
    #F/livingstatus/laser root
    F/livingstatus/laser/laser.hpp
    F/livingstatus/laser/laser.cpp
    #F/livingstatus/laser/datamerge
    F/livingstatus/laser/datamerge/datamerge.hpp
    F/livingstatus/laser/datamerge/datamerge.cpp
    F/livingstatus/laser/datamerge/sample.hpp
    F/livingstatus/laser/datamerge/sample.cpp
    F/livingstatus/laser/datamerge/sampleset.hpp
    F/livingstatus/laser/datamerge/sampleset.cpp
    F/livingstatus/laser/datamerge/samplesetfactory.hpp
    F/livingstatus/laser/datamerge/samplesetfactory.cpp
    F/livingstatus/laser/datamerge/features.hpp
    F/livingstatus/laser/datamerge/features.cpp
    #F/livingstatus/laser/train
    F/livingstatus/laser/forestpredict/forestpredict.hpp
    F/livingstatus/laser/forestpredict/forestpredict.cpp
#--------------------------------------------------------#
    #F/livingstatus/pyroelectricity
    F/livingstatus/pyroelectricity/pyroelectricity.hpp
    F/livingstatus/pyroelectricity/pyroelectricity.cpp
#--------------------------------------------------------#
    #F/livingstatus/voice
    F/livingstatus/voice/voice.cpp
    F/livingstatus/voice/voice.hpp
#--------------------------------------------------------#



    #not needed now
    F/interpolation/interpolation.hpp
    F/interpolation/interpolation.cpp
    #not needed now
    F/track/track.hpp
    F/track/track.cpp

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

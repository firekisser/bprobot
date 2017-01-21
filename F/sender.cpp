#include "ros/ros.h"
#include "std_msgs/String.h"
#include "bprobot/D_1_A_IFS_INPUT.h"
#include "bprobot/F_OPT_LIVING.h"
#include <sstream>

/**
* This tutorial demonstrates simple sending of messages over the ROS system.
*/

int main(int argc, char **argv)
{

    ros::init(argc, argv, "main");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<bprobot::D_1_A_IFS_INPUT>("D_1_A_IFS_INPUT", 1000);
    ros::Publisher control_pub = n.advertise<bprobot::F_OPT_LIVING>("F_OPT_LIVING", 1000);
    ros::Rate loop_rate(1);

    int count = 0;
    while (ros::ok())
    {

        bprobot::F_OPT_LIVING input1;
        ros::Time t;
        t.init();
        t = t.now();
        input1.time = t;
        input1.type = 1;
        input1.id = -1;
        input1.data = "{}";
        input1.module = "LaLaLa";
        control_pub.publish(input1);



        bprobot::D_1_A_IFS_INPUT input0;

        input0.t = 111111;
        input0.id = 100000;
        input0.b.push_back(1001);
        input0.b.push_back(1001);
        input0.b.push_back(1001);
        input0.b.push_back(1001);
        input0.b.push_back(1001);
        input0.b.push_back(1001);
        chatter_pub.publish(input0);
        
        input0.t = 222222;
        input0.id = 200000;
        input0.o.push_back(2002);
        input0.o.push_back(2002);
        input0.o.push_back(2002);
        input0.o.push_back(2002);
        input0.o.push_back(2002);
        input0.o.push_back(2002);
        input0.o.push_back(2002);
        chatter_pub.publish(input0);

        input0.t = 333333;
        input0.id = 300000;
        input0.b.push_back(3003);
        input0.b.push_back(3003);
        input0.b.push_back(3003);
        input0.b.push_back(3003);
        input0.b.push_back(3003);
        input0.b.push_back(3003);
        chatter_pub.publish(input0);

        ros::spinOnce();
        loop_rate.sleep();
        
        ++count;
    }        
    return 0;
}

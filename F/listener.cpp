/* This is a test module for msg_F_LIVING_STATUS message published
*/

#include "ros/ros.h"
#include "bprobot/msg_D_1_A_IFS_INPUT.h"
#include "bprobot/msg_F_LIVING_STATUS.h"
#include <vector>
#include <utility>
#include <string>

using namespace std;

void inputCallback(const bprobot::msg_F_LIVING_STATUS& msg)
{

    ROS_INFO("Time:");
    cout << msg.time <<endl;
    for (int i = 0; i < msg.targets.size(); ++i) 
    {
        ROS_INFO("%d : %s",i, msg.targets[i].c_str());
    }
    ROS_INFO("===================================");
}

int main(int argc,char **argv)
{
    ros::init(argc,argv,"listener");

    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("msg_F_LIVING_STATUS",1000,inputCallback);

    ros::spin();

    return 0;
}


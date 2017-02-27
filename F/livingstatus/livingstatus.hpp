#ifndef LIVINGSTATUS_HPP
#define LIVINGSTATUS_HPP

/****************************************
 *      Last Edited By LiuChang         *
 *      Subcribe Two Message:           *
 *          1.From Laser                *
 *          2.From Graphic D            *
 *      Date 2017/2/15                   *
 ****************************************/

//Warning::In this version, the project will work correctly only when laser_info's angle is in asc!

#include "ros/ros.h"

//sub folder && ...
#include "graphic/graphic.hpp"
#include "laser/laser.hpp"
#include "pyroelectricity/pyroelectricity.hpp"
#include "voice/voice.hpp"

//message
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Vector3.h"
#include "bprobot/msg_F_LIVING_STATUS.h"
#include "bprobot/msg_D_1_A_IFS_INPUT.h"
#include "bprobot/msg_G_1_A_IFS_INPUT.h"
#include "bprobot/msg_G_1_A_SENSOR_INPUT.h"
#include "bprobot/msg_G_2_A_IFS_INPUT.h"
#include "bprobot/msg_K_1_A_IFS_INPUT.h"

//rapidjson
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"

//ros
#include <ros/time.h>
#include <tf/tfMessage.h>
#include <tf/transform_broadcaster.h>

//#include <turtlesim/Pose.h>

//redis
#include <hiredis/hiredis.h>

//system
#include <iostream>
#include <ctime>
#include <cmath>
#include <stdexcept>
#include <string>
#include <boost/date_time.hpp>
#include <vector>
#include <utility>
#include <string>

//rad <-> deg
#define RAD2DEG(x)      ((x)*180./M_PI)
#define DEG2RAD(x)      ((x)*M_PI/180.)
#define INCREMENT       0


#ifndef TYPE_HUMAN
#define TYPE_HUMAN      0
#define TYPE_ANIMAL     1
#define TYPE_UNKNOWN    3
#endif

//using namespace boost;
using namespace std;
using namespace rapidjson;

class LivingStatus
{
public:

    explicit LivingStatus();
    explicit LivingStatus(int argc, char ** argv);
    ~LivingStatus();

    // ScanCallbacks
    //for laser data
    void scanCallback_laser(const sensor_msgs::LaserScan::ConstPtr& scan);
    //for gra data : face human dog cat
    void scanCallback_gra(const bprobot::msg_D_1_A_IFS_INPUT& msg);
    //for pyroelectricity data
    void scanCallback_pyroelectricity(const bprobot::msg_G_1_A_IFS_INPUT& msg);
    //for head_angle
    void scanCallback_head_angle(const bprobot::msg_G_1_A_SENSOR_INPUT& msg);
    //for human voice angle data
    void scanCallback_voice_interactive_G(const bprobot::msg_G_2_A_IFS_INPUT& msg);
    void scanCallback_voice_interactive_K(const bprobot::msg_K_1_A_IFS_INPUT& msg);
    //for Q_pose
    void scanCallback_tf_pose(const tf::tfMessage& msg);

    // Others
    //for the only one publisher
    void set_pub(ros::Publisher pub);
    //make json to be a part of data to be published
    string mkjson(bprobot::msg_F_LIVING_STATUS to_be_json);
    //uppdate to redis f_living_list
    void push_redis(string big_json_string);
    //get angle between camera and radar
    // TODO: but this cannot be determinated now (2017-1-12)
    //Watch Out +- !
    float get_increment();

private:
    //pair.first:degree  pair.second:range(distance)
//    vector<pair<float,float> > laser_info;

    vector<Graphic> gras_;
    Pyroelectricity * pyroelectricity_;
    Laser * laser_;
    pair<ros::Time, int> voice_angle_;
    //init!
    int head_angle_;
    ros::Publisher pub_;
    int argc_;
    char ** argv_;
    //init!
    string head_position_str_;
    //init!
    tf::Vector3 position_;
    SampleSetFactory factory_;
};

#endif //LIVINGSTATUS_HPP


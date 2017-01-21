/****************************************
 *      Last Edited By LiuChang         *
 *      Subcribe Two Message:           *
 *          1.From Laser                *
 *          2.From Graphic D            *
 *      Date 2017/1/3                   *
 ****************************************/

//Warning::In this version, the project will work correctly only when laser_info's angle is in asc!

#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "bprobot/F_LIVING_STATUS.h"
#include "bprobot/D_1_A_IFS_INPUT.h"
#include "bprobot/G_1_A_SENSOR_INPUT.h"
#include "bprobot/G_2_A_IFS_INPUT.h"
//#include <boost/shared_ptr.hpp>
#include <vector>
#include <utility>
#include <string>
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
//#include "redis-cplusplus-client-master/redisclient.h"
#include <iostream>
#include <ctime>
#include <cmath>
#include <tf/tfMessage.h>
#include <cstring>
#include <stdexcept>
#include <ros/time.h>
#include <string>
#include <boost/date_time.hpp>
#include <hiredis/hiredis.h>

#define RAD2DEG(x)  ((x)*180./M_PI)
#define DEG2RAD(x)  ((x)*M_PI/180.)
#define INCREMENT   0

using namespace boost;
using namespace std;
using namespace rapidjson;

class LivingStatus
{
public:
    explicit LivingStatus();
    explicit LivingStatus(int argc, char ** argv);
    ~LivingStatus();
    float get_degree_by_pixel_x(int x);
    float get_increment();
    int get_angle_allowance(int x, int width);
    vector<pair<float,float> > pick_valid(float degree, float allowance);
    int get_distance(int x, int width, int type);//type: 0-human 1-animal 3-unknown
    void scanCallback_laser(const sensor_msgs::LaserScan::ConstPtr& scan);
    void scanCallback_gra(const bprobot::D_1_A_IFS_INPUT& msg);
    void scanCallback_interative(const bprobot::G_2_A_IFS_INPUT& msg);
    float find_head_angle(string head_position_str);
    void scanCallback_head_angle(const bprobot::G_1_A_SENSOR_INPUT& msg);
    void set_pub(ros::Publisher pub);
    string mkjson(bprobot::F_LIVING_STATUS to_be_json);
    void push_redis(string big_json_string);
//    void run();

//    shared_ptr<redis::client> connect_client();
private:
    vector<pair<float,float> > laser_info;	//pair.first:degree  pair.second:range(distance)
    int head_angle;
    ros::Publisher pub;
//    shared_ptr<redis::client> cli;
    int argc;
    char ** argv;
//    bprobot::F_LIVING_STATUS to_be_json;
};


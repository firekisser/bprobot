#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <pthread.h>
#include "livingstatus/livingstatus.hpp"
#include "track/track.hpp"
#include "interpolation/interpolation.hpp"
#include "bprobot/msg_F_OPT_LIVING.h"
#include <unistd.h>

#include "testdata.hpp"

class Controller
{
public:
    explicit Controller();
    explicit Controller(int argc, char ** argv);
    ~Controller();

    void handle();                              //entry
    void living_status_exec();                  //living status
    void interpolation_exec();                  //interpolation
    void track_exec();                          //track -- not needed now -- 2017/2/7

    void scanCallback_trigger(const bprobot::msg_F_OPT_LIVING& msg);    //scanCallback of trigger

private:
    LivingStatus * ls_;                         //LivingStatus obj
    Track * tk_;                                //Track obj
    Interpolation * ip_;                        //Interpolation obj

    int argc_;                                  //argument counter in main()
    char ** argv_;                              //argument vector in main()

    ros::NodeHandle * n_;                       //node

    ros::Subscriber sub_trigger_;               //listen for trigger

    ros::Subscriber sub_laser_;                 //listen for laser
    ros::Subscriber sub_gra_;                   //listen for graphic data : face human dog cat
    ros::Subscriber sub_pyroelectricity_;       //listen for pyroelectricity data
    ros::Subscriber sub_head_angle_;            //listen for head angle
    ros::Subscriber sub_voice_interactive_K_;   //listen for voice interactive from module K
    ros::Subscriber sub_voice_interactive_G_;   //listen for voice interactive from module G
    ros::Subscriber sub_tf_pose_;               //listen for Q_pose for if the robot is moving, if moving, pyroelectricity data will be invalid

    bprobot::msg_F_OPT_LIVING trigger_msg_;     //backup of trigger_msg

};

#endif //CONTROLLER_HPP

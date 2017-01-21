#include <pthread.h>
#include "livingstatus/livingstatus.hpp"
#include "track/track.hpp"
#include "interpolation/interpolation.hpp"
#include "bprobot/F_OPT_LIVING.h"
#include <unistd.h>

class Controller
{
public:
    explicit Controller();
    explicit Controller(int argc, char ** argv);
    ~Controller();

    void handle();
    void living_status_exec();
    void interpolation_exec();
    void track_exec();
    void scanCallback_trigger(const bprobot::F_OPT_LIVING& msg);
//    static void * listen_trigger(void * arg);
private:
    LivingStatus * ls;
    Track * tk;
    Interpolation * ip;

    int argc;
    char ** argv;
    ros::NodeHandle * n;

    ros::Subscriber sub_trigger;

    ros::Subscriber sub_laser;
    ros::Subscriber sub_gra;
    ros::Subscriber sub_interative;
    ros::Subscriber sub_head_angle;

//    bprobot::F_OPT_LIVING * trigger_msg;
    bprobot::F_OPT_LIVING trigger_msg;
};

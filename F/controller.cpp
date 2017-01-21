#include "controller.hpp"

Controller::Controller()
{
    ;
}

Controller::Controller(int argc, char **argv)
{
    this->argc = argc;
    this->argv = argv;
    this->trigger_msg.type = -1;
}

Controller::~Controller()
{
    ;
}

void Controller::living_status_exec()
{
    ROS_INFO("living_status_exec");
    this->ls = new LivingStatus(argc, argv);
    sub_laser = n->subscribe<sensor_msgs::LaserScan>("/scan", 1000, &LivingStatus::scanCallback_laser, ls);
    sub_gra = n->subscribe("D_1_A_IFS_INPUT", 1000, &LivingStatus::scanCallback_gra, ls);
    sub_interative = n->subscribe("G_2_A_IFS_INPUT", 1000, &LivingStatus::scanCallback_interative, ls);
    sub_head_angle = n->subscribe("G_1_A_SENSOR_INPUT", 1000, &LivingStatus::scanCallback_head_angle, ls);
    ls->set_pub(n->advertise<bprobot::F_LIVING_STATUS>("F_LIVING_STATUS", 1000) );
    ros::Rate loop_rate(1);
    while(this->trigger_msg.type != -1 && this->trigger_msg.type == 1 && ros::ok())
    {
        cout << "living_status_exec" << endl;
        ros::spinOnce();
        loop_rate.sleep();
    }
    return;
}

//void * Controller::listen_trigger(void *arg)
//{
// //    int
//}

void Controller::interpolation_exec()
{
    this->ip = new Interpolation(argc, argv);
    ROS_INFO("interpolation_exec");
    cout << trigger_msg.type<<endl;
    return;
}

void Controller::track_exec()
{
    this->tk = new Track(argc, argv);
    ROS_INFO("track_exec");
    return;
}

void Controller::scanCallback_trigger(const bprobot::F_OPT_LIVING &msg)
{
//    this->trigger_msg = &msg;
    this->trigger_msg.time = msg.time;
    this->trigger_msg.id = msg.id;
    this->trigger_msg.module = msg.module;
    this->trigger_msg.type = msg.type;
    this->trigger_msg.data = msg.data;
    cout << "scanCallback_trigger" << this->trigger_msg.type << endl;
}

void Controller::handle()
{
    //init here
    ros::init(argc, argv, "liuchang");
    ros::NodeHandle node;
    this->n = &node;
    sub_trigger = n->subscribe("F_OPT_LIVING", 1000, &Controller::scanCallback_trigger, this);
    ros::AsyncSpinner spinner(1);
    spinner.start();
    while(true)
    {
        if(this->trigger_msg.type != -1)
        {
            cout << "trigger_msg.type:" << trigger_msg.type<<endl;
            switch(this->trigger_msg.type)
            {
                //1 ->开关对机器人周围人、交互人和动物的位置的追踪
                case 1:
                {
                    this->living_status_exec();
                }
                break;
                //2 ->实现 trigger  tracking_somebody  追踪人/人脸/人形
                case 2:
                {
                    this->track_exec();
                }
                break;
                //3 ->实现 trigger looks_interpolation  设置机器人插补形态
                case 3:
                {
                    this->interpolation_exec();
                }
                break;
                //default
                default:
                {
//                this->trigger_msg.type = 1;
                    cout << trigger_msg.type<<endl;
//                    ROS_INFO(":Error: The trigger's type is %d.", int(trigger_msg.type) );
                }
                break;
            }
        }
        else
        {
            cout <<"waitting trigger"<<endl;
        }
        sleep(1);
    }
    return;
}

//    //about ros time
//    ros::Time t;
//    t.init();
//    t = t.now();
//    cout<<t<<endl;
//    return;

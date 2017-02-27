#include "controller.hpp"

Controller::Controller()
{
    ;
}

Controller::Controller(int argc, char **argv)
{
    //init
    this->argc_ = argc;
    this->argv_ = argv;

    /* init type = -1,
     * once changed by module A,
     * should not back to -1 */
    this->trigger_msg_.type = 1;
}

Controller::~Controller()
{
    ;
}

void Controller::living_status_exec()
{
    //living_status waking up~
    ROS_INFO("living_status_exec");

    /* Create an LivingStatus object and load arguments
     * Before machine learning, they are empty*/
    this->ls_ = new LivingStatus(argc_, argv_);

    /***************
     * 5 listeners *
     ***************/
    //listen for laser
    this->sub_laser_ = this->n_->subscribe<sensor_msgs::LaserScan>("/scan", 1000, &LivingStatus::scanCallback_laser, ls_);
    //listen for graphic data : face human dog cat
    this->sub_gra_ = this->n_->subscribe("msg_D_1_A_IFS_INPUT", 1000, &LivingStatus::scanCallback_gra, ls_);
    //listen for pyroelectricity data
    this->sub_pyroelectricity_ = this->n_->subscribe("msg_G_1_A_IFS_INPUT", 1000, &LivingStatus::scanCallback_pyroelectricity, ls_);
    //listen for head angle
    this->sub_head_angle_ = this->n_->subscribe("msg_G_1_A_SENSOR_INPUT", 1000, &LivingStatus::scanCallback_head_angle, ls_);
    //listen for voice interactive from module G and K
    this->sub_voice_interactive_G_ = this->n_->subscribe("msg_G_2_A_IFS_INPUT", 1000, &LivingStatus::scanCallback_voice_interactive_G, ls_);
    this->sub_voice_interactive_K_ = this->n_->subscribe("msg_K_1_A_IFS_INPUT", 1000, &LivingStatus::scanCallback_voice_interactive_K, ls_);
    //listen for Q_pose for if the robot is moving, if moving, pyroelectricity data will be invalid
    this->sub_tf_pose_ = this->n_->subscribe("/Q_pose", 1000, &LivingStatus::scanCallback_tf_pose, ls_);

    /***************
     * 1 publisher *
     ***************/
    //pub living status
    ls_->set_pub(n_->advertise<bprobot::msg_F_LIVING_STATUS>("msg_F_LIVING_STATUS", 1000) );

    //loop 1Hz
    ros::Rate loop_rate(1);

    /* if trigger_msg.type == -1,
     * module A has not send trigger message yet,
     * which shouldn't happen
     */
    /* BTW, the trigger_msg is being updated in another thread.
     * if this->trigger_msg.type != 1,
     * trigger_msg.type has been updated differently,
     * then, enter the loop in void Controller::handle() again
     */
    while(this->trigger_msg_.type != -1 && this->trigger_msg_.type == 1 && ros::ok())
    {
        cout << "living_status_exec" << endl;
        ros::spinOnce();
        loop_rate.sleep();
    }
    return;
}


//not needed now
void Controller::interpolation_exec()
{
    this->ip_ = new Interpolation(argc_, argv_);
    ROS_INFO("interpolation_exec");
    cout << trigger_msg_.type<<endl;
    return;
}

//not needed now
void Controller::track_exec()
{
    this->tk_ = new Track(argc_, argv_);
    ROS_INFO("track_exec");
    return;
}

/**
 * a thread specially for trigger's callback
 * which come to life by ros::AsyncSpinner
 */
void Controller::scanCallback_trigger(const bprobot::msg_F_OPT_LIVING &msg)
{
    //update this->trigger_msg
    this->trigger_msg_.time = msg.time;
    this->trigger_msg_.id = msg.id;
    this->trigger_msg_.module = msg.module;
    this->trigger_msg_.type = msg.type;
    this->trigger_msg_.data = msg.data;
    cout << "scanCallback_trigger" << this->trigger_msg_.type << endl;
}

//the handle, the controller kernel of module F
void Controller::handle()
{
    //init arguments
    ros::init(argc_, argv_, "liuchang");
    ros::NodeHandle node;

    //init n
    this->n_ = &node;

    //listen for trigger
    sub_trigger_ = n_->subscribe("msg_F_OPT_LIVING", 1000, &Controller::scanCallback_trigger, this);

    //a thread specially for trigger's callback
    ros::AsyncSpinner spinner(1);
    spinner.start();

/*************************************************************************************************/
/******************************************TEST_AREA**********************************************/
/*************************************************************************************************/

//    float test_data[360] = {TEST_LASER_DATA};
//    sensor_msgs::LaserScan test_scan;
//    test_scan.angle_min = -3.12413907051;
//    test_scan.angle_max = 3.14159274101;
//    test_scan.angle_increment = 0.0174532923847;
//    test_scan.range_min = 0.15000000596;
//    test_scan.range_max = 6.0;
//    vector<float> v_test_scan(begin(test_data), end(test_data));
//    test_scan.ranges.swap(v_test_scan);

//    SampleSetFactory ssf;
//    DataMerge dm(test_scan, ssf);
//    dm.split_connected(0.06);
//    dm.remove_less_than(5);

////    if (this->argc_ > 1)
//    if(1)
//    {
////        ForestPredict f(this->argv_[1]);
//        ForestPredict f("trained_leg_detector.yaml");
//        for(list<SampleSet * >::iterator iter = dm.get_sample_set_list().begin(); \
//            iter != dm.get_sample_set_list().end(); iter++)
//        {
//            Features fe(*iter, test_scan);
//            vector<float> features = fe.get_features();
//            float probability = f.compute_probability(features);
//            cout <<"Location: "<< "(" << (*iter)->center().getX() << "+2.5)*200,(" << (*iter)->center().getY() << "+2.5)*200" << "probability: " << probability << endl;
//        }




////        for(int i = 0; i < 14; i++)
////        {
////            features.push_back(1);
////        }
////        float probability ;
////        probability = f.compute_probability(features);
////        cout << probability <<endl;
////        cout << probability <<endl;
////        cout << probability <<endl;

////        cout << probability <<endl;
////        cout << probability <<endl;
////        cout << probability <<endl;
//    }
//    else
//    {
//      printf("Please provide a trained random forests classifier as an input.\n");
//    }
//    return;

/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/

    while(true)
    {
        if(this->trigger_msg_.type != -1)
        {
            cout << "trigger_msg.type:" << trigger_msg_.type<<endl;
            switch(this->trigger_msg_.type)
            {
                //1 ->开关对机器人周围人、交互人和动物的位置的追踪
                case 1:
                {
                    //create a LivingStatus object and work when trigger_msg.type = 1
                    this->living_status_exec();
                }
                break;
                //2 ->实现 trigger  tracking_somebody  追踪人/人脸/人形
                case 2:
                {
                    //create a LivingStatus object and work when trigger_msg.type = 2
                    this->track_exec();
                }
                break;
                //3 ->实现 trigger looks_interpolation  设置机器人插补形态
                case 3:
                {
                    //create a LivingStatus object and work when trigger_msg.type = 3
                    this->interpolation_exec();
                }
                break;
                //default
                default:
                {
                    /* trigger_msg is not 1, 2 or 3,
                     * which shouldn't happen,
                     * may something wrong with module A
                     */
                    cout << trigger_msg_.type<<endl;
                    //ROS_INFO(":Error: The trigger's type is %d.", int(trigger_msg.type) );
                }
                break;
            }
        }
        // if trigger_msg.type == -1
        else
        {
            cout <<"waitting trigger"<<endl;
        }
        sleep(1);
    }
    return;
}



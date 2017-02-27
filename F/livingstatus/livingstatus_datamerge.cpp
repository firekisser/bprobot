#include "livingstatus.hpp"

void LivingStatus::scanCallback_laser(const sensor_msgs::LaserScan::ConstPtr &scan)
{
    this->laser_ = new Laser(*scan, this->factory_, 0.06);
    /*
    int count = scan->scan_time / scan->time_increment;
    //ROS_INFO("I heard a laser scan %s[%d]:", scan->header.frame_id.c_str(), count);
    //ROS_INFO("angle_range, %f, %f", RAD2DEG(scan->angle_min), RAD2DEG(scan->angle_max));    
    laser_info.clear();
    //laser_info.resize(count);
    for(int i = 0; i < count; i++)
    {
        float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
        //ROS_INFO(": [%f, %f]", degree, scan->ranges[i]);
        if(degree < 0)
        {
            /**
             * transform the degree data between -180 ~ 0 to 180 ~ 360
             * before:  0 ~ 180 , -180 ~ 0
             * after:   0 ~ 360
             * /
            degree = 360 + degree;
        }
        laser_info.push_back(make_pair(degree, scan->ranges[i]*1000) );//<degree,distance_mm>
    }
    */
}

void LivingStatus::scanCallback_gra(const bprobot::msg_D_1_A_IFS_INPUT& msg)
{
    Graphic g(this->position_, this->head_position_str_, msg);
    this->gras_.push_back(g);

    //delete data outdate in case memory leak
    vector<Graphic>::iterator iter;
    for(iter = this->gras_.begin(); iter != this->gras_.end(); iter++)
    {
        if((msg.t - (*iter).get_time()) >= 2)
        {
            this->gras_.erase(iter);
            iter = this->gras_.begin();
        }
    }

    vector<pair<int, GraResult> > gra_results(g.get_gra_results() );
    bprobot::msg_F_LIVING_STATUS output;    //to be published
    output.time = output.time.now();
    for(int i = 0; i < gra_results.size(); i++ )
    {
        GraResult * grf = &gra_results[i].second;

        Document document;  //rapidjson document
        Document::AllocatorType& allocator = document.GetAllocator();
        Value root(kObjectType);

        Value id;
        Value type;
        Value direct;
        Value gap;
        Value position;
        Value b;

        id.SetInt(grf->getDetectInfo()->get_id());
        type.SetInt(grf->getDetectInfo()->get_type());
        direct.SetInt(grf->getDetectInfo()->get_direct());
        gap.SetInt(grf->getDetectInfo()->get_gap());
//        position.SetString(grf->getDetectInfo()->get_position());
        const char * pos = grf->getDetectInfo()->get_position().c_str();
        position.SetString(pos, strlen(pos) );
        b.SetInt(grf->getDetectInfo()->get_b());

        root.AddMember("id", id, allocator);
        root.AddMember("type", type, allocator);
        root.AddMember("direct", direct, allocator);
        root.AddMember("gap", gap, allocator);
        root.AddMember("position", position, allocator);
        root.AddMember("b", b, allocator);

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        root.Accept(writer);
        std::string result = buffer.GetString();
        output.targets.push_back(result);

        this->push_redis(this->mkjson(output) );
        pub_.publish(output);
//        switch(gra_results[i].first)
//        {
//        case GRA_RESULT_FACE:
//            break;
//        case GRA_RESULT_HUMAN:
//            break;
//        case GRA_RESULT_PET:
//            break;
//        default:
//            break;
//        }

    }
}

void LivingStatus::scanCallback_voice_interactive_G(const bprobot::msg_G_2_A_IFS_INPUT& msg)
{
    if(msg.a == -1)
    {
        ;//cannot find interative people,ignore it
    }
    else
    {
        this->voice_angle_.first = msg.time;
        this->voice_angle_.second = msg.a;
    }
}

void LivingStatus::scanCallback_voice_interactive_K(const bprobot::msg_K_1_A_IFS_INPUT& msg)
{
    if(msg.a == -1)
    {
        ;//cannot find interative people,ignore it
    }
    else
    {
        this->voice_angle_.first = msg.t;
        this->voice_angle_.second = msg.a;
    }
}

void LivingStatus::scanCallback_head_angle(const bprobot::msg_G_1_A_SENSOR_INPUT& msg)
{
    this->head_position_str_ = msg.arm;
}

void LivingStatus::scanCallback_pyroelectricity(const bprobot::msg_G_1_A_IFS_INPUT &msg)
{
    vector<int> py;
    for(int i = 0; i < msg.py.size(); i++)
    {
        py.push_back(msg.py[i]);
    }
    this->pyroelectricity_ = new Pyroelectricity(msg.t, py);

}


//pick transforms[0]
//pick up location and if it is static
void LivingStatus::scanCallback_tf_pose(const tf::tfMessage &msg)
{
    this->position_.setValue(msg.transforms[0].transform.translation.x, \
            msg.transforms[0].transform.translation.y, \
            msg.transforms[0].transform.translation.z );
}

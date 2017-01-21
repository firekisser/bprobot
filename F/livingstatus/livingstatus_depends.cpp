#include "livingstatus.hpp"

LivingStatus::LivingStatus()
{
    ;
}

LivingStatus::LivingStatus(int argc, char ** argv)
{
    this->argc = argc;
    this->argv = argv;
//    this->cli = connect_client();
}

LivingStatus::~LivingStatus()
{
    ;
}

float LivingStatus::get_degree_by_pixel_x(int x)
{
    float oa = 320./sin(DEG2RAD(27./2.) );
    float om = sqrt(x*x + oa*oa -2*x*oa*cos(DEG2RAD((180-27.)/2.) ) );
    return RAD2DEG(asin(x*sin(DEG2RAD(180-27.)/2.)/om ) );
}

//get angle between camera and radar
//but this cannot be determinated now (2017-1-12)
//Watch Out +- !
float LivingStatus::get_increment()
{
//    tf::tfMessage::ConstPtr& t;
//    t->transforms.value_type.transform.translation.x;
    return INCREMENT;
}

int LivingStatus::get_angle_allowance(int x, int width)
{
    return (get_degree_by_pixel_x(x+width) - get_degree_by_pixel_x(x))/2. + 1;
}

vector<pair<float,float> > LivingStatus::pick_valid(float degree, float allowance)
{
    degree = int(degree) % 360;
    vector<pair<float,float> > valid_laser_data;
//    cout<<"cp2....1"<<endl;
    if(degree - allowance < 0)
    {
//        cout<<"cp2....2"<<endl;
        //0 ~ 360
        if(laser_info.size() == 0)
        {
//            cout<<"cp2s2"<<endl;
            return valid_laser_data;
        }
        int low = laser_info.size() - 1;
        while (laser_info[low].first > degree - allowance + 360)
        {
            low--;
        }
        int high = 0;
//        cout<<"cp2....x2"<<endl;
        while (laser_info[high].first < degree + allowance)
        {
            high++;
        }
//        cout<<"cp2....xx2"<<endl;
        for(int i = low; i < laser_info.size() + high; i++)
        {
            valid_laser_data.push_back(laser_info[i%laser_info.size() ] );
        }
    }
    else if(degree + allowance > 360)
    {
//        cout<<"cp2....3"<<endl;
        int low = laser_info.size() - 1;
        while (laser_info[low].first > degree - allowance)
        {
            low--;
        }
        int high = 0;
        while (laser_info[high].first < degree + allowance - 360)
        {
            high++;
        }
        for(int i = low; i < laser_info.size() + high; i++)
        {
            valid_laser_data.push_back(laser_info[i%laser_info.size() ] );
        }
    }
    else
    {
//        cout<<"cp2....4"<<endl;
        for(int i = 0; i < laser_info.size(); i++)
        {
            if(laser_info[i].first > degree - allowance || laser_info[i].first < degree + allowance)
            {
                valid_laser_data.push_back(laser_info[i]);
            }
        }
    }
//    cout<<"cpp....2"<<endl;
    return valid_laser_data;
}

int LivingStatus::get_distance(int x, int width, int type)//type: 0-human 1-animal 3-unknown
{
    vector<pair<float, float> > valid_data(pick_valid(head_angle + get_increment(), get_angle_allowance(x, width) ) );
    if(valid_data.size() == 0)
    {
        return -1;
    }
//    cout<<"cp2...1"<<endl;
//    valid_data = pick_valid(head_angle + get_increment(), get_angle_allowance(x, width) );
    int sum_all = 0;
    for(int i = 1; i < valid_data.size(); i++)
    {
        sum_all += valid_data[i].second;
    }

    int ave = sum_all/valid_data.size();

    int sum_target = 0;
    int count = 0;

    for(int i = 1; i < valid_data.size(); i++)
    {
        if(valid_data[i].second < ave)
        sum_target += valid_data[i].second;
        count++;
    }

    return sum_target/count;
}

float LivingStatus::find_head_angle(string head_position_str)
{
    float head_position = -1;
    for(int i = head_position_str.length()-1; i > 0; i--)
    {
        if(head_position_str[i] == '_')
        {
            head_position_str = head_position_str.substr(i+1);
        }
        try
        {
            head_position = strtof(head_position_str.c_str(),NULL);
        }
        catch(std::invalid_argument& e)
        {
            ROS_INFO("no conversion could be performed");
            return -2;
        }
        catch(std::out_of_range& e)
        {
            ROS_INFO("value out of range");
            return -3;
        }
        catch(...)
        {
            ROS_INFO("head_position_str: %s",head_position_str.c_str() );
            ROS_INFO("You can you see see, no can no BB");
            return -4;
        }
    }
    cout << head_position << endl;
    return head_position;
}

void LivingStatus::set_pub(ros::Publisher pub)
{
    this->pub = pub;
}


/*
//Do not run here , run in controller
void LivingStatus::run()
{

    ros::init(argc, argv, "liuchang");
    ros::NodeHandle n;
    ros::Subscriber sub_laser = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, &LivingStatus::scanCallback_laser, this);
    ros::Subscriber sub_gra = n.subscribe("D_1_A_IFS_INPUT", 1000, &LivingStatus::scanCallback_gra, this);
    ros::Subscriber sub_interative = n.subscribe("G_2_A_IFS_INPUT", 1000, &LivingStatus::scanCallback_interative, this);
    ros::Subscriber sub_head_angle = n.subscribe("G_1_A_SENSOR_INPUT", 1000, &LivingStatus::scanCallback_head_angle, this);
    pub = n.advertise<bprobot::F_LIVING_STATUS>("F_LIVING_STATUS", 1000);
    ros::spin();

}
*/

#include "livingstatus.hpp"

void LivingStatus::scanCallback_laser(const sensor_msgs::LaserScan::ConstPtr& scan)
{

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
            degree = 360 + degree;
        }
        laser_info.push_back(make_pair(degree, scan->ranges[i]*1000) );//<degree,distance_mm>
    }
}

void LivingStatus::scanCallback_gra(const bprobot::D_1_A_IFS_INPUT& msg)
{
    //Data merge here
    int human_count = 0;
    int animal_count = 0;
    
    bprobot::F_LIVING_STATUS output;//to be published

    time_t t;
    output.time = time(&t);//follow gra time 

    human_count = msg.b.size()/6;
    animal_count = msg.o.size()/7;

    if(human_count + animal_count == 0)
	    return;

//	ROS_INFO("Before Resize, Target Size: %ld", output.targets.size());
//  output.targets.resize(human_count + animal_count);
//	ROS_INFO("After  Resize, Target Size: %ld", output.targets.size());
//    cout<<"cp1"<<endl;
    for( int i = 0; i < human_count; i++)
    {   
        Document document;
        Document::AllocatorType& allocator = document.GetAllocator();
        Value root(kObjectType);
        
        Value id;
        Value type;
        Value direct;
        Value gap;
        Value position;
        Value b;
//        cout<<"cp2"<<endl;
        id.SetInt(msg.b[6 * i + 0]);
//        cout<<"cp2..1"<<endl;
        type.SetInt(0);
        int degree = int(get_degree_by_pixel_x(msg.b[6 * i + 1] + msg.b[6 * i + 3]/2 ) + get_increment() - 27/2. ) ;
        direct.SetInt(degree);
//        cout<<"cp2..2"<<endl;
        gap.SetInt(get_distance(msg.b[6 * i + 1], msg.b[6 * i + 3], 0) );
//        cout<<"cp2..3"<<endl;
        position.SetString("");
        b.SetInt(msg.b[6 * i + 5]);
//        cout<<"cp2.1"<<endl;
        root.AddMember("id", id, allocator);
        root.AddMember("type", type, allocator);
        root.AddMember("direct", direct, allocator);
        root.AddMember("gap", gap, allocator);
        root.AddMember("position", position, allocator); 
        root.AddMember("b", b, allocator);
//        cout<<"cp3"<<endl;
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        root.Accept(writer);
        std::string result = buffer.GetString();
		//ROS_INFO("Before Push, Target Size: %ld", output.targets.size());
        output.targets.push_back(result);
		//ROS_INFO("After  Push, Target Size: %ld\n", output.targets.size());
//        cout<<"cp4"<<endl;
    }

    for(int i = 0; i < animal_count; i++)
    {
        Document document;
        Document::AllocatorType& allocator = document.GetAllocator();
        Value root(kObjectType);
        
        Value id;
        Value type;
        Value direct;
        Value gap;
        Value position;
        Value b;
        
        id.SetInt(msg.o[7 * i + 0]);
        type.SetInt(1);
        int degree = int(get_degree_by_pixel_x(msg.o[7 * i + 2] + msg.o[7 * i + 4]/2 ) + get_increment() - 27/2. );
        direct.SetInt(degree);
        gap.SetInt(get_distance(msg.o[7 * i + 2], msg.o[7 * i + 4], 1) );
        position.SetString("");
        b.SetInt(msg.o[7 * i + 6]);

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

		//ROS_INFO("Before Push, Target Size: %ld", output.targets.size());
        output.targets.push_back(result);
		//ROS_INFO("After  Push, Target Size: %ld\n", output.targets.size());
    }
//    ROS_INFO("Last Target Size: %ld\n\n", output.targets.size());
    push_redis(mkjson(output) );
    pub.publish(output);
}

void LivingStatus::scanCallback_interative(const bprobot::G_2_A_IFS_INPUT& msg)
{
    if(msg.a == -1)
    {
        ;//cannot find interative people,ignore it
    }
    else
    {
        ;//publish a valid people
    }
}

void LivingStatus::scanCallback_head_angle(const bprobot::G_1_A_SENSOR_INPUT& msg)
{
    string head_position_str = msg.arm;
    head_angle = find_head_angle(head_position_str);
}

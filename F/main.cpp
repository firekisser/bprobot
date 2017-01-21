#include "controller.hpp"

int main(int argc, char ** argv)
{
    Controller * c = new Controller(argc, argv);
    c->handle();
    return 0;
}


/*
int main(int argc, char ** argv)
{
//    for(int i = 0; i < 64; i++)
//    {
//        cout << get_degree_by_pixel_x(i*10) << '\t';
//    }
//    return 0;

//    find_head_angle("111_233_33.2_4.22");
//    ros::Time t;
//    t.init();
//    t = t.now();
//    cout<<t<<endl;
//    return 0;
//    LivingStatus * ls = new LivingStatus(argc, argv);
//    ls->run();
//    redisContext * c = redisConnect("127.0.0.1", 6379);
    LivingStatus ls(argc, argv);

    ros::init(argc, argv, "liuchang");

    ros::NodeHandle n;

    ros::Subscriber sub_laser = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, &LivingStatus::scanCallback_laser, &ls);
    ros::Subscriber sub_gra = n.subscribe("D_1_A_IFS_INPUT", 1000, &LivingStatus::scanCallback_gra, &ls);
    ros::Subscriber sub_interative = n.subscribe("G_2_A_IFS_INPUT", 1000, &LivingStatus::scanCallback_interative, &ls);
    ros::Subscriber sub_head_angle = n.subscribe("G_1_A_SENSOR_INPUT", 1000, &LivingStatus::scanCallback_head_angle, &ls);

    ls.set_pub(n.advertise<bprobot::F_LIVING_STATUS>("F_LIVING_STATUS", 1000) );

    ros::spin();
    return 0;
}
*/

//#include<hiredis/hiredis.h>

/*
int main(int argc, char **argv)
{
    struct timeval timeout = {2, 0};    //2s的超时时间
    //redisContext是Redis操作对象
    redisContext *pRedisContext = (redisContext*)redisConnectWithTimeout("127.0.0.1", 6379, timeout);
    if ( (NULL == pRedisContext) || (pRedisContext->err) )
    {
        if (pRedisContext)
        {
            std::cout << "connect error:" << pRedisContext->errstr << std::endl;
        }
        else
        {
            std::cout << "connect error: can't allocate redis context." << std::endl;
        }
        return -1;
    }
    //redisReply是Redis命令回复对象 redis返回的信息保存在redisReply对象中
    redisReply *pRedisReply = (redisReply*)redisCommand(pRedisContext, "set 1 7");  //执行INFO命令
    std::cout<< "Output:" << pRedisReply->str << std::endl;
    //当多条Redis命令使用同一个redisReply对象时
    //每一次执行完Redis命令后需要清空redisReply 以免对下一次的Redis操作造成影响
    freeReplyObject(pRedisReply);

    return 0;
}
*/

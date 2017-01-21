#include "livingstatus.hpp"

string LivingStatus::mkjson(bprobot::F_LIVING_STATUS to_be_json)
{
    Document document;
    Document::AllocatorType& allocator = document.GetAllocator();
    Value root(kObjectType);

    Value time;
    Value targets(kArrayType);

    time.SetInt64(to_be_json.time);
    for(int i = 0; i < to_be_json.targets.size(); i++)
    {
        const char * tar = to_be_json.targets[i].c_str();
//        targets.PushBack(to_be_json.targets[i], allocator);
        targets.PushBack( * tar, allocator);
    }
    root.AddMember("time", time, allocator);
    root.AddMember("targets", targets, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    root.Accept(writer);
    return buffer.GetString();
//    return "";
}

void LivingStatus::push_redis(string big_json_string)
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
        return;
    }

    //Push Here
    //redisReply是Redis命令回复对象 redis返回的信息保存在redisReply对象中
    string cmd_str = "RPUSH f_living_list " + big_json_string;
    const char * cmd = cmd_str.c_str();
    redisReply *pRedisReply = (redisReply*)redisCommand(pRedisContext, cmd);  //执行INFO命令
    std::cout << pRedisReply->str << std::endl;
    //当多条Redis命令使用同一个redisReply对象时
    //每一次执行完Redis命令后需要清空redisReply 以免对下一次的Redis操作造成影响
    freeReplyObject(pRedisReply);
    return;
}


//shared_ptr<redis::client> LivingStatus::connect_client()
//{
//    const char * c_host = getenv("REDIS_HOST");
//    string host = "localhost";
//    if(c_host)
//    {
//        host = c_host;
//    }
//    return boost::shared_ptr<redis::client>(new redis::client(host) );
//}


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
    redisReply *pRedisReply = (redisReply*)redisCommand(pRedisContext, "INFO");  //执行INFO命令
    std::cout << pRedisReply->str << std::endl;
    //当多条Redis命令使用同一个redisReply对象时
    //每一次执行完Redis命令后需要清空redisReply 以免对下一次的Redis操作造成影响
    freeReplyObject(pRedisReply);

    return 0;
}
*/

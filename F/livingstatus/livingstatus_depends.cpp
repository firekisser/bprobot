#include "livingstatus.hpp"

LivingStatus::LivingStatus()
{
    ;
}

LivingStatus::LivingStatus(int argc, char ** argv)
{
    this->argc_ = argc;
    this->argv_ = argv;
//    this->cli = connect_client();
    this->head_angle_ = -1;
    this->position_.setX(-1);
    this->position_.setY(-1);
    this->position_.setZ(-1);
}

LivingStatus::~LivingStatus()
{
    ;
}

//get angle between camera and radar
//but this cannot be determinated now (2017-1-12)
//Watch Out +- !
float LivingStatus::get_increment()
{
    return INCREMENT;
}

void LivingStatus::set_pub(ros::Publisher pub)
{
    this->pub_ = pub;
}

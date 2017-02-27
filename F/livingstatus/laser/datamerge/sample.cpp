#include "sample.hpp"

Sample * Sample::Extract(int index, const sensor_msgs::LaserScan &scan)
{
    Sample * s =  new Sample();

    s->index_ = index;
    s->range_ = scan.ranges[index];
    s->x_ = cos(scan.angle_min + index * scan.angle_increment) * s->range_;
    s->y_ = sin(scan.angle_min + index * scan.angle_increment) * s->range_;

    if(s->range_ > scan.range_min && s->range_ < scan.range_max)
    {
       return s;
    }
    else
    {
        delete s;
        return NULL;
    }
}

Sample::~Sample()
{
    ;
}

int Sample::get_index()
{
    return this->index_;
}

float Sample::get_intensity()
{
    return this->intensity_;
}

float Sample::get_range()
{
    return this->range_;
}

float Sample::get_x()
{
    return this->x_;
}

float Sample::get_y()
{
    return this->y_;
}

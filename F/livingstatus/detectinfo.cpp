#include "detectinfo.hpp"

DetectInfo::DetectInfo()
{
    ;
}

DetectInfo::DetectInfo(int id, int type, int direct, int gap, tf::Vector3 position_vector, int b)
{
    this->id_ = id;
    this->type_ = type;
    this->direct_ = direct;
    this->gap_ = gap;
    this->position_ = this->generate_position_str(position_vector);
    this->b_ = b;
}

//translate tf::Vector3(X, Y, Z) to string(X_Y_Z)
string DetectInfo::generate_position_str(tf::Vector3 position_vector)
{
    ostringstream buffer;
    buffer << position_vector.getX() << "_" << position_vector.getY() << "_" << position_vector.getZ();
    string position_str = buffer.str();
    return position_str;
}

//getters
int DetectInfo::get_id()
{
    return this->id_;
}

int DetectInfo::get_type()
{
    return this->type_;
}

int DetectInfo::get_direct()
{
    return this->direct_;
}

int DetectInfo::get_gap()
{
    return this->gap_;
}

string DetectInfo::get_position()
{
    return this->position_;
}

int DetectInfo::get_b()
{
    return this->b_;
}

void DetectInfo::set_type(int type)
{
    this->type_ = type;
}

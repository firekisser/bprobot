#include "graresultface.hpp"

GraResultFace::GraResultFace()
{
    this->age_ = -1;
    this->sex_ = -1;
//    this->GraResult(-1, -1, -1, -1, -1, -1);
}

GraResultFace::GraResultFace(int age, int sex, int id, int start_pix_x, int start_pix_y, int pix_width, int pix_hight, int probability, tf::Vector3 position) \
    :GraResult(id, start_pix_x, start_pix_y, pix_width, pix_hight, probability, position)
{
    this->age_ = age;
    this->sex_ = sex;
//    this->GraResult(id, start_pix_x, start_pix_y, pix_width, pix_hight, probability, position);
}

GraResultFace::~GraResultFace()
{
    ;
}


#include "graresultpet.hpp"

GraResultPet::GraResultPet()
{
    this->type_ = -1;
//    this->GraResult(-1, -1, -1, -1, -1, -1);
}

GraResultPet::GraResultPet(int id, int type, int start_pix_x, int start_pix_y, int pix_width, int pix_hight, int probability, tf::Vector3 position) \
    :GraResult(id, start_pix_x, start_pix_y, pix_width, pix_hight, probability, position)
{
    this->type_ = type;
//    this->GraResult(id, start_pix_x, start_pix_y, pix_width, pix_hight, probability, position);
}

GraResultPet::~GraResultPet()
{
    ;
}

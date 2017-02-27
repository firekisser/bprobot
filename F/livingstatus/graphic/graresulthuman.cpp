#include "graresulthuman.hpp"

GraResultHuman::GraResultHuman()
{
//    this->GraResult();
    ;
}

GraResultHuman::GraResultHuman(int id, int start_pix_x, int start_pix_y, int pix_width, int pix_hight, int probability, tf::Vector3 position) \
    :GraResult(id, start_pix_x, start_pix_y, pix_width, pix_hight, probability, position)
{
//    this->GraResult(id, start_pix_x, start_pix_y, pix_width, pix_hight, probability, position);
}

GraResultHuman::~GraResultHuman()
{
    ;
}

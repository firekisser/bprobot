#ifndef GRARESULTHUMAN_HPP
#define GRARESULTHUMAN_HPP

#include "graresult.hpp"

class GraResultHuman : public GraResult
{

public:

    GraResultHuman();
    GraResultHuman(int id, int start_pix_x, int start_pix_y, int pix_width, int pix_hight, int probability, tf::Vector3 position);
    ~GraResultHuman();
};

#endif //GRARESULTHUMAN_HPP

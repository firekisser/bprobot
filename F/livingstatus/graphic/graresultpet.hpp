#ifndef GRARESULTPET_HPP
#define GRARESULTPET_HPP

#include "graresult.hpp"

class GraResultPet : public GraResult
{

public:

    GraResultPet();
    GraResultPet(int id, int type, int start_pix_x, int start_pix_y, int pix_width, int pix_hight, int probability, tf::Vector3 position);
    ~GraResultPet();

private:

    int type_;
};

#endif //GRARESULTPET_HPP

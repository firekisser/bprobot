 #ifndef GRARESULTFACE_HPP
#define GRARESULTFACE_HPP

#include "graresult.hpp"

class GraResultFace : public GraResult
{

public:

    GraResultFace();
    GraResultFace(int age, int sex, int id, int start_pix_x, int start_pix_y, int pix_width, int pix_hight, int probability, tf::Vector3 position);
    ~GraResultFace();


private:

    int age_;
    int sex_;

};

#endif //GRARESULTFACE_HPP

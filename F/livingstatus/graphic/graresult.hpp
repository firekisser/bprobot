#ifndef GRARESULT_HPP
#define GRARESULT_HPP

#include "../detectinfo.hpp"

#include <cmath>
#include <iostream>
#include <tf/tfMessage.h>
#include <tf/transform_broadcaster.h>

//rad <-> deg
#define RAD2DEG(x)  ((x)*180./M_PI)
#define DEG2RAD(x)  ((x)*M_PI/180.)

#define VISUAL_ANGLE 27.

class GraResult
{

public:

    GraResult();
    GraResult(int id, int start_pix_x, int start_pix_y, int pix_width, int pix_hight, int probability, tf::Vector3 position);
    ~GraResult();

    /**
     * find the angle in this article
     * http://okr.bpeer.com/okr1/Home/Post/detail?post_id=144
     */
    float get_degree_by_pixel_x(int x);

    //get the range of the angle allowed
    int get_angle_allowance(int x, int width);

    DetectInfo * getDetectInfo();

    void set_type(int type);


protected:

    int id_;
    int start_pix_x_;
    int start_pix_y_;
    int pix_width_;
    int pix_hight_;
    int probability_;

    int allowance_;

    DetectInfo * di_;

};

#endif //GRARESULT_HPP

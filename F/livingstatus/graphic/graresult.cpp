#include "graresult.hpp"

GraResult::GraResult()
{
    this->id_ = -1;
    this->pix_hight_ = -1;
    this->pix_width_ = -1;
    this->probability_ = -1;
    this->start_pix_x_ = -1;
    this->start_pix_y_ = -1;
}

GraResult::GraResult(int id, int start_pix_x, int start_pix_y, int pix_width, int pix_hight, int probability, tf::Vector3 position)
{
    //init base data
    this->id_ = id;
    this->start_pix_x_ = start_pix_x;
    this->start_pix_y_ = start_pix_y;
    this->pix_width_ = pix_width;
    this->pix_hight_ = pix_hight;
    this->probability_ = probability;

    //calculate
    this->allowance_ = this->get_angle_allowance(this->start_pix_x_, this->pix_width_);
//    this->di.id = id;
//    this->di.type = -1; //init in subclass
//    this->di.direct = get_degree_by_pixel_x(this->start_pix_x + this->pix_width/2);
//    this->gap = -1;
//    this
    this->di_ = new DetectInfo(id, -1, \
            get_degree_by_pixel_x(this->start_pix_x_ + this->pix_width_/2), \
            -1, position, probability);
}

GraResult::~GraResult()
{
    ;
}

/**
 * find the angle in this article
 * http://okr.bpeer.com/okr1/Home/Post/detail?post_id=144
 */
float GraResult::get_degree_by_pixel_x(int x)
{
    float oa = 320./sin(DEG2RAD(VISUAL_ANGLE/2.) );
    float om = sqrt(x*x + oa*oa -2*x*oa*cos(DEG2RAD((180-VISUAL_ANGLE)/2.) ) );
    return RAD2DEG(asin(x*sin(DEG2RAD(180-VISUAL_ANGLE)/2.)/om ) );
}

//get the range of the angle allowed
int GraResult::get_angle_allowance(int x, int width)
{
    return (get_degree_by_pixel_x(x+width) - get_degree_by_pixel_x(x))/2. + 1;
}

DetectInfo * GraResult::getDetectInfo()
{
    //return detectinfo
    return this->di_;
}

void GraResult::set_type(int type)
{
    this->di_->set_type(type);
}

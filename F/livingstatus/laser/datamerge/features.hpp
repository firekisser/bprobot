#ifndef FEATURES_HPP
#define FRETURES_HPP

#include "sampleset.hpp"
#include "opencv/cxcore.h"
#include "opencv/cv.h"
#include <vector>

//using namespace std;

class Features
{
public:
    Features();
    Features(SampleSet * sample_set, const sensor_msgs::LaserScan& scan);
    ~Features();

    std::vector<float> get_features();

private:
    // values
    SampleSet * sample_set_;
    sensor_msgs::LaserScan scan_;
    std::vector<float> features_;
    int num_points_;

    //compute features
    float compute_standard_deviation();     //0
    float compute_avg_median_deviation();   //1
    float compute_prev_jump();              //2
    float compute_next_jump();              //3
    float compute_width();                  //4
    float compute_linearity();              //5
    float compute_circularity();            //6
    float compute_radius();                 //7
    float compute_boundary_length();        //8
    float compute_angle_diff();             //9
    float compute_mean_curvature();         //10
    float compute_boundary_regularity();    //11
    float compute_iav();                    //12
    float compute_standard_deviation_iav(); //13

    bool load_features();
};

#endif //FEATURES_HPP

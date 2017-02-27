#ifndef LASER_HPP
#define LASER_HPP

#include "datamerge/datamerge.hpp"
#include "datamerge/features.hpp"
#include "forestpredict/forestpredict.hpp"

#include "sensor_msgs/LaserScan.h"
#include "opencv/cxcore.h"
#include "opencv/cv.h"

#include <vector>

class Laser
{
public:
    Laser();
    Laser(const sensor_msgs::LaserScan& scan, SampleSetFactory& factory, float connected_thresh);
    ~Laser();

private:
//    DataMerge * data_merge_;
    sensor_msgs::LaserScan scan_;
    SampleSetFactory factory_;
    float connected_thresh_;
};

#endif //LASER_HPP

#ifndef SAMPLESET_HPP
#define SAMPLESET_HPP

#include "sample.hpp"

#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"
#include "geometry_msgs/Point.h"
#include "tf/transform_datatypes.h"

// An ordered set of Samples
class SampleSet : public std::set<Sample *, CompareSample>
{

public:

    SampleSet();
//    SampleSet(...);
    ~SampleSet();

    // XXX: Iteration clear, if the SampleSet is very large, this will a little slow
    void clear();

    void appendToCloud(sensor_msgs::PointCloud& cloud, int r = 0, int g = 0, int b = 0);

    tf::Point center();

};

#endif //SAMPLESET_HPP

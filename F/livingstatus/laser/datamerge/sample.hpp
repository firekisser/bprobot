#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include "sensor_msgs/LaserScan.h"

// A class representing a single sample from the laser.
class Sample
{
public:

    ~Sample();

    static Sample * Extract(int index, const sensor_msgs::LaserScan& scan);

    int get_index();
    float get_range();
    float get_intensity();
    float get_x();
    float get_y();

    int index_;


private:

    Sample(){}

    float range_;
    float intensity_;
    float x_;
    float y_;

};

// The comparator allowing the creation of an ordered "SampleSet"
struct CompareSample
{
    CompareSample() {}

    inline bool operator()(const Sample* a, const Sample* b)
    {
        return (a->index_ <  b->index_ );
    }

};

#endif //SAMPLE_HPP

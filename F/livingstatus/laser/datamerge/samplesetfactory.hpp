#ifndef SAMPLESETFACTORY_HPP
#define SAMPLESETFACTORY_HPP

#include "sampleset.hpp"

class SampleSetFactory
{

public:
    SampleSetFactory();
//    SampleSetFactory(...);
    ~SampleSetFactory();

    inline void clear()
    {
        sample_set_.clear();
        filled_ = false;
    }

    void addScan(sensor_msgs::LaserScan& scan);

    bool hasSample(Sample * sample_to_check, float little_thresh);


private:
    SampleSet sample_set_;
    bool filled_;
    float angle_min_;
    float angle_max_;
    uint32_t size_;

};

#endif //SAMPLESETFACTORY_HPP

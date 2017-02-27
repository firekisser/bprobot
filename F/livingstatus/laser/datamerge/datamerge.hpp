#ifndef DATAMERGE_HPP
#define DATAMERGE_HPP

#include "samplesetfactory.hpp"
#include <list>

class DataMerge
{

public:
    DataMerge();
    DataMerge(const sensor_msgs::LaserScan& scan, SampleSetFactory& factory, float factory_threshold = 0.03);
    ~DataMerge();

    void remove_less_than(uint32_t num);

    void split_connected(float connect_thresh);

    std::list<SampleSet * > & get_sample_set_list();

private:
    std::list<SampleSet * > sample_set_list_;
    sensor_msgs::LaserScan scan_;

};

#endif //DATAMERGE_HPP

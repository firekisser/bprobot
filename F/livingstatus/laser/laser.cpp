#include "laser.hpp"

Laser::Laser()
{
    ;
}

Laser::Laser(const sensor_msgs::LaserScan &scan, SampleSetFactory &factory, float connected_thresh)
{
    this->scan_ = scan;
    this->factory_ = factory;
    this->connected_thresh_ = connected_thresh;

    DataMerge datamerge(this->scan_, this->factory_);
    datamerge.split_connected(this->connected_thresh_);
    datamerge.remove_less_than(5);


    ForestPredict forestpredict("trained_leg_detector.yaml");
    for(std::list<SampleSet * >::iterator iter = datamerge.get_sample_set_list().begin(); \
        iter != datamerge.get_sample_set_list().end(); iter++)
    {
        Features features(*iter, this->scan_);
        std::vector<float> feature_values = features.get_features();
        float probability = forestpredict.compute_probability(feature_values);
//        std::cout <<"Location: "<< "(" << (*iter)->center().getX() << "+2.5)*200,(" << (*iter)->center().getY() << "+2.5)*200" << "probability: " << probability << std::endl;
        std::cout <<"Location: "<< "<" << (*iter)->center().getX() << "," <<(*iter)->center().getY() << ">\t" << "probability: " << probability << std::endl;
    }

}

Laser::~Laser()
{
    ;
}

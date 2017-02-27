#include "samplesetfactory.hpp"

SampleSetFactory::SampleSetFactory()
{
    this->filled_ = false;
    this->angle_min_ = 0;
    this->angle_max_ = 0;
    this->size_ = 0;
}

SampleSetFactory::~SampleSetFactory()
{
    ;
}

void SampleSetFactory::addScan(sensor_msgs::LaserScan &scan)
{
    if (!filled_)
    {
        this->angle_min_ = scan.angle_min;
        this->angle_max_ = scan.angle_max;
        this->size_ = scan.ranges.size();
        this->filled_ = true;
    }
    else if(this->angle_min_ != scan.angle_min \
            || this->angle_max_ != scan.angle_max \
            || this->size_ != scan.ranges.size() )
    {
        throw std::runtime_error("laser_scan::ScanMask::addScan: inconsistantly sized scans added to mask");
    }

    for( uint32_t i = 0; i < scan.ranges.size(); i++)
    {
        Sample * sample_tmp = Sample::Extract(i, scan);
        if(sample_tmp != NULL)
        {
            SampleSet::iterator find_result = this->sample_set_.find(sample_tmp);
            if(find_result != this->sample_set_.end() )
            {
                //this will not happen in general
                std::cout << "### Debug : find out why come here" << std::endl;
                if((*find_result)->get_range() > sample_tmp->get_range())
                {
                    delete(*find_result);
                    this->sample_set_.erase(find_result);
                    this->sample_set_.insert(sample_tmp);
                }
                else
                {
                    delete sample_tmp;
                }
            }
            else
            {
                this->sample_set_.insert(sample_tmp);
            }
        }
    }
}

bool SampleSetFactory::hasSample(Sample *sample_to_check, float little_thresh)
{
    if(sample_to_check != NULL)
    {
        SampleSet::iterator find_result = this->sample_set_.find(sample_to_check);
        if(find_result != this->sample_set_.end() )
        {
            if(((*find_result)->get_range() ) < sample_to_check->get_range() )
            {
                return true;
            }
        }
    }
    return false;
}

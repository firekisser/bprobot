#include "datamerge.hpp"

DataMerge::DataMerge()
{
    ;
}

DataMerge::DataMerge(const sensor_msgs::LaserScan &scan, SampleSetFactory &factory, float factory_threshold)
{
    this->scan_ = scan;

    SampleSet * sample_set = new SampleSet;

    //all scan data loaded here
    //add_scan unused

    for(uint32_t index = 0; index < scan.ranges.size(); index++)
    {
        Sample * sample_tmp = Sample::Extract(index, scan);

        if(sample_tmp != NULL)
        {
            if(!factory.hasSample(sample_tmp, factory_threshold))
            {
                sample_set->insert(sample_tmp);
            }
            else
            {
                delete sample_tmp;
            }
        }
    }
    this->sample_set_list_.push_back(sample_set);

}

DataMerge::~DataMerge()
{
    for(std::list<SampleSet * >::iterator sample_set_iter = this->sample_set_list_.begin(); \
        sample_set_iter != this->sample_set_list_.end(); sample_set_iter++)
    {
        delete(* sample_set_iter);
    }
}

void DataMerge::remove_less_than(uint32_t num)
{
    std::list<SampleSet * >::iterator iter = this->sample_set_list_.begin();
    while(iter != this->sample_set_list_.end() )
    {
        if((*iter)->size() < num )
        {
            delete(*iter);
            this->sample_set_list_.erase(iter++);
        }
        else
        {
            ++iter;
        }
    }
}

void DataMerge::split_connected(float connect_thresh)
{
    std::list<SampleSet * > tmp_sample_set_list;
    std::list<SampleSet * >::iterator sample_set_list_iter = this->sample_set_list_.begin();
    // for each sampleset
    while(sample_set_list_iter != this->sample_set_list_.end())
    {
        // go through the entire list
        while((*sample_set_list_iter)->size() > 0)
        {
            /**
             * take the first element,
             * actually the type of "first_sample" is sample*
             */
            SampleSet::iterator first_sample = (*sample_set_list_iter)->begin();

            //start a new queue
            std::list<Sample * > sample_queue;
            sample_queue.push_back(*first_sample);

            (*sample_set_list_iter)->erase(first_sample);

            //Grow until we get to the end of the queue
            std::list<Sample * >::iterator sample_queue_iter = sample_queue.begin();

            while(sample_queue_iter != sample_queue.end() )
            {
                int expand = (int)(asin(connect_thresh / (*sample_queue_iter)->get_range() ) \
                                   / std::abs(this->scan_.angle_increment) );

                SampleSet::iterator sample_rest_iter = (*sample_set_list_iter)->begin();

                while((sample_rest_iter != (*sample_set_list_iter)->end() && \
                       (*sample_rest_iter)->get_index() < (*sample_queue_iter)->get_index() + expand ) )
                {
                    if((*sample_rest_iter)->get_range() - (*sample_queue_iter)->get_range() > connect_thresh)
                    {
                        //next point not connected
                        break;
                    }
                    else if( sqrt(  pow( (*sample_queue_iter)->get_x() - (*sample_rest_iter)->get_x(), 2.0f) \
                                  + pow( (*sample_queue_iter)->get_y() - (*sample_rest_iter)->get_y(), 2.0f) ) \
                              < connect_thresh )
                    {
                        //point connected
                        sample_queue.push_back(*sample_rest_iter);
                        (*sample_set_list_iter)->erase(sample_rest_iter++);
                        break;
                    }
                    else
                    {
                        ++sample_rest_iter;
                    }
                }
                ++sample_queue_iter;
            }

            //move all the samples into the new sampleset
            SampleSet * tmp_sample_set = new SampleSet();
            for(sample_queue_iter = sample_queue.begin(); sample_queue_iter != sample_queue.end(); sample_queue_iter++)
            {
                tmp_sample_set->insert(*sample_queue_iter);
            }

            // store the temporary sample set list
            tmp_sample_set_list.push_back(tmp_sample_set);
        }

        //now that sample_set_list_iter is out of use, we can delete
        delete(* sample_set_list_iter);

        //and remove from the map
        this->sample_set_list_.erase(sample_set_list_iter++);
    }

    this->sample_set_list_.insert(this->sample_set_list_.begin(), tmp_sample_set_list.begin(), tmp_sample_set_list.end());

}

std::list<SampleSet * > & DataMerge::get_sample_set_list()
{
    return this->sample_set_list_;
}

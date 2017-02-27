#include "pyroelectricity.hpp"

Pyroelectricity::Pyroelectricity()
{
    ;
}

Pyroelectricity::Pyroelectricity(ros::Time time, vector<int> pyro)
{
    this->time_ = time;
    this->pyro_.swap(pyro);
    this->pyro_info_init();
    this->make_intervals();
    this->merge_intervals();
}

Pyroelectricity::~Pyroelectricity()
{
    ;
}

void::Pyroelectricity::pyro_info_init()
{
    for(int i = 0; i < 6; i++)
    {
        this->pyro_info_.push_back(make_tuple(0, 130-60, 130+60));
        this->pyro_info_.push_back(make_tuple(1, 81-60,  81+60));
        this->pyro_info_.push_back(make_tuple(2, 33-60,  33+60));
        this->pyro_info_.push_back(make_tuple(3, 327-60, 327+60));
        this->pyro_info_.push_back(make_tuple(4, 279-60, 279+60));
        this->pyro_info_.push_back(make_tuple(5, 230-60, 230+60));
    }
}

void::Pyroelectricity::make_intervals()
{
    for(int i = 0; i < this->pyro_.size()/2; i++)
    {
        if(this->pyro_[i*2+1] == 1)
        {
            this->intervals_.push_back(make_pair( \
                    get<1>(this->pyro_info_[this->pyro_[i*2+0] ]), \
                    get<2>(this->pyro_info_[this->pyro_[i*2+0] ]) ) );
        }
    }
}

void::Pyroelectricity::merge_intervals()
{
    for(int i = 0; i < this->intervals_.size(); i++)
    {
        if(this->intervals_[i].first < 0)
        {
            this->intervals_.push_back(make_pair(360+intervals_[i].first, 360) );
            this->intervals_[i].first = 0;
        }
        if(this->intervals_[i].second > 360)
        {
            this->intervals_.push_back(make_pair(0, intervals_[i].second-360) );
            this->intervals_[i].second = 360;
        }
    }
//    for(int i = 0; i < this->intervals_.size(); i++)
//    {
//        cout << "<" << intervals_[i].first << "," << intervals_[i].second << ">" << endl;
//    }
}

bool Pyroelectricity::is_someone_here(int direct)
{
    for(int i = 0; i < this->intervals_.size(); i++)
    {
        if(direct >= this->intervals_[i].first \
                && direct <= this->intervals_[i].second)
        {
            return true;
        }
    }
    return false;
}

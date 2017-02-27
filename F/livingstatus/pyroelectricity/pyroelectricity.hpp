#ifndef PYROELECTRICITY_HPP
#define PYROELECTRICITY_HPP

#include <vector>
#include <tuple>
#include <ros/ros.h>

using namespace std;


class Pyroelectricity
{

public:

    Pyroelectricity();
    Pyroelectricity(ros::Time time, vector<int> pyro);
    ~Pyroelectricity();

    //load 6 pyro info(their id, )
    void pyro_info_init();
    void make_intervals();
    void merge_intervals();
    bool is_someone_here(int direct);


private:

    ros::Time time_;
    vector<int> pyro_;
    vector<tuple<int,int,int> > pyro_info_;
    vector<pair<int, int> > intervals_;
};

#endif //PYROELECTRICITY_HPP

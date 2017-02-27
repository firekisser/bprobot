#include "sampleset.hpp"

SampleSet::SampleSet()
{
    ;
}

SampleSet::~SampleSet()
{
    clear();
}

// XXX: Iteration clear, if the SampleSet is very large, this will a little slow
void SampleSet::clear()
{
    for(SampleSet::iterator iter = this->begin(); iter != this->end(); iter++)
    {
        delete(*iter);
    }
    set<Sample *, CompareSample>::clear();
}

void SampleSet::appendToCloud(sensor_msgs::PointCloud &cloud, int r, int g, int b)
{
    float color_val = 0;

    //convert rgb to hex and merge together
    int rgb = (r << 16) | (g << 8) | b;

    //这个&不是与运算符，而是取地址符，从后往前看，把rgb取地址，
    //然后转换成float *类型，然后在通过指针 * 运算赋值给float变量
    color_val = *(float*) & (rgb);

    for(iterator sample_iter = this->begin(); sample_iter != this->end(); sample_iter++)
    {
        geometry_msgs::Point32 point;
        point.x = (*sample_iter)->get_x();
        point.y = (*sample_iter)->get_y();
        point.z = 0;

        cloud.points.push_back(point);

        if(cloud.channels[0].name == "rgb")
        {
            cloud.channels[0].values.push_back(color_val);
        }
    }
}

tf::Point SampleSet::center()
{
    float x_mean = 0.0;
    float y_mean = 0.0;
    for(iterator iter = this->begin(); iter != this->end(); iter++)
    {
        x_mean += ((*iter)->get_x() ) / size();
        y_mean += ((*iter)->get_y() ) / size();
    }
    return tf::Point(x_mean, y_mean, 0.0);
}

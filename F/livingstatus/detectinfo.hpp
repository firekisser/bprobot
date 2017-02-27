#ifndef DETECTINFO_HPP
#define DETECTINFO_HPP

#include <sstream>
#include <cstring>
#include <tf/transform_broadcaster.h>

using namespace std;

//this class conclude all info needed in json to be published
class DetectInfo
{

public:

    DetectInfo();
    DetectInfo(int id, int type, int direct, int gap, tf::Vector3 position_vector, int b);

    //translate tf::Vector3(X, Y, Z) to string("X_Y_Z")
    string generate_position_str(tf::Vector3 position_vector);

    int get_id();
    int get_type();
    int get_direct();
    int get_gap();
    string get_position();
    int get_b();

    void set_type(int type);


private:

    int id_;
    int type_;
    int direct_;
    int gap_;
    string position_;
    int b_;

};

#endif //DETECTINFO_HPP

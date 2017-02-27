#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include <iostream>
#include <vector>
#include <string>

#include "bprobot/msg_D_1_A_IFS_INPUT.h"

#include "graresult.hpp"
#include "graresultface.hpp"
#include "graresulthuman.hpp"
#include "graresultpet.hpp"

#define GRA_RESULT_FACE     0
#define GRA_RESULT_HUMAN    1
#define GRA_RESULT_PET      2

#ifndef TYPE_HUMAN
#define TYPE_HUMAN      0
#define TYPE_ANIMAL     1
#define TYPE_UNKNOWN    3
#endif

using namespace std;

class Graphic
{

public:

    explicit Graphic();

    //load data
    explicit Graphic(tf::Vector3 position, string head_position_str, bprobot::msg_D_1_A_IFS_INPUT gra_msg);
    ~Graphic();

    void load_data(int type, GraResult gr);

    /**
     * decode the head_angle from the string
     * confirm the format of
     * X头部垂直高度坐标_Y头部左右水平坐标_Z头部点抬头_K头部左右看角度
     */
    int find_head_angle(string head_position_str);

    //decode data from gra_msg and load them to gra_results
    void decode_n_load(bprobot::msg_D_1_A_IFS_INPUT gra_msg);

    vector<pair<int, GraResult> > get_gra_results();

    std::time_t get_time();


private:

    //data loaded
    std::string head_position_str_;
    bprobot::msg_D_1_A_IFS_INPUT gra_msg_;
    float head_angle_;

    std::time_t t_;
    vector<pair<int, GraResult> >  gra_results_;

    tf::Vector3 position_;
};

#endif //GRAPHIC_HPP

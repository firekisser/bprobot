#include "graphic.hpp"

Graphic::Graphic()
{
    ;
}

//load data
Graphic::Graphic(tf::Vector3 position, string head_position_str, bprobot::msg_D_1_A_IFS_INPUT gra_msg)
{
    this->position_.setValue(position.x(), position.y(), position.z() );

    this->head_position_str_ = head_position_str;
    this->gra_msg_ = gra_msg;
    this->head_angle_ = this->find_head_angle(head_position_str);
    decode_n_load(gra_msg);

    this->t_ = gra_msg.t;

    return;

}

Graphic::~Graphic()
{
    ;
}

void Graphic::decode_n_load(bprobot::msg_D_1_A_IFS_INPUT gra_msg)
{
    //faces
    for(int i = 0; i < gra_msg.f.size()/8; i++)
    {
        GraResultFace grf( gra_msg.f[i * 8 + 0], gra_msg.f[i * 8 + 1], \
                gra_msg.f[i * 8 + 2], gra_msg.f[i * 8 + 3], \
                gra_msg.f[i * 8 + 4], gra_msg.f[i * 8 + 5], \
                gra_msg.f[i * 8 + 6], gra_msg.f[i * 8 + 7], this->position_);
        grf.set_type(TYPE_HUMAN);
        load_data(GRA_RESULT_FACE, grf);
    }
    //human
    for(int i = 0; i < gra_msg.b.size()/6; i++)
    {
        GraResultHuman grh( gra_msg.b[i * 6 + 0], gra_msg.b[i * 6 + 1], \
                gra_msg.b[i * 6 + 2], gra_msg.b[i * 6 + 3], \
                gra_msg.b[i * 6 + 4], gra_msg.b[i * 6 + 5], this->position_);
        grh.set_type(TYPE_HUMAN);
        load_data(GRA_RESULT_HUMAN, grh);
    }
    //pet
    for(int i = 0; i < gra_msg.o.size()/7; i++)
    {
        GraResultPet grp( gra_msg.o[i * 7 + 0], gra_msg.o[i * 7 + 1], \
                gra_msg.o[i * 7 + 2], gra_msg.o[i * 7 + 3], \
                gra_msg.o[i * 7 + 4], gra_msg.o[i * 7 + 5], \
                gra_msg.o[i * 7 + 6], this->position_);
        grp.set_type(TYPE_ANIMAL);
        load_data(GRA_RESULT_PET, grp);
    }
    return;
}

void Graphic::load_data(int type, GraResult gr)
{
    switch(type)
    {
    case GRA_RESULT_FACE:
        this->gra_results_.push_back(make_pair(GRA_RESULT_FACE, gr) );
        break;
    case GRA_RESULT_HUMAN:
        this->gra_results_.push_back(make_pair(GRA_RESULT_HUMAN, gr) );
        break;
    case GRA_RESULT_PET:
        this->gra_results_.push_back(make_pair(GRA_RESULT_PET, gr) );
        break;
    default:
        break;
    }
    return;
}

/**
 * decode the head_angle from the string
 * confirm the format of
 * X头部垂直高度坐标_Y头部左右水平坐标_Z头部点抬头_K头部左右看角度
 */
int Graphic::find_head_angle(string head_position_str)
{
    int head_position = -1;
    for(int i = head_position_str.length()-1; i > 0; i--)
    {
        if(head_position_str[i] == '_')
        {
            head_position_str = head_position_str.substr(i+1);
        }
        try
        {
            head_position = strtof(head_position_str.c_str(),NULL);
        }
        catch(std::invalid_argument& e)
        {
            ROS_INFO("no conversion could be performed");
            return -2;
        }
        catch(std::out_of_range& e)
        {
            ROS_INFO("value out of range");
            return -3;
        }
        catch(...)
        {
            ROS_INFO("head_position_str: %s",head_position_str.c_str() );
            ROS_INFO("You can you see see, no can no BB");
            return -4;
        }
    }
    cout << head_position << endl;
    return head_position;
}

vector<pair<int, GraResult> > Graphic::get_gra_results()
{
    return this->gra_results_;
}

std::time_t Graphic::get_time()
{
    return this->t_;
}

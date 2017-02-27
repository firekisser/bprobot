#ifndef FORESTPREDICT_HPP
#define FORESTPREDICT_HPP

#include "opencv/cxcore.h"
#include "opencv/cv.h"
#include "opencv/ml.h"

#include <string>
#include <iostream>

class ForestPredict
{
public:
    ForestPredict();
    ForestPredict(const char* filename);//load trained tree;
    ~ForestPredict();

    float compute_probability(std::vector<float> features);

private:
    CvRTrees forest_;
    int feature_count_;
};

#endif //FORESTPREDICT_HPP

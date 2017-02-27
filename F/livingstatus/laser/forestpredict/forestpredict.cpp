#include "forestpredict.hpp"

ForestPredict::ForestPredict()
{
    ;
}

ForestPredict::ForestPredict(const char* filename)
{
//    "trained_leg_detector.yaml"
    std::cout << filename << std::endl;
//    this->forest_.load(trained_forest.c_str());
    this->forest_.load(filename);
    std::cout << filename << std::endl;
//    std::cout << this->forest_.get_train_error();
    std::cout << "sss" << std::endl;
    this->feature_count_ = this->forest_.get_active_var_mask()->cols;
    std::cout << filename << std::endl;
    std::cout << "Loaded forest with "  \
              << this->feature_count_   \
              << " features: "          \
              << filename               \
              << std::endl;
}

ForestPredict::~ForestPredict()
{
    ;
}

float ForestPredict::compute_probability(std::vector<float> features)
{
    CvMat * feature_mat = cvCreateMat(1, this->feature_count_, CV_32FC1);
    for(int i = 0; i < this->feature_count_; i++)
    {
        feature_mat->data.fl[i] = (float)(features[i]);
    }
    float probability = this->forest_.predict_prob(feature_mat);
    return probability;
}

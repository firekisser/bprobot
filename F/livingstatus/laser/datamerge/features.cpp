#include "features.hpp"

Features::Features()
{
    ;
}

Features::Features(SampleSet *sample_set, const sensor_msgs::LaserScan &scan)
{
    this->sample_set_ = sample_set;
    this->scan_ = scan;
    this->num_points_ = sample_set->size();

    if(this->load_features() == false)
    {
        std::cout << "Error : Features are not enough." << std::endl;
    }

//    std::cout << "Mine" << std::endl;
//    for(std::vector<float>::iterator iter = this->features_.begin(); iter != this->features_.end(); iter ++)
//    {
//        std::cout << *iter << " ";
//    }
//    std::cout << std::endl;
}

Features::~Features()
{
    ;
}

std::vector<float> Features::get_features()
{
    return this->features_;
}

float Features::compute_standard_deviation()     //0
{
    float x_mean = 0.0;
    float y_mean = 0.0;
    for(SampleSet::iterator iter = this->sample_set_->begin(); \
        iter != this->sample_set_->end(); iter++)
    {
        x_mean += ((*iter)->get_x()) / this->num_points_;
        y_mean += ((*iter)->get_y()) / this->num_points_;
    }

    double sum_std_diff = 0.0;
    for(SampleSet::iterator iter = this->sample_set_->begin(); \
        iter != this->sample_set_->end(); iter++)
    {
        sum_std_diff += pow((*iter)->get_x() - x_mean, 2.0f) + \
                        pow((*iter)->get_y() - y_mean, 2.0f);
    }

    float standard_deviation = sqrt(1.0 / (this->num_points_ - 1.0f) * sum_std_diff);
    return standard_deviation;
}

float Features::compute_avg_median_deviation()   //1
{
    std::vector<float> x_median_set;
    std::vector<float> y_median_set;
    for(SampleSet::iterator iter = this->sample_set_->begin(); \
        iter != this->sample_set_->end(); iter++)
    {
        x_median_set.push_back((*iter)->get_x());
        y_median_set.push_back((*iter)->get_y());
    }

    std::sort(x_median_set.begin(), x_median_set.end());
    std::sort(y_median_set.begin(), y_median_set.end());

    float x_median = 0.5 * (*(x_median_set.begin() + (this->num_points_ - 1) / 2) + \
                            *(x_median_set.begin() + this->num_points_ / 2));
    float y_median = 0.5 * (*(y_median_set.begin() + (this->num_points_ - 1) / 2) + \
                            *(y_median_set.begin() + this->num_points_ / 2));

    double sum_med_diff = 0.0;
    for(SampleSet::iterator iter = this->sample_set_->begin(); \
        iter != this->sample_set_->end(); iter++)
    {
        sum_med_diff += sqrt(pow((*iter)->get_x() - x_median, 2.0f) + \
                             pow((*iter)->get_y() - y_median, 2.0f));
    }

    float avg_median_deviation = sum_med_diff / this->num_points_;
    return avg_median_deviation;
}

float Features::compute_prev_jump()              //2
{
    SampleSet::iterator first = this->sample_set_->begin();
    int prev_index = (*first)->get_index() - 1;
    float prev_jump = 0;
    if(prev_index >= 0)
    {
        Sample * prev = Sample::Extract(prev_index, this->scan_);
        if(prev)
        {
            prev_jump = sqrt(pow((*first)->get_x() - prev->get_x(), 2.0f) + \
                             pow((*first)->get_y() - prev->get_y(), 2.0f));
            delete prev;
        }
    }
    return prev_jump;
}

float Features::compute_next_jump()              //3
{
    SampleSet::iterator last  = this->sample_set_->end();
    last--;
    int next_index = (*last)->get_index() + 1;
    float next_jump = 0;
    if(next_index < this->scan_.ranges.size())
    {
        Sample * next = Sample::Extract(next_index, this->scan_);
        if(next)
        {
            next_jump = sqrt(pow((*last)->get_x() - next->get_x(), 2.0f) + \
                             pow((*last)->get_y() - next->get_y(), 2.0f));
            delete next;
        }
    }
    return next_jump;
}

float Features::compute_width()                  //4
{
    float width = 0.0;
    SampleSet::iterator first = this->sample_set_->begin();
    SampleSet::iterator last  = this->sample_set_->end();
    last--;
    width = sqrt(pow((*first)->get_x() - (*last)->get_x(), 2.0f) + \
                 pow((*first)->get_y() - (*last)->get_y(), 2.0f));
    return width;
}

float Features::compute_linearity()              //5
{
    CvMat * points = cvCreateMat(this->num_points_, 2 , CV_64FC1);

    float x_mean = 0.0;
    float y_mean = 0.0;
    for(SampleSet::iterator iter = this->sample_set_->begin(); \
        iter != this->sample_set_->end(); iter++)
    {
        x_mean += ((*iter)->get_x()) / this->num_points_;
        y_mean += ((*iter)->get_y()) / this->num_points_;
    }

    int j = 0;
    for(SampleSet::iterator iter = this->sample_set_->begin(); \
            iter != this->sample_set_->end(); iter++)
    {
        cvmSet(points, j, 0, (*iter)->get_x() - x_mean);
        cvmSet(points, j, 1, (*iter)->get_y() - y_mean);
        j++;
    }

    CvMat* W = cvCreateMat(2, 2, CV_64FC1);
    CvMat* U = cvCreateMat(this->num_points_, 2, CV_64FC1);
    CvMat* V = cvCreateMat(2, 2, CV_64FC1);
    cvSVD(points, W, U, V);

    CvMat * rot_points = cvCreateMat(this->num_points_, 2, CV_64FC1);
    cvMatMul(U, W, rot_points);

    float linearity = 0.0;
    for(int i = 0; i < this->num_points_; i++)
    {
        linearity += pow(cvmGet(rot_points, i, 1), 2);
    }

    cvReleaseMat(&points);
    points = 0;
    cvReleaseMat(&W);
    W = 0;
    cvReleaseMat(&U);
    U = 0;
    cvReleaseMat(&V);
    V = 0;
    cvReleaseMat(&rot_points);
    rot_points = 0;

    return linearity;
}

float Features::compute_circularity()            //6
{
    CvMat * A = cvCreateMat(this->num_points_, 3, CV_64FC1);
    CvMat * B = cvCreateMat(this->num_points_, 1, CV_64FC1);

    int j = 0;
    for(SampleSet::iterator iter = this->sample_set_->begin(); \
        iter != this->sample_set_->end(); iter ++)
    {
        float x = (*iter)->get_x();
        float y = (*iter)->get_y();

        cvmSet(A, j, 0, -2.0 * x);
        cvmSet(A, j, 1, -2.0 * y);
        cvmSet(A, j, 2, 1);

        cvmSet(B, j, 0, -pow(x, 2) - pow(y, 2));
        j++;
    }

    CvMat * sol = cvCreateMat(3, 1, CV_64FC1);

    cvSolve(A, B, sol, CV_SVD);

    float xc = cvmGet(sol, 0, 0);
    float yc = cvmGet(sol, 1, 0);
    float rc = sqrt(pow(xc, 2) + pow(yc, 2) - cvmGet(sol, 2, 0));

    cvReleaseMat(&A);
    A = 0;
    cvReleaseMat(&B);
    B = 0;
    cvReleaseMat(&sol);
    sol = 0;

    float circularity = 0.0;
    for(SampleSet::iterator iter = this->sample_set_->begin(); \
        iter != this->sample_set_->end(); iter++)
    {
        circularity += pow(rc - sqrt(pow(xc - (*iter)->get_x(), 2) + \
                                     pow(yc - (*iter)->get_y(), 2)), 2);
    }

    return circularity;
}

float Features::compute_radius()                 //7
{
    CvMat * A = cvCreateMat(this->num_points_, 3, CV_64FC1);
    CvMat * B = cvCreateMat(this->num_points_, 1, CV_64FC1);

    int j = 0;
    for(SampleSet::iterator iter = this->sample_set_->begin(); \
        iter != this->sample_set_->end(); iter ++)
    {
        float x = (*iter)->get_x();
        float y = (*iter)->get_y();

        cvmSet(A, j, 0, -2.0 * x);
        cvmSet(A, j, 1, -2.0 * y);
        cvmSet(A, j, 2, 1);

        cvmSet(B, j, 0, -pow(x, 2) - pow(y, 2));
        j++;
    }

    CvMat * sol = cvCreateMat(3, 1, CV_64FC1);

    cvSolve(A, B, sol, CV_SVD);

    float xc = cvmGet(sol, 0, 0);
    float yc = cvmGet(sol, 1, 0);
    float rc = sqrt(pow(xc, 2) + pow(yc, 2) - cvmGet(sol, 2, 0));

    return rc;
}

float Features::compute_boundary_length()        //8
{
    float boundary_length = 0.0;
    float last_boundary_seg = 0.0;

    SampleSet::iterator left = this->sample_set_->begin();
    left++;
    left++;
    SampleSet::iterator mid = this->sample_set_->begin();
    mid++;
    SampleSet::iterator right = this->sample_set_->begin();

    while (left != this->sample_set_->end())
    {
      float mlx = (*left)->get_x() - (*mid)->get_x();
      float mly = (*left)->get_y() - (*mid)->get_y();
      float L_ml = sqrt(mlx * mlx + mly * mly);

      float mrx = (*right)->get_x() - (*mid)->get_x();
      float mry = (*right)->get_y() - (*mid)->get_y();
      float L_mr = sqrt(mrx * mrx + mry * mry);

      boundary_length += L_mr;
      last_boundary_seg = L_ml;

      left++;
      mid++;
      right++;
    }

    boundary_length += last_boundary_seg;
    return boundary_length;
}

float Features::compute_angle_diff()             //9
{
    float ang_diff = 0.0;

    SampleSet::iterator left = this->sample_set_->begin();
    left++;
    left++;
    SampleSet::iterator mid = this->sample_set_->begin();
    mid++;
    SampleSet::iterator right = this->sample_set_->begin();

    while (left != this->sample_set_->end())
    {
      float mlx = (*left)->get_x() - (*mid)->get_x();
      float mly = (*left)->get_y() - (*mid)->get_y();

      float mrx = (*right)->get_x() - (*mid)->get_x();
      float mry = (*right)->get_y() - (*mid)->get_y();
      float L_mr = sqrt(mrx * mrx + mry * mry);

      float A = (mlx * mrx + mly * mry) / pow(L_mr, 2);
      float B = (mlx * mry - mly * mrx) / pow(L_mr, 2);

      float th = atan2(B, A);

      if (th < 0)
        th += 2 * M_PI;

      ang_diff += th / this->num_points_;

      left++;
      mid++;
      right++;
    }

    return ang_diff;
}

float Features::compute_mean_curvature()         //10
{
    float mean_curvature = 0.0;

    SampleSet::iterator left = this->sample_set_->begin();
    left++;
    left++;
    SampleSet::iterator mid = this->sample_set_->begin();
    mid++;
    SampleSet::iterator right = this->sample_set_->begin();

    while (left != this->sample_set_->end())
    {
      float mlx = (*left)->get_x() - (*mid)->get_x();
      float mly = (*left)->get_y() - (*mid)->get_y();
      float L_ml = sqrt(mlx * mlx + mly * mly);

      float mrx = (*right)->get_x() - (*mid)->get_x();
      float mry = (*right)->get_y() - (*mid)->get_y();
      float L_mr = sqrt(mrx * mrx + mry * mry);

      float lrx = (*left)->get_x() - (*right)->get_x();
      float lry = (*left)->get_y() - (*right)->get_y();
      float L_lr = sqrt(lrx * lrx + lry * lry);

      float A = (mlx * mrx + mly * mry) / pow(L_mr, 2);
      float B = (mlx * mry - mly * mrx) / pow(L_mr, 2);

      float th = atan2(B, A);

      if (th < 0)
        th += 2 * M_PI;

      float s = 0.5 * (L_ml + L_mr + L_lr);
      float area = sqrt(s * (s - L_ml) * (s - L_mr) * (s - L_lr));

      if (th > 0)
        mean_curvature += 4 * (area) / (L_ml * L_mr * L_lr * this->num_points_);
      else
        mean_curvature -= 4 * (area) / (L_ml * L_mr * L_lr * this->num_points_);

      left++;
      mid++;
      right++;
    }

    return mean_curvature;
}

float Features::compute_boundary_regularity()    //11
{
    float boundary_regularity = 0.0;
    float boundary_length = 0.0;
    float last_boundary_seg = 0.0;
    double sum_boundary_reg_sq = 0.0;

    SampleSet::iterator left = this->sample_set_->begin();
    left++;
    left++;
    SampleSet::iterator mid = this->sample_set_->begin();
    mid++;
    SampleSet::iterator right = this->sample_set_->begin();

    while (left != this->sample_set_->end())
    {
      float mlx = (*left)->get_x() - (*mid)->get_x();
      float mly = (*left)->get_y() - (*mid)->get_y();
      float L_ml = sqrt(mlx * mlx + mly * mly);

      float mrx = (*right)->get_x() - (*mid)->get_x();
      float mry = (*right)->get_y() - (*mid)->get_y();
      float L_mr = sqrt(mrx * mrx + mry * mry);

      boundary_length += L_mr;
      sum_boundary_reg_sq += L_mr * L_mr;
      last_boundary_seg = L_ml;

      left++;
      mid++;
      right++;
    }

    boundary_length += last_boundary_seg;
    sum_boundary_reg_sq += last_boundary_seg * last_boundary_seg;
    boundary_regularity = sqrt((sum_boundary_reg_sq - pow(boundary_length, 2) / this->num_points_) / (this->num_points_ - 1));
    return boundary_regularity;
}

float Features::compute_iav()                    //12
{
    SampleSet::iterator first = this->sample_set_->begin();
    SampleSet::iterator mid = this->sample_set_->begin();
    mid++;
    SampleSet::iterator last = this->sample_set_->end();
    last--;

    double sum_iav = 0.0;

    while (mid != last)
    {
      float mlx = (*first)->get_x() - (*mid)->get_x();
      float mly = (*first)->get_y() - (*mid)->get_y();

      float mrx = (*last)->get_x() - (*mid)->get_x();
      float mry = (*last)->get_y() - (*mid)->get_y();
      float L_mr = sqrt(mrx * mrx + mry * mry);

      float A = (mlx * mrx + mly * mry) / pow(L_mr, 2);
      float B = (mlx * mry - mly * mrx) / pow(L_mr, 2);

      float th = atan2(B, A);

      if (th < 0)
        th += 2 * M_PI;

      sum_iav += th;

      mid++;
    }

    float iav = sum_iav / this->num_points_;
    return iav;
}

float Features::compute_standard_deviation_iav() //13
{
    SampleSet::iterator first = this->sample_set_->begin();
    SampleSet::iterator mid = this->sample_set_->begin();
    mid++;
    SampleSet::iterator last = this->sample_set_->end();
    last--;

    double sum_iav = 0.0;
    double sum_iav_sq  = 0.0;

    while (mid != last)
    {
      float mlx = (*first)->get_x() - (*mid)->get_x();
      float mly = (*first)->get_y() - (*mid)->get_y();

      float mrx = (*last)->get_x() - (*mid)->get_x();
      float mry = (*last)->get_y() - (*mid)->get_y();
      float L_mr = sqrt(mrx * mrx + mry * mry);

      float A = (mlx * mrx + mly * mry) / pow(L_mr, 2);
      float B = (mlx * mry - mly * mrx) / pow(L_mr, 2);

      float th = atan2(B, A);

      if (th < 0)
        th += 2 * M_PI;

      sum_iav += th;
      sum_iav_sq += th * th;

      mid++;
    }

    float std_iav = sqrt((sum_iav_sq - pow(sum_iav, 2) /  \
                          this->num_points_) / (this->num_points_ - 1));
    return std_iav;
}

bool Features::load_features()
{
    this->features_.push_back(this->compute_standard_deviation());
    this->features_.push_back(this->compute_avg_median_deviation());
    this->features_.push_back(this->compute_prev_jump());
    this->features_.push_back(this->compute_next_jump());
    this->features_.push_back(this->compute_width());
    this->features_.push_back(this->compute_linearity());
    this->features_.push_back(this->compute_circularity());
    this->features_.push_back(this->compute_radius());
    this->features_.push_back(this->compute_boundary_length());
    this->features_.push_back(this->compute_angle_diff());
    this->features_.push_back(this->compute_mean_curvature());
    this->features_.push_back(this->compute_boundary_regularity());
    this->features_.push_back(this->compute_iav());
    this->features_.push_back(this->compute_standard_deviation_iav());

    return (this->features_.size() == 14);
}

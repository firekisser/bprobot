#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP
#include <iostream>

class Interpolation
{
public:
    explicit Interpolation();
    explicit Interpolation(int argc, char ** argv);
    ~Interpolation();

private:
    int argc_;
    char ** argv_;
};

#endif //INTERPOLATION_HPP

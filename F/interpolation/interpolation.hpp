#include <iostream>

class Interpolation
{
public:
    explicit Interpolation();
    explicit Interpolation(int argc, char ** argv);
    ~Interpolation();

private:
    int argc;
    char ** argv;
};

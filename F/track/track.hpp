#include <iostream>

class Track
{
public:
    explicit Track();
    explicit Track(int argc, char ** argv);
    ~Track();
private:
    int argc;
    char ** argv;
};

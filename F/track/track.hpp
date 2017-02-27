#ifndef TRACK_HPP
#define TRACK_HPP

#include <iostream>

class Track
{
public:
    explicit Track();
    explicit Track(int argc, char ** argv);
    ~Track();
private:
    int argc_;
    char ** argv_;
};

#endif //TRACK_HPP

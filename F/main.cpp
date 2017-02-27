#include "controller.hpp"

#include "livingstatus/pyroelectricity/pyroelectricity.hpp"
#include "ros/ros.h"

int main(int argc, char ** argv)
{
    Controller * c = new Controller(argc, argv);
    c->handle();
    return 0;
}

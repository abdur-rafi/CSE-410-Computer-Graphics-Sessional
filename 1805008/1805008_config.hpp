#ifndef H_CONFIG
#define H_CONFIG

#include <fstream>
#include <iostream>
#include "1805008_utility.hpp"

struct Config{
    double near, far;
    double fovY, ratio;
    int levelOfRecursion;
    int w;

    void parseConfig(std::ifstream& );
    
};


struct CameraConfig{
    point l, r, u;
    point eyePos;

};

#endif 
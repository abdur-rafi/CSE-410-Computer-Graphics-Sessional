#ifndef H_CONFIG
#define H_CONFIG

#include <fstream>
#include <iostream>

struct Config{
    double near, far;
    double fovY, ratio;
    int levelOfRecursion;
    int w;

    void parseConfig(std::ifstream& );
    
};


#endif 
#ifndef H_RAYTRACER
#define H_RAYTRACER

#include <fstream>
#include "config.hpp"
#include "utility.hpp"
#include "objects.hpp"
#include <vector>

class RayTracer{
    
    Config config;
    std::vector<Object*> objects;

public:

    RayTracer(std::ifstream &f);

    ~RayTracer();
};

#endif
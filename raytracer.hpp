#ifndef H_RAYTRACER
#define H_RAYTRACER

#include <fstream>
#include "config.hpp"
#include "utility.hpp"
#include "objects.hpp"
#include "lightSource.hpp"
#include <vector>

class RayTracer{
    
    Config config;
    std::vector<Object*> objects;
    point* eyePos;
    point ***pointBuffer;
    std::vector<LightSource*> lights;

public:

    Config getConfig();

    RayTracer(std::ifstream &f);

    void draw(point eyePos);

    ~RayTracer();

    void generatePointBuffer(const CameraConfig &cConfig);

    void generateImage(const CameraConfig &cConfig);
};

#endif
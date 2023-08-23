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
    point* eyePos;
    point ***pointBuffer;

public:
    std::vector<Object*> objects;
    std::vector<LightSource*> lights;

    Config getConfig();

    RayTracer(std::ifstream &f);

    void draw(point eyePos);

    ~RayTracer();

    void generatePointBuffer(const CameraConfig &cConfig);

    IntersectionReturnVal intersection(const Line& line);


    void generateImage(const CameraConfig &cConfig);

    point calcColor(const IntersectionReturnVal& val, const Line&, const point&);

    point colorRecursive(const Line& line, int level);
};

#endif
#ifndef H_LIGHT_SOURCE
#define H_LIGHT_SOURCE

// #include "raytracer.hpp"
#include "utility.hpp"
#include "objects.hpp"

class RayTracer;

class LightSource{



public:

    point position;
    double fallOfParam;

    LightSource(point p, double f);
    virtual bool inShadow(const point& pt, RayTracer* rt) = 0;

    virtual void draw() = 0;

    // void draw() = 0;
};


class NormalLight : public LightSource{
    Sphere* sphere;
public:

    NormalLight(point pos, double fparam);

    static NormalLight* parse(std::ifstream &f);
    bool inShadow(const point& pt, RayTracer* rt);
    void draw();
};


class SpotLight : public LightSource{

    point lookingTo;
    point lookingDir;
    double cutOffAngle;

public:

    SpotLight(point pos, double fparam, point looking, double cutOffAngle);

    static SpotLight* parse(std::ifstream &f);
    bool inShadow(const point& pt, RayTracer* rt);
    void draw();
};

#endif
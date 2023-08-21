#ifndef H_LIGHT_SOURCE
#define H_LIGHT_SOURCE

#include "utility.hpp"
#include "objects.hpp"

class LightSource{

    point position;
    double fallOfParam;


public:

    LightSource(point p, double f);

    // void draw() = 0;
};


class NormalLight : public LightSource{

public:

    NormalLight(point pos, double fparam);

    static NormalLight* parse(std::ifstream &f);

};


class SpotLight : public LightSource{

    point looking;
    double cutOffAngle;

public:

    SpotLight(point pos, double fparam, point looking, double cutOffAngle);

    static SpotLight* parse(std::ifstream &f);
};

#endif
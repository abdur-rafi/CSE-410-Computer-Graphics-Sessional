#ifndef H_OBJECTS
#define H_OBJECTS

#include "utility.hpp"
#include <fstream>

class Object{

public:

    virtual void draw(point eyePos) = 0;


};

class Sphere : public Object{
    point center, color;
    double radius;
    quartet coeffs;
    double shininess;
public : 

    Sphere(point cent, double r, point col, quartet cfs,double shininess);

    void draw(point eyePos);

    static Sphere* parseSphere(std::ifstream &f);
};

class Pyramid : public Object{
    point lowestPoint;
    double w, h;
    point color;
    quartet coeffs;
    double shininess;

public:
    Pyramid(point lp, double w_, double h_, point col, quartet cf, double sh);

    static Pyramid* parsePyramid(std::ifstream &f);

    void draw(point eyePos);

};


class Cube : public Object{
    point bottomLeft;
    double side;
    point color;
    quartet coeffs;
    double shininess;

public:
    Cube(point lp, double side, point col, quartet cf, double sh);

    static Cube* parseCube(std::ifstream &f);

    void draw(point eyePos);

};

class CheckerBoard : public Object{
    double w;
    point coeff;

public:
    CheckerBoard(double w_, point cf);

    void draw(point eyePos);
    
    static CheckerBoard* parseCheckerBoard(std::ifstream &f);
};


#endif 
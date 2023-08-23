#ifndef H_OBJECTS
#define H_OBJECTS

#include "utility.hpp"
#include <fstream>
#include <vector>
class RayTracer;
class Object{
public:
    quartet coeffs;
    double shininess;

    virtual void draw(point eyePos) = 0;
    
    virtual IntersectionReturnVal intersection(const Line &line) = 0;
    virtual point getColor(const point& ) = 0;

};

class Sphere : public Object{
    point center, color;
    double radius;
public : 

    Sphere(point cent, double r, point col, quartet cfs,double shininess);

    void draw(point eyePos);

    static Sphere* parseSphere(std::ifstream &f);
    IntersectionReturnVal intersection(const Line &line);
    point getColor(const point& );

};

class Pyramid : public Object{
    point lowestPoint;
    double w, h;
    point color;
    std::vector<Surface*> surfaces;

public:
    Pyramid(point lp, double w_, double h_, point col, quartet cf, double sh);

    static Pyramid* parsePyramid(std::ifstream &f);

    void draw(point eyePos);
    IntersectionReturnVal intersection(const Line &line);
    point getColor(const point& );
    
    
};


class Cube : public Object{
    point bottomLeft;
    double side;
    point color;
    std::vector<Surface*> surfaces;

public:
    Cube(point lp, double side, point col, quartet cf, double sh);

    static Cube* parseCube(std::ifstream &f);

    void draw(point eyePos);
    IntersectionReturnVal intersection(const Line &line);
    
    point getColor(const point& );

};

class CheckerBoard : public Object{
    double w;

public:
    CheckerBoard(double w_, quartet cf);

    void draw(point eyePos);
    
    static CheckerBoard* parseCheckerBoard(std::ifstream &f);
    IntersectionReturnVal intersection(const Line &line);

    point getColor(const point& );

};


#endif 
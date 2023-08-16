#ifndef H_UTILITY
#define H_UTILITY

#include <GL/glut.h> 
#include <fstream>

struct point {
    GLfloat x, y, z;

    point operator*(float s) const;
    point operator+(point p) const;

    point operator-(point p) const;

    static point parsePoint(std::ifstream&);
};

struct quartet{
    GLfloat x,y,z,w;

    static quartet parseQuartet(std::ifstream &);
};

#endif
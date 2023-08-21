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



point crossProduct(point& p1, point& p2);

void rotateVector(point& axis, point& vector, float angle);

void normalize(point& p);

float magnitude(const point &p);

#endif
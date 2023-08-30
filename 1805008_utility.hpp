#ifndef H_UTILITY
#define H_UTILITY

#include <GL/glut.h> 
#include <fstream>

#define D2R (M_PI / 180.0)
#define EPS 1e-3;

class Object;

struct point {
    GLfloat x, y, z;
    
    point();
    point(double a, double b, double c);

    point operator*(float s) const;
    point operator+(point p) const;

    point operator-(point p) const;
    point operator* (const point& p) const;

    static point parsePoint(std::ifstream&);

    void normalize();

    double dotProduct(const point& p) const;
    // double dotProduct(const point& p1, const point& p2);

};

struct quartet{
    GLfloat x,y,z,w;

    static quartet parseQuartet(std::ifstream &);
};



point crossProduct(const point& p1, const point& p2);

void rotateVector(point& axis, point& vector, float angle);


float magnitude(const point &p);


class Line{
public:
    point src;
    point dir;

    Line(const point& from ,const point& toOrDir, bool isDir);
};

struct IntersectionReturnVal{
    double t;
    point normal;
    Object* obj;
};

class Surface{
public:
    point* normal;
    virtual double intersection(const Line &line) = 0;
    virtual void draw() = 0;
};

class Triangle : public Surface{
    point a, b, c;
public:
    Triangle(point p1, point p2, point p3);

    double intersection(const Line &line);
    void draw();

};

class Quadrilateral : public Surface{
    point a, b, c, d;
    Triangle *tr1, *tr2;
public:
    Quadrilateral(point p1, point p2, point p3, point p4);

    double intersection(const Line &line);
    void draw();

};

class Determinant3By3{

    double a[3][3];

public:
    Determinant3By3(const point& p1, const point& p2, const point& p3);

    void assignCol(const point& p, int colNo);

    double getVal();
};

#endif
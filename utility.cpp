#include "utility.hpp"
#include <cmath>

point point::operator*(float s) const{
    point t = {x * s, y * s, z * s};
    return t;
}
point point::operator+(point p) const{
    point t = {x + p.x, y + p.y, z + p.z};
    return t;
}

point point::operator-(point p) const{
    point t = {x - p.x, y - p.y, z - p.z};
    return t;
}

point point::parsePoint(std::ifstream &f){
    point t;
    f >> t.x >> t.y >> t.z;
    return t;
}

point crossProduct(point& p1, point& p2){
    return {p1.y * p2.z - p2.y * p1.z, -p1.x * p2.z + p2.x * p1.z, p1.x * p2.y - p2.x * p1.y};
}

void rotateVector(point& axis, point& vector, float angle){
    point u = crossProduct(axis, vector);
    vector = u * sin(angle) + vector * cos(angle);
}

void normalize(point& p){
    double mag = sqrt(p.x  * p.x + p.y * p.y + p.z * p.z);
    if(mag > 1e-7){
        p = p * (1. / mag);
    }
}

float magnitude(const point &p){
    return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

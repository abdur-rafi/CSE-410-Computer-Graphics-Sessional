#include "utility.hpp"


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
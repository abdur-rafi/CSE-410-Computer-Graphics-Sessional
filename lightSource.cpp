#include "lightSource.hpp"
#include "raytracer.hpp"
#include <cmath>

LightSource::LightSource(point p, double f){
    this->position = p;
    this->fallOfParam = f;
}

NormalLight::NormalLight(point p, double f) : LightSource(p, f){

}

NormalLight* NormalLight::parse(std::ifstream &f){
    point p = point::parsePoint(f);
    double fop;
    f >> fop;
    return new NormalLight(p, fop);
}

SpotLight :: SpotLight(point pos, double fparam, point looking, double cutOffAngle) : LightSource(pos, fparam){
    this->lookingTo = looking;
    this->cutOffAngle = cutOffAngle;
    this->lookingDir = lookingTo - position;
    this->lookingDir.normalize();

}

SpotLight* SpotLight::parse(std::ifstream &f){
    point p = point::parsePoint(f);
    double fop;
    f >> fop;
    point l = point::parsePoint(f);
    double angle;
    f >> angle;

    return new SpotLight(p, fop, l, angle);
}

bool NormalLight::inShadow(const point& pt, RayTracer* rt){
    point dir = this->position - pt;
    dir.normalize();
    point from = pt + dir * EPS;
    Line line(from, dir, true);
    
    return rt->intersection(line).t >= 0; 
}   


bool SpotLight::inShadow(const point& pt,RayTracer* rt){

    point dir = this->position - pt;
    dir.normalize();
    point from = pt + dir * EPS;
    Line line(from, dir, true);
    
    if(rt->intersection(line).t >= 0){
        return true;
    }

    point srcToPointDir = dir * -1;
    double prod = srcToPointDir.dotProduct(this->lookingDir);

    if(std::acos(prod) > this->cutOffAngle * D2R){
        return true;
    }
    return false;

}
#include "lightSource.hpp"

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
    this->looking = looking;
    this->cutOffAngle = cutOffAngle;
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
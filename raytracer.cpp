#include "raytracer.hpp"
#include <string>

RayTracer::RayTracer(std::ifstream &f){
    config.parseConfig(f);
    
    objects.push_back(CheckerBoard::parseCheckerBoard(f));

    int n;
    f >> n;
    for(int i = 0; i < n; ++i){
        std::string type;
        f >> type;
        if(type == "cube"){
            objects.push_back(Cube::parseCube(f));
        }
        else if(type == "sphere"){
            objects.push_back(Sphere::parseSphere(f));
        }
        else if (type == "pyramid"){
            objects.push_back(Pyramid::parsePyramid(f));
        }
    }

}

void RayTracer::draw(point eyePos){
    for(auto x : objects){
        x->draw(eyePos);
    }
}


Config RayTracer::getConfig(){
    return config;
}

RayTracer::~RayTracer(){
    for(auto x : objects){
        delete x;
    }
}
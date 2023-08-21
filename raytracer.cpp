#include "raytracer.hpp"
#include <string>
#include <cmath>


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
    f >> n;
    for(int i = 0; i < n; ++i){
        lights.push_back(NormalLight::parse(f));
    }
    f >> n;
    for(int i = 0; i < n; ++i){
        lights.push_back(SpotLight::parse(f));
    }
    pointBuffer = 0;

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
    for(auto x : lights){
        delete x;
    }
}

void RayTracer::generatePointBuffer(CameraConfig cConfig){
    if(pointBuffer){
        for(int i = 0; i < config.w; ++i){
            for(int j = 0; j < config.w; ++j){
                delete(pointBuffer[i][j]);
            }
            delete[] pointBuffer;
        }
        delete[] pointBuffer;
    }

    point midPoint = cConfig.eyePos + cConfig.l * config.near;
    double distanceX = 2 * config.near * tan(config.fovY / 2.);
    double distanceY = 2 * config.near * tan((config.fovY * config.ratio)/2.);

    pointBuffer = new point**[config.w];
    for(int i = 0; i < config.w; ++i)
        pointBuffer[i] = new point*[config.w];
    
    double dx = distanceX / config.w;
    double dy = distanceY / config.w;

    int midIndex = config.w / 2;
    
    point leftTopCorner = midPoint - cConfig.u * midIndex - cConfig.r * midIndex;
    for(int i = 0; i < config.w; ++i){
        for(int j = 0; j < config.w; ++j){
            point curr = leftTopCorner + cConfig. u * i + cConfig.r * j;
            pointBuffer[i][j] = new point(curr);
            // std::cout << pointBuffer[i][j]->x << " " << pointBuffer[i][j]->y << " " << pointBuffer[i][j]->z << "\n" ;
        }
    }



}
#include "raytracer.hpp"
#include <string>
#include <cmath>
#include "bitmap_image.hpp"

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
    if(pointBuffer){
        glColor3f(0, 0, 1);
        glBegin(GL_QUADS);
            glVertex3f(pointBuffer[0][0]->x, pointBuffer[0][0]->y, pointBuffer[0][0]->z);
            glVertex3f(pointBuffer[0][config.w-1]->x, pointBuffer[0][config.w-1]->y, pointBuffer[0][config.w-1]->z);
            glVertex3f(pointBuffer[config.w-1][config.w-1]->x, pointBuffer[config.w-1][config.w-1]->y, pointBuffer[config.w-1][config.w-1]->z);
            glVertex3f(pointBuffer[config.w-1][0]->x, pointBuffer[config.w-1][0]->y, pointBuffer[config.w-1][0]->z);

        glEnd();
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

void RayTracer::generatePointBuffer(const CameraConfig &cConfig){
    // if(pointBuffer){
    //     for(int i = 0; i < config.w; ++i){
    //         for(int j = 0; j < config.w; ++j){
    //             delete(pointBuffer[i][j]);
    //         }
    //         delete[] pointBuffer;
    //     }
    //     delete[] pointBuffer;
    // }

    point midPoint = cConfig.eyePos + cConfig.l * config.near;
    double distanceX = 2 * config.near * tan((config.fovY / 2.) * D2R);
    double distanceY = 2 * config.near * tan(((config.fovY * config.ratio)/2.) * D2R);

    pointBuffer = new point**[config.w];
    for(int i = 0; i < config.w; ++i)
        pointBuffer[i] = new point*[config.w];
    
    double dx = distanceX / config.w;
    double dy = distanceY / config.w;

    int midIndex = config.w / 2;
    
    point leftTopCorner = midPoint + cConfig.u * midIndex * dy - cConfig.r * midIndex * dx;
    for(int i = 0; i < config.w; ++i){
        for(int j = 0; j < config.w; ++j){
            point curr = leftTopCorner - cConfig. u * i * dy + cConfig.r * j * dx;
            pointBuffer[i][j] = new point(curr);
            // std::cout << pointBuffer[i][j]->x << " " << pointBuffer[i][j]->y << " " << pointBuffer[i][j]->z << "\n" ;
        }
    }



}

void RayTracer::generateImage(const CameraConfig &cConfig){
    bitmap_image image(config.w, config.w);
    image.set_all_channels(0);
    for(int i = 0; i < config.w; ++i){
        for(int j = 0; j < config.w; ++j){

            point curr = *pointBuffer[i][j];
            Line line(curr, curr - cConfig.eyePos, true);
            double tMinValid = -1;
            for(auto x : objects){
                double t = x->intersection(line);
                if(t >= 0){
                    if(tMinValid < 0 || t < tMinValid){
                        tMinValid = t;
                    }
                }
            }
            if(tMinValid >= 0){
                image.set_pixel(j, i, 255, 255, 255);
            }
        }
    }
    image.save_image("test.bmp");

}
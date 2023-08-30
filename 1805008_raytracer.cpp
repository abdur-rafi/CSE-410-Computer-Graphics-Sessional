#include <string>
#include <cmath>
#include "1805008_raytracer.hpp"
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
        LightSource* lt = NormalLight::parse(f);
        lights.push_back(lt);
    }
    f >> n;
    for(int i = 0; i < n; ++i){
        LightSource* lt = SpotLight::parse(f);
        lights.push_back(lt);
    }
    pointBuffer = 0;

}

void RayTracer::draw(point eyePos){
    for(auto x : objects){
        x->draw(eyePos);
    }
    for(auto x : lights){
        x->draw();
    }
    // if(pointBuffer){
    //     glColor3f(0, 0, 1);
    //     glBegin(GL_QUADS);
    //         glVertex3f(pointBuffer[0][0]->x, pointBuffer[0][0]->y, pointBuffer[0][0]->z);
    //         glVertex3f(pointBuffer[0][config.w-1]->x, pointBuffer[0][config.w-1]->y, pointBuffer[0][config.w-1]->z);
    //         glVertex3f(pointBuffer[config.w-1][config.w-1]->x, pointBuffer[config.w-1][config.w-1]->y, pointBuffer[config.w-1][config.w-1]->z);
    //         glVertex3f(pointBuffer[config.w-1][0]->x, pointBuffer[config.w-1][0]->y, pointBuffer[config.w-1][0]->z);

    //     glEnd();
    // }
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
        }
    }



}



IntersectionReturnVal RayTracer::intersection(const Line& line){
    IntersectionReturnVal minT{-1};
    point color;
    for(auto x : objects){
        auto t = x->intersection(line);
        if(t.t >= 0){
            if(minT.t < 0 || t.t < minT.t){
                minT = t;
            }
        }
    }

    return minT;
}

void RayTracer::generateImage(const CameraConfig &cConfig){
    bitmap_image image(config.w, config.w);
    image.set_all_channels(0);
    for(int i = 0; i < config.w; ++i){
        for(int j = 0; j < config.w; ++j){

            point curr = *pointBuffer[i][j];
            Line line(curr, curr - cConfig.eyePos, true);
            point color = this->colorRecursive(line, this->getConfig().levelOfRecursion) * 255;
            image.set_pixel(j, i, color.x, color.y, color.z);
        }
    }
    image.save_image("test.bmp");
    
    for(int i = 0; i < config.w; ++i){
        for(int j = 0; j < config.w; ++j){
            delete pointBuffer[i][j];
        }
    }
    for(int i = 0; i < config.w; ++i)
        delete[] pointBuffer[i];
    
    delete[] pointBuffer;
}

point RayTracer::calcColor(const IntersectionReturnVal& val, const Line& line, const point& reflected){

    point intPoint = line.src + line.dir * val.t;
    point toCamera = line.dir * (-1);
    toCamera.normalize();
    double lambert = 0;
    double phong = 0;
    if(val.normal.dotProduct(line.dir) <= 0){
        for(auto x : this->lights){
            if(!x->inShadow(intPoint, this)){
                point toSrc = x->position - intPoint;
                double distance = std::sqrt(toSrc.dotProduct(toSrc));
                toSrc.normalize();
                double scalingFactor = std::exp( -distance * distance * x->fallOfParam );
                lambert += std::max(0.,val.normal.dotProduct(toSrc)) * scalingFactor;
                if(val.obj->coeffs.w > 0){
                    double prod = reflected.dotProduct(toSrc);
                    if(prod > 0){
                        phong += pow( prod, val.obj->shininess) * scalingFactor;
                    }
                }
            }
        }
    }
    point objColor = val.obj->getColor(intPoint);
    quartet coeffs = val.obj->coeffs;
    point color = objColor * (coeffs.x + coeffs.y * lambert + coeffs.z * phong);

    return color;   
}


point RayTracer::colorRecursive(const Line& line, int level){
    point color;
    IntersectionReturnVal val = this->intersection(line);
    if(val.t < 0 || val.t > config.far){
        return color;
    }
    point reflected = line.dir - val.normal * ( 2 * line.dir.dotProduct(val.normal));
    reflected.normalize();
    color = calcColor(val, line, reflected);
    if(level > 0){
        point from = line.src + line.dir * val.t + reflected * EPS;
        Line rLine(from , reflected, true);
        color = color + colorRecursive(rLine, level - 1) * val.obj->coeffs.w;
    }

    return color;

}
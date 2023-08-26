#include "lightSource.hpp"
#include "raytracer.hpp"
#include <cmath>
#include "objects.hpp"
#include "utility.hpp"

LightSource::LightSource(point p, double f){
    this->position = p;
    this->fallOfParam = f;
}

NormalLight::NormalLight(point p, double f) : LightSource(p, f){
    this->sphere = new Sphere(p, 5,{1,1,1},{0,0,0, 0},0);
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

void NormalLight::draw(){
    sphere->draw({});
}

void SpotLight::draw(){
    double delta = 1;
    double height = 15;
    point radiusVec = crossProduct(this->lookingDir, point(0, 1, 0));
    radiusVec.normalize();
    point center = this->position + this->lookingDir * height;
    double circleRadius = height * std::tan(this->cutOffAngle * D2R);
    radiusVec = radiusVec * circleRadius;
    point prev = center + radiusVec;

    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
        for(int i = delta; i < 360; i += delta){
            rotateVector(this->lookingDir, radiusVec, delta);
            point newPoint = center + radiusVec;
            glVertex3f(this->position.x, this->position.y, this->position.z);
            glVertex3f(prev.x, prev.y, prev.z);
            glVertex3f(newPoint.x, newPoint.y, newPoint.z);
            prev = newPoint;
        }

    glEnd();    
    // glPushMatrix();
    // glTranslatef(this->position.x, this->position.y, this->position.z);
    // glutSolidSphere(10, 20, 20);
    // glPopMatrix();
}
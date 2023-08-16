#include "objects.hpp"

Sphere::Sphere(point cent, double r, point col, quartet cfs, double sh){

    center = cent;
    radius = r;
    color = col;
    coeffs = cfs;
    shininess = sh;
}

void Sphere::draw(){

}

Sphere* Sphere::parseSphere(std::ifstream &f){
    point cen = point::parsePoint(f);
    double r;
    f >> r;
    point col = point::parsePoint(f);
    quartet cfs = quartet::parseQuartet(f);
    double sh;
    f >> sh;

    return new Sphere(cen, r, col, cfs, sh);


}

quartet quartet::parseQuartet(std::ifstream &f){
    quartet x;
    f >> x.x >> x.y >> x.z >> x.w;
    return x;
}

Pyramid* Pyramid::parsePyramid(std::ifstream &f){
    point lp = point::parsePoint(f);
    double w,h;
    f >> w >> h;
    point col = point::parsePoint(f);
    quartet q = quartet::parseQuartet(f);
    double sh;
    f >> sh;
    return new Pyramid(lp, w, h, col, q, sh);
}

Pyramid::Pyramid(point lp, double w_, double h_, point col, quartet cf, double sh){
    lowestPoint = lp;
    w = w_;
    h = h_;
    color = col;
    coeffs = cf;
    shininess = sh;
}

void Pyramid::draw(){

}

Cube* Cube::parseCube(std::ifstream &f){
    point bc = point::parsePoint(f);
    double side;
    f >> side;
    point col = point::parsePoint(f);
    quartet coeff = quartet::parseQuartet(f);
    double sh;
    f >> sh;
    return new Cube(bc, side, col, coeff, sh);
}

Cube::Cube(point lp, double side, point col, quartet cf, double sh){
    bottomLeft = lp;
    this->side = side;
    color = col;
    coeffs = cf;
    shininess = sh;
}

void Cube::draw(){

}

CheckerBoard::CheckerBoard(double a, point cf){
    w = a;
    coeff = cf;
}

CheckerBoard* CheckerBoard::parseCheckerBoard(std::ifstream &f){
    double a;
    f >> a;
    point coeff = point::parsePoint(f);
    return new CheckerBoard(a, coeff);
}

void CheckerBoard::draw(){

}
#include "utility.hpp"
#include <cmath>
#include <iostream>

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

void point:: normalize(){
    double mag = sqrt(x  * x + y * y + z * z);
    if(mag > 1e-7){
        x /= mag;
        y /= mag;
        z /= mag;
    }
}

float magnitude(const point &p){
    return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}


Line::Line(const point& from , const point& toOrDir, bool isDir){
    src = from;
    dir = toOrDir;
    if(!isDir){
        dir = dir - from;
    }
    dir.normalize();
}

double point::dotProduct(const point& p){
    return x * p.x + y * p.y + z * p.z;
}

double point::dotProduct(const point& p1, const point& p2){
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}


Triangle::Triangle(point p1, point p2, point p3){
    this->a = p1;
    this->b = p2;
    this->c = p3;
}
double Triangle::intersection(const Line &line){
    Determinant3By3 betaD(a - line.src, a - c, line.dir);
    Determinant3By3 gammaD(a - b, a - line.src, line.dir);
    Determinant3By3 tD(a - b, a - c, a - line.src);
    Determinant3By3 A(a - b, a - c, line.dir);
    double aVal = A.getVal();
    // std::cout << "aval " << aVal << "\n";
    double beta = betaD.getVal() / A.getVal();
    double gamma = gammaD.getVal() / A.getVal();
    // std::cout << beta << " " << gamma << "\n";
    double t = tD.getVal() / A.getVal();
    if(beta >= 0 && gamma >= 0 && beta + gamma <= 1 && t > 0){
        return t;
    }
    return -1;

}

Quadrilateral::Quadrilateral(point p1, point p2, point p3, point p4){
    this->a = p1;
    this->b = p2;
    this->c = p3;
    this->d = p4;
    tr1 = new Triangle(a, b, c);
    tr2 = new Triangle(c, d, a);
}

double Quadrilateral::intersection(const Line &line){
    double t1 = tr1->intersection(line);
    if(t1 >= 0){
        return t1;
    }
    double t2 = tr2->intersection(line);
    if(t2 >= 0){
        return t2;
    }
    return -1;

}

void Determinant3By3::assignCol(const point& p, int col){
    a[0][col] = p.x;
    a[1][col] = p.y;
    a[2][col] = p.z;
}

Determinant3By3::Determinant3By3(const point& p1, const point& p2, const point& p3){
    assignCol(p1, 0);
    assignCol(p2, 1);    
    assignCol(p3, 2);    
}

double Determinant3By3::getVal(){
    double res = 0;
    res += a[0][0] * (a[1][1] * a[2][2] - a[1][2] * a[2][1]);
    res -= a[0][1] * (a[1][0] * a[2][2] - a[2][0] * a[1][2]);
    res += a[0][2] * (a[1][0] * a[2][1] - a[2][0] * a[1][1]);
    return res;
}

void Triangle::draw(){

    glBegin(GL_TRIANGLES);
        glVertex3d(a.x, a.y, a.z);
        glVertex3d(b.x, b.y, b.z);
        glVertex3d(c.x, c.y, c.z);
    glEnd();

}

void Quadrilateral::draw(){
    tr1->draw();
    tr2->draw();
}

point::point(){
    x = y = z = 0;
}
point::point(double a, double b, double c){
    x = a;
    y = b;
    z = c;
}
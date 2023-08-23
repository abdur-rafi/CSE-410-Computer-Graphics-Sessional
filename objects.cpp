#include "objects.hpp"
#include <cmath>
#include <iostream>
#include "lightSource.hpp"
#include "raytracer.hpp"

Sphere::Sphere(point cent, double r, point col, quartet cfs, double sh){

    center = cent;
    radius = r;
    color = col;
    coeffs = cfs;
    shininess = sh;
}

void Sphere::draw(point eyePos){
    int stacks = 100;
    int slices = 100;


    struct point points[stacks+1][slices+1];
    for (int j = 0; j <= stacks; j++) {
        double phi = -M_PI / 2.0 + j * M_PI / stacks;
        double r = radius * cos(phi);
        double h = radius * sin(phi);
        for (int i = 0; i < slices+1; i++) {
            double theta = i * 2.0 * M_PI / slices;
            points[j][i].x = r * cos(theta);
            points[j][i].y = r * sin(theta);
            points[j][i].z = h;
        }
    }
    glPushMatrix();
        glTranslatef(center.x, center.y, center.z);
        glBegin(GL_QUADS);
            for (int j = 0; j < stacks; j++) {
                for (int i = 0; i < slices; i++) {
                    // GLfloat c = (2+cos((i+j) * 2.0 * M_PI / slices)) / 3;
                    glColor3f(color.x, color.y, color.z);
                    glVertex3f(points[j][i].x, points[j][i].y, points[j][i].z);
                    glVertex3f(points[j][i+1].x, points[j][i+1].y, points[j][i+1].z);

                    glVertex3f(points[j+1][i+1].x, points[j+1][i+1].y, points[j+1][i+1].z);
                    glVertex3f(points[j+1][i].x, points[j+1][i].y, points[j+1][i].z);
                }
            }
        glEnd();
    glPopMatrix();

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

    double baseCenterToCorner = w / sqrt(2);
    point cornerPoints[4];
    for(int i = 0; i < 4; ++i){
        cornerPoints[i] = lowestPoint;
        cornerPoints[i].x += baseCenterToCorner * cos((135 - i * 90) * D2R);
        cornerPoints[i].y -= baseCenterToCorner * sin((135 - i * 90) * D2R);
    }
    point topPoint = lowestPoint;
    topPoint.z += h;
    for(int i = 0; i < 4; ++i){
        surfaces.push_back(new Triangle(cornerPoints[(i+1)%4],cornerPoints[i],  topPoint));
    }
    surfaces.push_back(new Quadrilateral(cornerPoints[0], cornerPoints[1], cornerPoints[2], cornerPoints[3]));
}

void Pyramid::draw(point eyePos){
    glColor3f(color.x, color.y, color.z);
    for(auto surface : surfaces){
        surface->draw();
    }
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
    point cornerPoints[8];
    point t = bottomLeft;
    for(int i = 0; i < 2; ++i){

        cornerPoints[0 + i * 4] = t;
        cornerPoints[1 + i * 4] = t + point(side, 0, 0);
        cornerPoints[2 + i * 4] = t + point(side,side,0);
        cornerPoints[3 + i * 4] = t + point(0,side,0);
        t = t + point(0, 0, side);
    }
    surfaces.push_back(
        new Quadrilateral(
            cornerPoints[0], cornerPoints[3 ],
            cornerPoints[2], cornerPoints[1 ]
        )
    );

    surfaces.push_back(
        new Quadrilateral(
            cornerPoints[4], cornerPoints[5],
            cornerPoints[6], cornerPoints[7]
        )
    );
    


    surfaces.push_back(
        new Quadrilateral(
            cornerPoints[0], cornerPoints[1],
            cornerPoints[5], cornerPoints[4]
        )
    );


    surfaces.push_back(
        new Quadrilateral(
            cornerPoints[3], cornerPoints[7],
            cornerPoints[6], cornerPoints[2]
        )
    );

    surfaces.push_back(
        new Quadrilateral(
            cornerPoints[0], cornerPoints[4],
            cornerPoints[7], cornerPoints[3]
        )
    );
    
    surfaces.push_back(
        new Quadrilateral(
            cornerPoints[1], cornerPoints[2],
            cornerPoints[6], cornerPoints[5]
        )
    );
    




}

void Cube::draw(point eyePos){
    glColor3f(color.x, color.y, color.z);
    for(auto x : surfaces){
        x->draw();
    }
}

CheckerBoard::CheckerBoard(double a, quartet cf){
    w = a;
    coeffs = cf;
}

CheckerBoard* CheckerBoard::parseCheckerBoard(std::ifstream &f){
    double a;
    f >> a;
    point coeff = point::parsePoint(f);
    return new CheckerBoard(a, {coeff.x, coeff.y, 0, coeff.z});
}

void CheckerBoard::draw(point eyePos){
    int c = 0;
    glPushMatrix();
        // glTranslatef(eyePos.x, 0, eyePos.z);
        glTranslatef(0, -500, 0);
        glBegin(GL_QUADS);
        for(int i = -10; i < 20; ++i){
            for(int j = 1; j < 50; ++j){
                if(!c){
                    glColor3f(0, 0, 0);
                }
                else{
                    glColor3f(1, 1, 1);
                }
                c = 1 - c;
                // glVertex3f((i-1) * w, 0 , (j-1) * w);
                // glVertex3f((i) * w, 0 , (j-1) * w);
                // glVertex3f((i) * w, 0 , (j) * w);
                // glVertex3f((i-1) * w, 0 , (j) * w);
                glVertex3f((i-1) * w, (j-1) * w, 0);
                glVertex3f((i) * w, (j-1) * w, 0);
                glVertex3f((i) * w, (j) * w, 0);
                glVertex3f((i-1) * w, (j) * w, 0);

            }
        }
        
        glEnd();
    glPopMatrix();
}

IntersectionReturnVal Sphere::intersection(const Line &line){
    point R0 = this->center - line.src;
    double td = R0.dotProduct(line.dir);
    double r2 = radius * radius;
    if(td < 0){
        return {-1};
    }
    double d2 = R0.dotProduct(R0) - td * td;
    if(d2 > r2){
        return {-1};
    }
    double t = sqrt(r2 - d2);
    if(R0.dotProduct(R0) < r2){
        t = td + t;
    }
    else{
        t = td - t;
    }
    point normal = line.src + line.dir * t - this->center ;
    normal.normalize();
    return {t, normal, this};
}

IntersectionReturnVal Pyramid::intersection(const Line &line){
    double tValidMin = -1;
    point normal;
    for(auto surface : surfaces){
        double tCurr = surface->intersection(line);
        if(tCurr >= 0){
            if(tValidMin < 0){
                tValidMin = tCurr;
                normal = *surface->normal;
            }
            else{
                if(tValidMin > tCurr){
                    tValidMin = tCurr;
                    normal = *surface->normal;
                }
            }
        }
    }
    return {tValidMin, normal, this};

}

IntersectionReturnVal Cube::intersection(const Line &line){
    double tValidMin = -1;
    point normal;
    for(auto surface : surfaces){
        double tCurr = surface->intersection(line);
        if(tCurr >= 0){
            if(tValidMin < 0){
                tValidMin = tCurr;
                normal = *surface->normal;
            }
            else{
                if(tValidMin > tCurr){
                    tValidMin = tCurr;
                    normal = *surface->normal;
                }
            }
        }
    }
    return {tValidMin, normal, this};
}

IntersectionReturnVal CheckerBoard::intersection(const Line &line){
    // return -1;
    if(line.dir.z == 0){
        
        return {-1};
    }
    double t = -line.src.z / line.dir.z;
    if(t < 0){
    return {-1};
    }
    point normal(0, 0, 1);
    if(line.src.z < 0){
        normal.z = -1;
    }
    return {t, normal, this};
}

point Sphere::getColor(const point &p){
    return this->color;
}


point Cube::getColor(const point &p){
    return this->color;
}


point Pyramid::getColor(const point &p){
    return this->color;
}


point CheckerBoard::getColor(const point &p){
    int i = std::abs(p.x) / this->w;
    int j = std::abs(p.y) / this->w;
    if(p.x > 0 && p.y > 0 || (p.x < 0 && p.y < 0)){
        if((i + j) % 2 == 0){
            return {1, 1, 1};
        }
        return {0, 0, 0};
    }
    else{
        if((i + j) % 2 == 0){
            return {0, 0, 0};
        }
        return {1, 1, 1};
    }
}

// point Sphere::getColor(const Line& line, RayTracer* rt){
//     double t = this->intersection(line);
//     if(t < 0){
//         std::cout << "Issue\n";
//         exit(-1);
//     }
//     point intPoint = line.src + line.dir * t;
//     point normal = intPoint - this->center;
//     normal.normalize();
//     point toCamera = line.dir * (-1);
//     toCamera.normalize();
//     double lambert = 0;
//     double phong = 0;
//     for(auto x : rt->lights){
//         if(!x->inShadow(intPoint, rt)){
//             point toSrc = x->position - intPoint;
//             toSrc.normalize();
//             lambert += normal.dotProduct(toSrc);
//             point reflected = line.dir - normal * ( line.dir.dotProduct(normal));
//             reflected.normalize();
//             phong += pow(reflected.dotProduct(toSrc), this->shininess);
//         }
//     }
//     point color = this->color * (this->coeffs.x + this->coeffs.y * lambert + this->coeffs.z * phong);
//     return color;
// }

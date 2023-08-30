#include <cmath>
#include <iostream>
#include "1805008_objects.hpp"
#include "1805008_lightSource.hpp"
#include "1805008_raytracer.hpp"



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
    textureW = new bitmap_image("texture_w.bmp");
    textureB = new bitmap_image("texture_b.bmp");

}

CheckerBoard* CheckerBoard::parseCheckerBoard(std::ifstream &f){
    double a;
    f >> a;
    point coeff = point::parsePoint(f);
    return new CheckerBoard(a, {coeff.x, coeff.y, 0, coeff.z});
}

void CheckerBoard::draw(point eyePos){
    int t = useTexture;
    useTexture = 0;
    point color = this->getColor(eyePos);
    useTexture = t;
    int x = eyePos.x / this->w;
    int y = eyePos.y / this->w;
    if(eyePos.x < 0) --x;
    if(eyePos.y  < 0) -- y;
    x *= this->w;
    y *= this->w;
    x -= 30 * this->w;
    y -= 30 * this->w;
    point black(1, 1, 1);
    point currColor = black - color;
    int c = 0;
    glPushMatrix();
        glBegin(GL_QUADS);
        for(int yi = 0; yi < 60; ++yi){
            // point currColor = color;
            for(int xi = 0; xi < 60; ++xi){
                currColor = black - currColor;
                glColor3f(currColor.x, currColor.y, currColor.z);
                glVertex3f(x + xi * w,y + yi * w, 0);
                glVertex3f(x + (xi+1) * w,y + yi * w, 0);
                glVertex3f(x + (xi+1) * w,y + (yi+1) * w, 0);
                glVertex3f(x + xi * w,y + (yi+1) * w, 0);

                

            }
            currColor = black - currColor;

        }
        
        glEnd();
    glPopMatrix();
}

IntersectionReturnVal Sphere::intersection(const Line &line){
    point R0 = this->center - line.src;
    double td = R0.dotProduct(line.dir);
    double r2 = radius * radius;
    double eps = EPS;
    if( R0.dotProduct(R0) > (r2 + eps) && td < 0){
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
    int i = p.x / this->w;
    int j = p.y / this->w;
    point color;
    bitmap_image *image;
    if(p.x >= 0 && p.y >= 0 || (p.x < 0 && p.y < 0)){
        if((i + j) % 2 == 0){
            
            color = {1, 1, 1};
            image = this->textureW;
        }
        else{
            color = {0, 0, 0};
            image = this->textureB;
        }
    }
    else{
        if((i + j) % 2 == 0){
            color = {0, 0, 0};
            image = this->textureB;
      
        }
        else{
            color = {1, 1, 1};
            image = this->textureW;
        
        }
        
    }

    if(useTexture){
        int texture_width = image->width();
        int texture_height = image->height();
        if(p.x < 0) --i;
        if(p.y < 0) --j;
        double offset_x = p.x - i * this->w;
        double offset_y = p.y - j * this->w;
        int x = offset_x * texture_width / this->w;
        int y = offset_y * texture_height / this->w;
        unsigned char r, g, b;
        image->get_pixel(x, y, r, g, b); 

        return  point( r/255.0, g/255.0, b/255.0);
    }

    return color;


    // return color;
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

#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>
#include <vector>

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

// Global variables
GLfloat eyex = 1, eyey = 2, eyez = 2;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;
bool isAxes = true, isCube = false, isPyramid = false;

float sideColors[][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {1, 1, 0},
    {0, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {.5, 0, 1},
};

int rotAngleZ = 0;
int rotAngleX = 0;
float scale = 1;
float scaleInc = .01;
double dist = (1.0 / 3);
double dihedralAngle = 109.47;
double cylinderAngle = 70.5287794;
double cylinderMaxR = (1.0 / 3) / sin((cylinderAngle / 2) * (M_PI / 180));
double cylinderMinR = 0;
double d2r = M_PI / 180;
// double cylinderMaxDist = (2.0/3) / (tan((cylinderAngle / 2) * (M_PI / 180)));
double cylinderMaxDist = (1.0/3) / tan((dihedralAngle / 2) * d2r) + (1./3) / tan((cylinderAngle / 2) * d2r)  ;
double sphereMaxR = 1./sqrt(3);


// float rotation

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,1);
    glEnd();
}


void drawTriangle(int colorIndex){
    glBegin(GL_TRIANGLES);
        glColor3fv(sideColors[colorIndex]);
        glVertex3f(1,0,0);
        glVertex3f(0,1,0);
        glVertex3f(0,0,1);
    glEnd();
}

// void drawSide(int angle,float x, float y, float z,int colorIndex){
//     glPushMatrix();
//         glRotatef(angle, x, y, z);
//         drawTriangle(colorIndex);
//     glPopMatrix();
// }

void drawPlane(int angle, float x, float y, float z, int colorIndex){
    glPushMatrix();
        glRotatef(angle, x, y, z);
        glTranslatef(dist - scale * dist, dist - scale * dist, dist - scale * dist);
        glScaled(scale,scale,scale);
        drawTriangle(colorIndex);
    glPopMatrix();
}

void drawCylinder(double h, double r, double startAngle, double endAngle, int segments){
    startAngle *= (M_PI / 180.);
    endAngle *= (M_PI / 180.);

    double angleGap = (endAngle - startAngle);
    double delta = angleGap / segments;
    double s = startAngle;
    double xf1, zf1, xf2, zf2;
    glBegin(GL_QUADS);
        while(s < endAngle){
            xf1 = r * cos(s);
            zf1 = r * sin(s);
            xf2 = r * cos(s + delta);
            zf2 = r * sin(s + delta);

            glVertex3d(xf1, -h / 2, zf1);
            glVertex3d(xf1, h / 2, zf1);
            glVertex3d(xf2, h / 2, zf2);
            glVertex3d(xf2, -h / 2, zf2);
            s += delta;
        }
    glEnd();
}

// void drawCylinder(double height, double radius, int segments) {
//     double tempx = radius, tempz = 0;
//     double currx, currz;
//     glBegin(GL_QUADS);
//         for (int i = 1; i <= segments; i++) {
//             double theta = i * 2.0 * M_PI / segments;
//             currx = radius * cos(theta);
//             currz = radius * sin(theta);

//             GLfloat c = (2+cos(theta))/3;
//             glColor3f(c,c,c);
//             glVertex3f(currx, height, currz);
//             glVertex3f(currx, 0, currz);

//             glVertex3f(tempx, 0, tempz);
//             glVertex3f(tempx, height, tempz);

//             tempx = currx;
//             tempz = currz;
//         }
//     glEnd();
// }


void drawOctaHedral(){

    for(int i = 0; i < 4; ++i)
        drawPlane(i * 90, 0, 1, 0, i);
    
    glPushMatrix();
        glRotatef(180, 1, 0, 0);
        for(int i = 0; i < 4; ++i)
            drawPlane(i * 90, 0, 1, 0, i + 4);
        
    glPopMatrix();

}

void drawSide(int xs, int ys, int zs, int rotateC, int rotateA){
    double r = (1 - scale) * cylinderMaxR;
    double off = ((1 - scale) * cylinderMaxDist) / sqrt(2);
    glPushMatrix();
        glColor3f(1, .5, .8);
        glTranslatef(xs * (.5 - off), ys * (.5 - off), zs * (.5 - off));
        glRotatef(rotateA, 1 - abs(xs), 01 - abs(ys), 1 - abs(zs));
        glPushMatrix();
        glScalef(1, (scale) * sqrt(2),1);
        glRotatef(rotateC, 0, 1, 0);
        drawCylinder(1, r, -cylinderAngle / 2 , cylinderAngle / 2 , 100);
        glPopMatrix();
    glPopMatrix();
}

void drawSides(){
    drawSide(1, 1, 0 , 0, 45);
    drawSide(0, 1, 1 , -90, -45);
    drawSide(0, 1, -1 , 90, 45);
    drawSide(-1, 1, 0 , -180, -45);
    glPushMatrix();
        glRotatef(90, 1, 0, 0);
        drawSide(1, 1, 0 , 0, 45);
        drawSide(0, 1, 1 , -90, -45);
        drawSide(0, 1, -1 , 90, 45);
        drawSide(-1, 1, 0 , -180, -45);

    glPopMatrix();
    glPushMatrix();
        glRotatef(180, 1, 0, 0);
        drawSide(1, 1, 0 , 0, 45);
        drawSide(0, 1, 1 , -90, -45);
        drawSide(0, 1, -1 , 90, 45);
        drawSide(-1, 1, 0 , -180, -45);

    glPopMatrix();
    glPushMatrix();
        glRotatef(270, 1, 0, 0);
        drawSide(1, 1, 0 , 0, 45);
        drawSide(0, 1, 1 , -90, -45);
        drawSide(0, 1, -1 , 90, 45);
        drawSide(-1, 1, 0 , -180, -45);

    glPopMatrix();
    // drawSide(1, 0, -1, 90);
    // drawSide(-1, 1);
    // drawSide(-1, -1);

}
struct point {
    GLfloat x, y, z;
};

void drawSphere(double radius, int stacks, int slices) {
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

    glBegin(GL_QUADS);
        for (int j = 0; j < stacks; j++) {
            for (int i = 0; i < slices; i++) {
                GLfloat c = (2+cos((i+j) * 2.0 * M_PI / slices)) / 3;
                glColor3f(c,c,c);
                glVertex3f(points[j][i].x, points[j][i].y, points[j][i].z);
                glVertex3f(points[j][i+1].x, points[j][i+1].y, points[j][i+1].z);

                glVertex3f(points[j+1][i+1].x, points[j+1][i+1].y, points[j+1][i+1].z);
                glVertex3f(points[j+1][i].x, points[j+1][i].y, points[j+1][i].z);
            }
        }
    glEnd();
}


std::vector<std::vector<point>> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    std::vector<std::vector<point>> vs;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;
        
        std::vector<point> curr;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
            curr.push_back({v[0], v[1], v[2]});
        }
        vs.push_back(curr);
    }

    return vs;
}

auto x = buildUnitPositiveX(5);

void drawSphereSide(){
    glPushMatrix();
        glColor3f(.5, 1, .5);
        glTranslatef(scale, 0, 0);
        glScalef((1-scale) / sqrt(3) , (1-scale) / sqrt(3), (1-scale) / sqrt(3));
        glBegin(GL_QUADS);
        int n = x.size();
        for(int i = 0; i + 1 < n; ++i){
            int m = x[i].size();
            for(int j = 0; j + 1 < m; ++j){
                glVertex3f(x[i][j].x, x[i][j].y,x[i][j].z);
                glVertex3f(x[i+1][j].x, x[i+1][j].y,x[i+1][j].z);
                glVertex3f(x[i+1][j+1].x, x[i+1][j+1].y,x[i+1][j+1].z);
                glVertex3f(x[i][j+1].x, x[i][j+1].y,x[i][j+1].z);
            }
        }
        glEnd();
    glPopMatrix();
}

void drawSpheres(){
    glPushMatrix();
        drawSphereSide();
        glRotatef(90, 0, 1, 0);
        drawSphereSide();
        glRotatef(90, 0, 1, 0);
        drawSphereSide();
        glRotatef(90, 0, 1, 0);
        drawSphereSide();
        glRotatef(90, 0, 0, 1);
        drawSphereSide();
        glRotatef(-90, 0, 0, 1);
        drawSphereSide();
    glPopMatrix();
}

void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // control viewing (or camera)
    gluLookAt(eyex,eyey,eyez,
              centerx,centery,centerz,
              upx,upy,upz);
    
    glPushMatrix();
        glRotatef(rotAngleZ, 0, 1, 0);
        glRotatef(rotAngleX, 1, 0, 0);

    // draw
    // if (isAxes) drawAxes();
    // if (isCube) drawCube();
    // if (isPyramid) drawPyramid();
    // drawCube();
    drawAxes();

    // drawSphereQuad(.5, 50, 50);

    drawOctaHedral();


    // drawCylinder(.5, .2, 10);
    // drawSide(0, 0, 1, 0, 0);
    // drawSide(90, 0, 1, 0, 1);
    drawSides();
    drawSpheres();
    // drawSpheres();
    
    // glPushMatrix();
    //     glRotatef(90, 0, 1, 0);
    //     drawSphereSide();
    //     glRotatef(90, 0, 1, 0);
    //     drawSphereSide();
    // glPopMatrix();
    
    // drawSphere(.3, 500, 500);
    // double r = (1 - scale) * cylinderMaxR;
    // double off = ((1 - scale) * cylinderMaxDist) / sqrt(2);
    // std::cout << "r : " << r << "\n";
    // glPushMatrix();
    //     glColor3f(1, 1, 1);
    //     // glTranslatef(.5 - r / sqrt(2), .5 - r / sqrt(2), 0);
    //     glTranslatef(.5 - off, .5 - off, 0);
    //     // glRotatef(cylinderAngle / 2, 0, 1, 0);
    //     glRotatef(45, 0, 0, 1);
    //     glPushMatrix();
    //     glScalef(1, (scale) * sqrt(2),1);
    //     drawCylinder(1, r, -cylinderAngle / 2 , cylinderAngle / 2 , 100);
    //     glPopMatrix();
    //     // drawCylinder(1, .1, -cylinderAngle / 2, cylinderAngle / 2, 100);

    // glPopMatrix();
    // glPushMatrix();
    //     glScaled(1,1,1);
    //     drawSide(0, 0, 1, 0, 0);
    // glPopMatrix();

    // glPushMatrix();
    //     glTranslatef(dist - scale * dist, dist - scale * dist, dist - scale * dist);
    //     glScaled(scale,scale,scale);
    //     // for(int i = 0; i < 4; ++i)
    //     //     drawSide(i * 90, 0, 1, 0, i);
    //     drawSide(0, 0, 1, 0, 0);
    // glPopMatrix();

    // glPushMatrix();
    //     glRotatef(90, 0, 1, 0);
    //     glTranslatef(dist - scale * dist, dist - scale * dist, dist - scale * dist);
    //     glScaled(scale,scale,scale);
    //     // for(int i = 0; i < 4; ++i)
    //     //     drawSide(i * 90, 0, 1, 0, i);
    //     drawSide(0, 0, 1, 0, 1);
    // glPopMatrix();

    // glPushMatrix();
    //     // glScalef(.5, .5, .5);
    //     glScalef(scale,scale,scale);
    //     for(int i = 0; i < 4; ++i)
    //         drawSide(i * 90, 0, 1, 0, i);
    // glPopMatrix();

    // glPushMatrix();
    //     glRotatef(180, 1, 0, 0);
    //     glScalef(scale,scale,scale);
    //     // glScalef(.5, .5, .5);
    //     for(int i = 0; i < 4; ++i)
    //         drawSide(i * 90, 0, 1, 0, i + 4);
    // glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}


/* Callback handler for normal-key event */
void keyboard(unsigned char key, int x, int y) {
    // key is the char pressed, e.g., 'a' or 27 for ESC
    // (x, y) is the mouse location in Windows' coordinates
    switch (key) {
    case '.':
        scale = fmin(scale + scaleInc, 1);
        break;
    case ',':
        scale = fmax(0, scale-scaleInc);
    
    default:
        break;
    }
    glutPostRedisplay();
}


void special(int key, int x, int y) {
    // specialKey: GLUT_KEY_* (* for LEFT, RIGHT, UP, DOWN, HOME, END, PAGE_UP, PAGE_DOWN, F1,...F12). 
    // (x, y) is the mouse location in Windows' coordinates
    switch(key) {
    case GLUT_KEY_LEFT:
        rotAngleZ += 5;
        break;
    case GLUT_KEY_RIGHT:
        rotAngleZ -= 5;
        break;
    case GLUT_KEY_UP:
        rotAngleX += 5;
        break;
    case GLUT_KEY_DOWN:
        rotAngleX -= 5;
    }
    glutPostRedisplay();
}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(640, 640);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    auto x = buildUnitPositiveX(3);
    std::cout << x[0].size() << "\n";
    // std::cout << x.size() << "\n";
    // for(int i = 0; i < x.size()/3; ++i){
    //     int b = 3 * i;
    //     std::cout << x[b] << " " << x[b+1] << " " << x[b + 2] << "\n";
    // }
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    return 0;
}

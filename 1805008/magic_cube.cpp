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

const float rotationRate = .05;
const float movementRate = .1;

struct point {
    GLfloat x, y, z;

    point operator*(float s) const{
        point t = {x * s, y * s, z * s};
        return t;
    }
    point operator+(point p) const{
        point t = {x + p.x, y + p.y, z + p.z};
        return t;
    }

    point operator-(point p) const{
        point t = {x - p.x, y - p.y, z - p.z};
        return t;
    }
};

point crossProduct(point& p1, point& p2){
    return {p1.y * p2.z - p2.y * p1.z, -p1.x * p2.z + p2.x * p1.z, p1.x * p2.y - p2.x * p1.y};
}

void rotateVector(point& axis, point& vector, float angle){
    point u = crossProduct(axis, vector);
    vector = u * sin(angle) + vector * cos(angle);
}

void normalize(point& p){
    double mag = sqrt(p.x  * p.x + p.y * p.y + p.z * p.z);
    if(mag > 1e-7){
        p = p * (1. / mag);
    }
}

// point eye = {0, 0 , 4};
point eyePos = {2.56357 ,1.91455, 2.92836};

// point look = {0, 0, -1};
point look = {-0.615372 ,-0.337015 ,-0.712558};
// point right = {1, 0, 0};
point right = {0.759506 ,-0.0116736, -0.650395};
// point up = {0, 1, 0};
point up = {-0.210875 ,0.941427 ,-0.263148};

// Global variables
GLfloat eyex = 1, eyey = 2, eyez = 2;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;
bool isAxes = true, isCube = false, isPyramid = false;

float planeColors[][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {1, 1, 0},
    {0, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
    {.5, 0, 1},
};

float sphereColors[][3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {1, 1, 0},
    {1, 0, 1},
    {0, 1, 1}
};

float cylinderColor[3] = {0.31, 0.365, 0.184};



int rotAngleY = 0;
float scale = 1;
float scaleInc = .01;
double dist = (1.0 / 3);
double dihedralAngle = 109.47;
double cylinderAngle = 70.5287794;
double cylinderMaxR = (1.0 / 3) / sin((cylinderAngle / 2) * (M_PI / 180));
double cylinderMinR = 0;
double d2r = M_PI / 180;
double cylinderMaxDist = (1.0/3) / tan((dihedralAngle / 2) * d2r) + (1./3) / tan((cylinderAngle / 2) * d2r)  ;
double sphereMaxR = 1./sqrt(3);
const point center = {1./3, 1./3, 1./3};

std::vector<std::vector<point>> buildUnitPositiveX(int);


auto x = buildUnitPositiveX(5);


void drawTriangle(int colorIndex){
    glBegin(GL_TRIANGLES);
        glColor3fv(planeColors[colorIndex]);
        glVertex3f(1,0,0);
        glVertex3f(0,1,0);
        glVertex3f(0,0,1);
    glEnd();
}


void drawPlane(int angle, point rotV, int colorIndex){
    point t = center * (1 - scale);
    glPushMatrix();
        glRotatef(angle, rotV.x, rotV.y, rotV.z);
        glTranslatef(t.x, t.y, t.z);
        glScaled(scale,scale,scale);
        drawTriangle(colorIndex);
    glPopMatrix();
}

void drawOctaHedral(){

    for(int i = 0; i < 4; ++i)
        drawPlane(i * 90, {0, 1, 0}, i);
    
    glPushMatrix();
        glRotatef(180, 1, 0, 0);
        for(int i = 0; i < 4; ++i)
            drawPlane(i * 90, {0, 1, 0}, i + 4);
        
    glPopMatrix();

}


void drawCylinder(int segments){
    double h = 1;
    double r = 1;
    double startAngle = (-cylinderAngle / 2) * (M_PI / 180.);
    double endAngle = (cylinderAngle / 2) * (M_PI / 180.);

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


void drawSide(){
    double r = (1 - scale) * cylinderMaxR;
    double off = ((1 - scale) * cylinderMaxDist) / sqrt(2);
    glPushMatrix();
        glColor3fv(cylinderColor);
        glTranslatef((.5 - off),(.5 - off), 0);
        glRotatef(45, 0,0,1);
        glPushMatrix();
            glScalef(1, (scale) * sqrt(2),1);
            glScalef(r, 1, r);
            drawCylinder(100);
        glPopMatrix();
    glPopMatrix();
}


void drawSides(){
    glPushMatrix();

        drawSide();
        glRotatef(90, 0, 1, 0);
        drawSide();
        glRotatef(90, 0, 1, 0);
        drawSide();
        glRotatef(90, 0, 1, 0);
        drawSide();
        glRotatef(90, 0, 1, 0);

        glRotatef(90, 1, 0, 0);

        drawSide();
        glRotatef(90, 0, 0, 1);
        drawSide();
        glRotatef(90, 0, 0, 1);
        drawSide();
        glRotatef(90, 0, 0, 1);
        drawSide();
        glRotatef(90, 0, 0, 1);

        glRotatef(90, 0, 0, 1);
        glRotatef(90, 1, 0, 0);

        drawSide();
        glRotatef(90, 0, 1, 0);
        drawSide();
        glRotatef(90, 0, 1, 0);
        drawSide();
        glRotatef(90, 0, 1, 0);
        drawSide();
        glRotatef(90, 0, 1, 0);

    glPopMatrix();
}

// reference : https://www.songho.ca/opengl/gl_sphere.html
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


void drawSpherePortion(int colorIndex){
    glPushMatrix();
        glColor3fv(sphereColors[colorIndex]);
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
        drawSpherePortion(0);
        glRotatef(90, 0, 1, 0);
        drawSpherePortion(1);
        glRotatef(90, 0, 1, 0);
        drawSpherePortion(2);
        glRotatef(90, 0, 1, 0);
        drawSpherePortion(3);
        glRotatef(90, 0, 1, 0);
        
        glRotatef(90, 0, 0, 1);

        drawSpherePortion(4);
        glRotatef(-180, 0, 0, 1);
        drawSpherePortion(5);
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
    gluLookAt(eyePos.x, eyePos.y, eyePos.z,
              eyePos.x + look.x, eyePos.y + look.y, eyePos.z + look.z,
              up.x, up.y, up.z);
    
    glPushMatrix();
        glRotatef(rotAngleY, 0, 1, 0);
        // glRotatef(rotAngleX, 1, 0, 0);


        // drawAxes();
        drawOctaHedral();
        drawSides();
        drawSpheres();

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
    point prev;
    switch (key) {
    case '.':
        scale = fmin(scale + scaleInc, 1);
        break;
    case ',':
        scale = fmax(0, scale-scaleInc);
        break;
    case 'd':
        rotAngleY += 5;
        break;
    case 'a':
        rotAngleY -= 5;
        break;
    case '2':
        rotateVector(up, look, -rotationRate);
        rotateVector(up, right, -rotationRate);
        break;
    case '1':
        rotateVector(up, look, rotationRate);
        rotateVector(up, right, rotationRate);
        break;
    case '3':
        rotateVector(right, look, rotationRate);
        rotateVector(right, up, rotationRate);
        break;
    case '4':
        rotateVector(right, look, -rotationRate);
        rotateVector(right, up, -rotationRate);
        break;
    case '5':
        rotateVector(look, right, rotationRate);
        rotateVector(look, up, rotationRate);
        break;
    case '6':
        rotateVector(look, right, -rotationRate);
        rotateVector(look, up, -rotationRate);
        break;
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
        eyePos = eyePos - right * movementRate;
        break;
    case GLUT_KEY_RIGHT:
        eyePos = eyePos + right * movementRate;
        break;
    case GLUT_KEY_UP:
        eyePos = eyePos + look * movementRate;
        break;
    case GLUT_KEY_DOWN:
        eyePos = eyePos - look * movementRate;
        break;
    case GLUT_KEY_PAGE_UP:
        eyePos = eyePos + up * movementRate;
        break;
    case GLUT_KEY_PAGE_DOWN:
        eyePos = eyePos - up * movementRate;
        break;
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
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    return 0;
}

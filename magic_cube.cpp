#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>

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

void drawSide(int angle, float x, float y, float z, int colorIndex){
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
        drawSide(i * 90, 0, 1, 0, i);
    
    glPushMatrix();
        glRotatef(180, 1, 0, 0);
        for(int i = 0; i < 4; ++i)
            drawSide(i * 90, 0, 1, 0, i + 4);
        
    glPopMatrix();

}


double distCylinder = 1./sqrt(2);
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


    // drawOctaHedral();


    // drawCylinder(.5, .2, 10);
    drawSide(0, 0, 1, 0, 0);
    drawSide(90, 0, 1, 0, 1);

    double r = (1 - scale) * cylinderMaxR;
    double off = ((1 - scale) * cylinderMaxDist) / sqrt(2);
    std::cout << "r : " << r << "\n";
    glPushMatrix();
        glColor3f(1, 1, 1);
        // glTranslatef(.5 - r / sqrt(2), .5 - r / sqrt(2), 0);
        glTranslatef(.5 - off, .5 - off, 0);
        // glRotatef(cylinderAngle / 2, 0, 1, 0);
        glRotatef(45, 0, 0, 1);
        glPushMatrix();
        glScalef(1, (scale) * sqrt(2),1);
        drawCylinder(1, r, -cylinderAngle / 2 , cylinderAngle / 2 , 100);
        glPopMatrix();
        // drawCylinder(1, .1, -cylinderAngle / 2, cylinderAngle / 2, 100);

    glPopMatrix();
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
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    return 0;
}

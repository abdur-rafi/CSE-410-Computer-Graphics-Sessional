#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>
#include <vector>
#include "config.hpp"
#include "objects.hpp"
#include "utility.hpp"
#include <fstream>
#include "raytracer.hpp"


RayTracer *rt;
/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

const float rotationRate = .05;
const float movementRate = 5;

CameraConfig cConfig = {
    {0, 0, -1},
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 200}
};


int rotAngleY = 0;

void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(100,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);
        glVertex3f(0,100,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,100);
    glEnd();
}


void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of glu`At
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // control viewing (or camera)
    gluLookAt(cConfig.eyePos.x, cConfig.eyePos.y, cConfig.eyePos.z,
              cConfig.eyePos.x + cConfig.l.x, cConfig.eyePos.y + cConfig.l.y, cConfig.eyePos.z + cConfig.l.z,
              cConfig.u.x, cConfig.u.y, cConfig.u.z);
    
    glPushMatrix();
        glRotatef(rotAngleY, 0, 1, 0);
        // glRotatef(rotAngleX, 1, 0, 0);
        drawAxes();
        rt->draw(cConfig.eyePos);

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
    gluPerspective(rt->getConfig().fovY, rt->getConfig().ratio, rt->getConfig().near, rt->getConfig().far);
}


/* Callback handler for normal-key event */
void keyboard(unsigned char key, int x, int y) {
    // key is the char pressed, e.g., 'a' or 27 for ESC
    // (x, y) is the mouse location in Windows' coordinates
    point prev, t1, t2, center;
    switch (key) {

    case 'd':
        rotAngleY += 5;
        break;
    case 'a':
        rotAngleY -= 5;
        break;
    case '2':
        rotateVector(cConfig.u, cConfig.l, -rotationRate);
        rotateVector(cConfig.u, cConfig.r, -rotationRate);
        break;
    case '1':
        rotateVector(cConfig.u, cConfig.l, rotationRate);
        rotateVector(cConfig.u, cConfig.r, rotationRate);
        break;
    case '3':
        rotateVector(cConfig.r, cConfig.l, rotationRate);
        rotateVector(cConfig.r, cConfig.u, rotationRate);
        break;
    case '4':
        rotateVector(cConfig.r, cConfig.l, -rotationRate);
        rotateVector(cConfig.r, cConfig.u, -rotationRate);
        break;
    case '5':
        rotateVector(cConfig.l, cConfig.r, rotationRate);
        rotateVector(cConfig.l, cConfig.u, rotationRate);
        break;
    case '6':
        rotateVector(cConfig.l, cConfig.r, -rotationRate);
        rotateVector(cConfig.l, cConfig.u, -rotationRate);
        break;
    case '0':
        rt->generatePointBuffer(cConfig);
        rt->generateImage(cConfig);
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
        cConfig.eyePos = cConfig.eyePos - cConfig.r * movementRate;
        break;
    case GLUT_KEY_RIGHT:
        cConfig.eyePos = cConfig.eyePos + cConfig.r * movementRate;
        break;
    case GLUT_KEY_UP:
        cConfig.eyePos = cConfig.eyePos + cConfig.l * movementRate;
        break;
    case GLUT_KEY_DOWN:
        cConfig.eyePos = cConfig.eyePos - cConfig.l * movementRate;
        break;
    case GLUT_KEY_PAGE_UP:
        cConfig.eyePos = cConfig.eyePos + cConfig.u * movementRate;
        break;
    case GLUT_KEY_PAGE_DOWN:
        cConfig.eyePos = cConfig.eyePos - cConfig.u * movementRate;
        break;
    }
    glutPostRedisplay();
}


/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

    std::ifstream f;
    f.open("description.txt");
    
    rt = new RayTracer(f);

    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(768, 768);               // Set the window's initial width & height
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
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <chrono>
#include <ctime>


const float CLOCK_RADIUS = .4f;
const float LARGE_TICK = .1f;
const float SMALL_TICK = .05f;
const float CLOCK_OUTER_RADIUS = .5f;
const float BOB_RADIUS = .09f;
const float ROD_LEN = .4f;
const float JOINT_RADIUS = .05f;
const float CLOCK_CENTER_X = 0.0f;
const float CLOCK_CENTER_Y = 0.3f;
const float ANGLE_DELTA = (M_PI * 2) / 100;
const float MAX_ANGLE = (M_PI) / 4;
const float OMEGA = M_PI;
const float ROD_WIDTH = .05f;
const int INTERVAL = 40;
const float HOUR_LEN = .3f;
const float HOUR_WIDTH = .03f;
const float MIN_LEN = .34f;
const float MIN_WIDTH = .02f;
const float SEC_LEN = .38;
const float SEC_WIDTH = .015f;
const float EPS = 1e-5;
const float QUARTER = M_PI / 2 ;
int time_ = 0;

float xFromAngle(float angle, float radius, float ox){
    return ox + cosf(angle) * radius;
}
float yFromAngle(float angle, float radius, float ox){
    return ox + sinf(angle) * radius;
}


void timer(int value){
    time_ += INTERVAL;
    if(time_ > 2000)
        time_ -= 2000;
    glutPostRedisplay();
    // std::cout<< time_ << "\n";
    glutTimerFunc(INTERVAL,timer, 0);
}

void drawCircle(GLfloat x, GLfloat y, GLfloat radius, bool filled){
    if(!filled)
        glBegin(GL_LINE_LOOP);
    else{
        glBegin(GL_TRIANGLE_FAN);
    }
        glColor3f(1.,1.,1.);
        double angle = 0;
        while(angle < M_PI * 2){
            glVertex2f(xFromAngle(angle, radius, x),
            yFromAngle(angle, radius, y));
            angle += ANGLE_DELTA;
        }
    glEnd();
}

void initGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // glLineWidth(5);

}

float angleAtTime(float t){
    return MAX_ANGLE * cosf(OMEGA * t);
}

float getAngleIn2dCoord(float x1, float y1, float x2, float y2){
    float angle;
    float dy = y2 - y1;
    float dx = x2 - x1;

    if(abs(dx) <= EPS){
        if(dy < 0){
            angle = M_PI / 2;
        }
        else{
            angle = -M_PI / 2;
        }
    }
    else{
        angle = atanf(dy / dx);
    }
    return angle;
}

void drawRod(float jointCenterX, float jointCenterY, float bobCenterX, float bobCenterY, float width){
    float angle = getAngleIn2dCoord(jointCenterX, jointCenterY, bobCenterX, bobCenterY);
    // std :: cout << "angle : " << angle << "\n";
    float rodC1x = xFromAngle(angle - QUARTER , width / 2, jointCenterX);
    float rodC1y = yFromAngle(angle - QUARTER , width / 2, jointCenterY);


    float rodC2x = xFromAngle(angle + QUARTER, width / 2, jointCenterX);
    float rodC2y = yFromAngle(angle + QUARTER, width / 2, jointCenterY);


    float rodC3x = xFromAngle(angle - QUARTER, width / 2, bobCenterX);
    float rodC3y = yFromAngle(angle - QUARTER, width / 2, bobCenterY);


    float rodC4x = xFromAngle(angle + QUARTER, width / 2, bobCenterX);
    float rodC4y = yFromAngle(angle + QUARTER, width / 2, bobCenterY);



    glBegin(GL_QUADS);
        // glColor3f(1.,0.,0.);
        glVertex2f(rodC1x, rodC1y);
        // glVertex2f(bobCenterX, bobCenterY);
        glVertex2f(rodC2x, rodC2y);
        glVertex2f(rodC4x, rodC4y);
        glVertex2f(rodC3x, rodC3y);
        // glVertex2f(CLOCK_CENTER_X, CLOCK_CENTER_Y - CLOCK_OUTER_RADIUS);

    glEnd();

}

void drawClock(float angle){
    drawCircle(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_RADIUS, false);
    drawCircle(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_OUTER_RADIUS, false);

    double tickAngle = 0;
    double w = SMALL_TICK;
    glBegin(GL_LINES);
        for(int i = 0; i < 12; ++i){
        
            tickAngle += (M_PI / 6);
            
            if((i + 1) % 3 == 0){
                w = LARGE_TICK;
            }
            else{
                w = SMALL_TICK;
            }

            glVertex2f(
                xFromAngle(tickAngle,CLOCK_RADIUS,CLOCK_CENTER_X),
                yFromAngle(tickAngle, CLOCK_RADIUS, CLOCK_CENTER_Y)
            );


            glVertex2f(
                xFromAngle(tickAngle,CLOCK_RADIUS - w,CLOCK_CENTER_X),
                yFromAngle(tickAngle, CLOCK_RADIUS - w, CLOCK_CENTER_Y)
            );
        }

    glEnd();

}

float getTimeAngle(float t, float div){
    return QUARTER - t * (M_PI / div);
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);

    // pendulum

    float jointCenterX = CLOCK_CENTER_X;
    float jointCenterY = CLOCK_CENTER_Y - CLOCK_OUTER_RADIUS;

    drawCircle(jointCenterX, jointCenterY, JOINT_RADIUS, true);


    float angle = angleAtTime((float)time_ / 1000.) ;
    // std :: cout << time_ << " " << angle << "\n";
    float bobCenterX = xFromAngle(angle - M_PI / 2, ROD_LEN,CLOCK_CENTER_X);
    float bobCenterY = yFromAngle(angle - M_PI / 2, ROD_LEN,CLOCK_CENTER_Y - CLOCK_OUTER_RADIUS);
    drawCircle(
        bobCenterX,
        bobCenterY,
        BOB_RADIUS, 
        true
    );
    drawRod(jointCenterX, jointCenterY, bobCenterX, bobCenterY, ROD_WIDTH);

    // hands
    
    std::time_t currentTime = std::time(nullptr);

    std::tm* localTime = std::localtime(&currentTime);

    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;

    // std::cout << "hour : " << hour << "\n";

    float hourAngle = getTimeAngle(hour + (float)minute/60.0f, 6);
    // std:: cout << "hour angle " << hourAngle << "\n";

    drawRod(
        CLOCK_CENTER_X, 
        CLOCK_CENTER_Y, 
        xFromAngle(hourAngle ,HOUR_LEN, CLOCK_CENTER_X),
        yFromAngle(hourAngle ,HOUR_LEN, CLOCK_CENTER_Y),
        HOUR_WIDTH
    );

    float minuteAngle = getTimeAngle(minute + (float)second / 60.f, 30);
    drawRod(
        CLOCK_CENTER_X, 
        CLOCK_CENTER_Y, 
        xFromAngle(minuteAngle ,HOUR_LEN, CLOCK_CENTER_X),
        yFromAngle(minuteAngle ,HOUR_LEN, CLOCK_CENTER_Y),
        MIN_WIDTH
    );

    float secondAngle = getTimeAngle(second, 30);
    drawRod(
        CLOCK_CENTER_X, 
        CLOCK_CENTER_Y, 
        xFromAngle(secondAngle ,HOUR_LEN, CLOCK_CENTER_X),
        yFromAngle(secondAngle ,HOUR_LEN, CLOCK_CENTER_Y),
        MIN_WIDTH
    );


    drawClock(angle);





    // glFlush();
    glutSwapBuffers();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE); 
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Clock");
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    initGL();
    glutMainLoop();
    return 0;
}
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <ctime>

// clock circles
const float CLOCK_CENTER_X = 0.0f;
const float CLOCK_CENTER_Y = 0.3f;
const float CLOCK_RADIUS = .4f;
const float CLOCK_OUTER_RADIUS = .5f;
const float CENTER_KNOB_W = .02f;
const float CLOCK_OUTER_COLOR[] = {0.498, 0.588, 1};
const float CLOCK_INNER_COLOR[] = {0.  ,0.  ,0.133};
const float CENTER_KNOB_COLOR[] = {0.498, 0.588, 1};
// const float CENTER_KNOB_COLOR[] = {0.016, 0.059, 0.086};
const float CENTER_KNOB_R = .025f;
// ticks
const float LARGE_TICK = .1f;
const float SMALL_TICK = .05f;
const float TICKS_COLOR[] = {1, 1, 1};
// pendulum
const float BOB_RADIUS = .1f;
const float JOINT_RADIUS = .05f;
const float ROD_LEN = .45f;
const float ROD_WIDTH = .05f;
const float ANGLE_DELTA = (M_PI * 2) / 100;
const float MAX_ANGLE = (M_PI) / 6;
const float OMEGA = M_PI;
const float JOIN_COLOR[] = {0.016, 0.059, 0.086};
const float BOB_COLOR[] = {0.996, 0.843, 0.4};


// hands
const float HOUR_LEN = .13f;
const float H_TR_HEIGHT = .1f;
const float HOUR_WIDTH = .025f;

const float MIN_LEN = .18f;
const float M_TR_HEIGHT = .1f;
const float MIN_WIDTH = .018f;

const float SEC_LEN = .2;
const float S_TR_HEIGHT = .1f;
const float SEC_WIDTH = .01f;

const float OUTER_BODY_START_ANGLE = -60. * (M_PI / 180);
const float OUTER_BODY_SPAN = 60 * (M_PI / 180);
const float OUTER_BODY_H = .7;
const float HANDS_COLOR[] = {1, 1, 1};

const float OUTER_BODY_COLOR[] = {0, 0.071, 0.259};
// others
const float EPS = 1e-5;
const int INTERVAL = 20;
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

void drawCircle(GLfloat x, GLfloat y, GLfloat radius, bool filled, const float* color){
    if(!filled)
        glBegin(GL_LINE_LOOP);
    else{
        glBegin(GL_TRIANGLE_FAN);
    }
        glColor3fv(color);
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

void drawTriag(float mx, float my, float cx, float cy, float width){
    float angle = getAngleIn2dCoord(mx, my, cx, cy);

    float rodC1x = xFromAngle(angle - QUARTER , width / 2, mx);
    float rodC1y = yFromAngle(angle - QUARTER , width / 2, my);


    float rodC2x = xFromAngle(angle + QUARTER, width / 2, mx);
    float rodC2y = yFromAngle(angle + QUARTER, width / 2, my);

    glBegin(GL_TRIANGLES);
        glVertex2f(rodC1x, rodC1y);
        glVertex2f(rodC2x, rodC2y);
        glVertex2f(cx, cy);

    glEnd();


}

void drawClock(){

    drawCircle(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_OUTER_RADIUS, true, CLOCK_OUTER_COLOR);
    drawCircle(CLOCK_CENTER_X, CLOCK_CENTER_Y, CLOCK_RADIUS, true, CLOCK_INNER_COLOR);

    double tickAngle = 0;
    double w = SMALL_TICK;

    glColor3fv(TICKS_COLOR);
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

void drawCenterKnob(){
    glColor3fv(CENTER_KNOB_COLOR);
    // drawCircle(CLOCK_CENTER_X, CLOCK_CENTER_Y, CENTER_KNOB_R, true, CENTER_KNOB_COLOR);
    glBegin(GL_QUADS);
        glVertex2f(-CENTER_KNOB_W + CLOCK_CENTER_X,-CENTER_KNOB_W + CLOCK_CENTER_Y);
        glVertex2f(CENTER_KNOB_W + CLOCK_CENTER_X,-CENTER_KNOB_W + CLOCK_CENTER_Y);
        glVertex2f(CENTER_KNOB_W + CLOCK_CENTER_X,CENTER_KNOB_W + CLOCK_CENTER_Y);
        glVertex2f(-CENTER_KNOB_W + CLOCK_CENTER_X,CENTER_KNOB_W + CLOCK_CENTER_Y);
    glEnd();
}

float getTimeAngle(float t, float div){
    return QUARTER - t * (M_PI / div);
}

void drawPendulum(){
    float jointCenterX = CLOCK_CENTER_X;
    float jointCenterY = CLOCK_CENTER_Y - CLOCK_OUTER_RADIUS;



    float angle = angleAtTime((float)time_ / 1000.) ;
    // std :: cout << time_ << " " << angle << "\n";
    float bobCenterX = xFromAngle(angle - M_PI / 2, ROD_LEN,CLOCK_CENTER_X);
    float bobCenterY = yFromAngle(angle - M_PI / 2, ROD_LEN,CLOCK_CENTER_Y - CLOCK_OUTER_RADIUS);
    drawCircle(
        bobCenterX,
        bobCenterY,
        BOB_RADIUS, 
        true,
        BOB_COLOR
    );
    drawRod(jointCenterX, jointCenterY, bobCenterX, bobCenterY, ROD_WIDTH);
    drawCircle(jointCenterX, jointCenterY, JOINT_RADIUS, true, JOIN_COLOR);

}

void drawHand(float angle, float len, float height, float width){

    float mx = xFromAngle(angle ,len, CLOCK_CENTER_X);
    float my = yFromAngle(angle ,len, CLOCK_CENTER_Y);

    float cx = xFromAngle(angle, len + height, CLOCK_CENTER_X);
    float cy = yFromAngle(angle, len + height, CLOCK_CENTER_Y);

    drawRod(
        CLOCK_CENTER_X, 
        CLOCK_CENTER_Y, 
        mx,
        my,
        width
    );
    drawTriag(
        mx, 
        my, 
        cx,
        cy,
        width
    );
}

void drawHands(){
    std::time_t currentTime = std::time(nullptr);

    std::tm* localTime = std::localtime(&currentTime);

    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;


    float hourAngle = getTimeAngle(hour + (float)minute / 60.0f , 6);
    
    glColor3fv(HANDS_COLOR);
    // std::cout << "hour " << hour << "hour angle : " << hourAngle << "\n";
    drawHand(hourAngle, HOUR_LEN, H_TR_HEIGHT, HOUR_WIDTH);

    // std::cout << mx << " " << my << " " << cx << " " << cy << "\n";

    float minuteAngle = getTimeAngle(minute + (float)second / 60.f, 30);
    
    drawHand(minuteAngle, MIN_LEN, M_TR_HEIGHT, MIN_WIDTH);


    float secondAngle = getTimeAngle(second, 30);

    drawHand(secondAngle, SEC_LEN, S_TR_HEIGHT, SEC_WIDTH);
    

}

void drawOuterBody(){
    float spx = xFromAngle(OUTER_BODY_START_ANGLE, CLOCK_OUTER_RADIUS, CLOCK_CENTER_X);
    float spy = yFromAngle(OUTER_BODY_START_ANGLE, CLOCK_OUTER_RADIUS, CLOCK_CENTER_Y);

    float epx = xFromAngle(OUTER_BODY_START_ANGLE - OUTER_BODY_SPAN, CLOCK_OUTER_RADIUS, CLOCK_CENTER_X);
    float epy = yFromAngle(OUTER_BODY_START_ANGLE - OUTER_BODY_SPAN, CLOCK_OUTER_RADIUS, CLOCK_CENTER_Y);

    glBegin(GL_POLYGON);
        glColor3fv(OUTER_BODY_COLOR);
        glVertex2f(spx, spy);
        glVertex2f(spx, spy - OUTER_BODY_H);
        glVertex2f(epx, spy - OUTER_BODY_H);
        glVertex2f(epx, epy);

    glEnd();

}


void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    drawOuterBody();
    drawClock();
    drawHands();
    drawCenterKnob();
    // pendulum
    drawPendulum();
    // hands
    // clock body

    glutSwapBuffers();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE); 
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Clock");
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    initGL();
    glutMainLoop();
    return 0;
}
#include "camera.h"
#include <cmath>
#include <QVector3D>
#include <GL/glu.h>
Camera::Camera(){
    x = 0.0;
    y = 0.0;
    z = 0.0;
    centerx = 0;
    centery = 0;
    centerz = 0;
    angle = 0.0;
    anglev = 0.0;
    fovy = 1.0;
    radius = 100.0;
    Camera::updatePosition();
}
void Camera::setCenter(float cx, float cy, float cz) {
    centerx = cx;
    centery = cy;
    centerz = cz;
}
float Camera::cosine(long double ang)
{
    return (float) std::cos(ang*M_PI/180);
}

float Camera::sine(long double ang)
{
    return (float) std::sin(ang*M_PI/180);
}

void Camera::setAngle(GLfloat angle){
    this->angle = angle;
    Camera::updatePosition();
}

void Camera::setAngleV(GLfloat anglev){
    this->anglev = anglev;
    Camera::updatePosition();
}

void Camera::setRadius(GLfloat radius){
    this->radius = radius;
    Camera::updatePosition();
}

void Camera::updatePosition(){
    radiusb = radius * cosine(anglev);
    if (angle <= 90){
        x = radiusb* Camera::cosine(angle) + centerx;
        z = radiusb* Camera::sine(angle) + centerz;
    }
    else if (angle > 90 && angle <= 180){
        x = -1*(radiusb* Camera::sine(angle - 90)) + centerx;
        z = radiusb* Camera::cosine(angle - 90) + centerz;
    }
    else if (angle > 180 && angle <= 270){
        x = -1*(radiusb* Camera::cosine(angle - 180)) + centerx;
        z = -1*(radiusb* Camera::sine(angle - 180)) + centerz;
    }
    else if (angle > 270 && angle <= 360){
        x = radiusb* Camera::sine(angle - 270) + centerx;
        z = -1*(radiusb* Camera::cosine(angle - 270)) + centerz;
    }
    if (anglev <= -90) anglev = -89;
    else if (anglev >= 90) anglev = 89;
    y = radius * sine(anglev) + centery;
    Camera::setConfig();
}

void Camera::setConfig()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0*fovy, (GLfloat) view_w/(GLfloat) view_h, 0.1, 10000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, centerx, centery, centerz, 0.0, 1.0, 0.0);
    glLightModelf(GL_LIGHT_MODEL_AMBIENT, 1);

    GLfloat pos[4] = {x,y,z,0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

void Camera::setViewport(int w, int h) {
    view_w = w;
    view_h = h;
    glViewport(0, 0, (GLsizei) view_w, (GLsizei) view_h);
    Camera::setConfig();
}

void Camera::setFovy(GLfloat fovy) {
    this->fovy = fovy;
    Camera::setConfig();
}


#ifndef CAMERA_H
#define CAMERA_H

#include "GL/gl.h"
#include <QMatrix4x4>
class Camera {
public:
    Camera();
    float centerx, centery, centerz;
    void setAngle(GLfloat angle);
    void setAngleV(GLfloat anglev);
    void setRadius(GLfloat radius);
    void setViewport(int w, int h);
    void setFovy(GLfloat fovy);
    void setCenter(float,float,float);
private:
    float x,y,z;
    GLint view_w, view_h;
    GLfloat angle,anglev,radius,radiusb,fovy;
    void updatePosition();
    float cosine(long double ang);
    float sine(long double ang);
    void setConfig();
    void qgluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
};

#endif // CAMERA_H

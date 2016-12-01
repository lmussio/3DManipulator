#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QWheelEvent>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QThread>
#include "mouse3d.h"

#include <GL/glu.h>
#include <time.h>
#include <iostream>
#include "camera.h"
#include "object.h"
#include <string>
#include <vector>
#include <cmath>
#include <valarray>
#include <math.h>

#define LIGHT_LIMIT 7

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
    Mouse3D *mouse3d;
public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();
    bool default_light;
    bool hasLight();
    void setObject(int id);
    int setObjectGeometry(int id);
    void addObject();
    bool delObject();
    std::vector<Object> getObjects();
    void setObjectAnimation(int id);
    void setObjectRainbow();
    void resetObjectRainbow();
    void setObjectScreenRandom();
    void resetObjectScreenRandom();
    void setObjectFlagWaving();
    void resetObjectFlagWaving();
    void setObjectColorRGB(float r, float g, float b);
    void setObjectScale(float sx,float sy,float sz);
    void setObjectX(float x);
    void setObjectY(float y);
    void setObjectZ(float z);
    void setObjectRotX(float rotx);
    void setObjectRotY(float roty);
    void setObjectRotZ(float rotz);
    void setObjectCam();
    void setObjectTexture(QString image_path);
    void clearObjectTexture();
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
private:
    GLfloat fovy, angle, anglev, obj_angle, radius, fAspect;
    float max_radius;
    GLfloat r, g, b;
    int mouse_xini, mouse_yini;
    int mouse_xclick, mouse_yclick;
    int mouse_x,mouse_y;
    int obj_select;
    GLint view_w, view_h;
    char auto_mode;
    float sens;
    GLint primitive;
    Camera camera;
    std::vector<Object> obj;
    int sens3d;
    GLfloat pivotx, pivoty, pivotz,pivot_angle;
    char pivot_mode;
    int plimit;
    float motion_angle;
    bool light_used[LIGHT_LIMIT];
    void draw();
    void drawPrimitive();
    void init();
    void light();
    void lightController();
    void initMap();
    void drawGrid(int grid_dimension,int grid_size);
    void drawPivot(int size);

private slots:
    void animate();
    void updateCamera();
    void motion(int x, int y, int z);
    void motionR(int rx, int ry, int rz);
    void button(int bnum, int bpress);
public slots:
    void setAngle(int);
    void setAngleV(int);
    void setRadius(int);
signals:
    void angleChanged(int);
    void angleVChanged(int);
    void radiusChanged(int);
    void setList(std::vector<Object>);
    void updateList(std::vector<Object>);
};

#endif // GLWIDGET_H

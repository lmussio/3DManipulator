#ifndef OBJECT_H
#define OBJECT_H

#include <GL/gl.h>
#include <time.h>
#include <QOpenGLTexture>
#include <QString>

#define CUBE 0
#define LIGHT 1
#define SPHERE 2
#define FLAG 3
#define STOP 0
#define ROTATE_LEFT 1
#define ROTATE_RIGHT 2
#define ROTATE_UP 3
#define ROTATE_DOWN 4
#define ROTATE_ALL 5

class Object {
public:
    int menu;
    GLfloat x,y,z,randomx,randomy,randomz;
    GLfloat r, g, b;
    GLfloat size;
    int light_num;
    char lock;
    Object() : Object(0,0,0) {}
    Object(GLfloat,GLfloat,GLfloat);
    Object(GLfloat X,GLfloat Y,GLfloat Z, GLfloat rotx, GLfloat roty, GLfloat rotz);
    void setPrimitive(int op);
    void setColor(int op);
    void setColorRGB(float r, float g, float b);
    void getColorRGB(float &r, float &g, float &b);
    void setAnimation(int op);
    int getAnimation();
    void setRainbow();
    void resetRainbow();
    int getRainbow();
    void setScreenRandom();
    void resetScreenRandom();
    int getScreenRandom();
    void setFlagWaving();
    void resetFlagWaving();
    int getFlagWaving();
    void animate();
    void draw();
    void setScale(GLfloat,GLfloat,GLfloat);
    void getScale(float &sx,float &sy,float &sz);
    void incScale(GLfloat,GLfloat,GLfloat);
    void decScale(GLfloat,GLfloat,GLfloat);
    void setPosX(GLfloat);
    void setPosY(GLfloat);
    void setPosZ(GLfloat);
    void getPos(float &px,float &py,float &pz);
    void setRotX(GLfloat rotx);
    void setRotY(GLfloat roty);
    void setRotZ(GLfloat rotz);
    void getRot(float &rx,float &ry,float &rz);
    int getPrimitive();
    void setTexture(QString image_path);
    void clearTexture();
    QString getTexturePath();
private:
    GLfloat ix,iy,iz,fx,fy,fz;
    int random, rspeed, rlimit;
    char zdir,ydir,xdir;
    GLfloat angle, angle_x, angle_y, angle_z, scale_x, scale_y, scale_z,rotate_x,rotate_y,rotate_z,arotate_x,arotate_y,arotate_z;
    int primitive;
    char animation;
    char rainbow_mode, screen_random;
    //-----------------
    char flag_waving;
    QOpenGLTexture *texture;
    QString texture_path;
    int pos;
    int grid;
    int ustride;
    int uorder;
    int vstride;
    int vorder;

    GLfloat pontos[8][4][3];
    GLfloat pontos2[8][4][3];
    //-----------------
    void flagGen();
    void flagMove();
    int dice(int min, int max);
    //-----------------
};

#endif // OBJECT_H

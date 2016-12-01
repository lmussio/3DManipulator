#include "object.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <ctime>
#include <stdlib.h>
#include <QObject>
#include <QMessageBox>

Object::Object(GLfloat X,GLfloat Y,GLfloat Z) {
    texture = NULL;
    texture_path = "";
    x = X;
    y = Y;
    z = Z;
    randomx=x;
    randomy=y;
    randomz=z;
    rspeed = 2;
    random = 3;
    rlimit = 1300;
    r = (rand() % 100)/100.0;
    g = (rand() % 100)/100.0;
    b = (rand() % 100)/100.0;
    primitive = CUBE;
    animation = STOP;
    angle = 0;
    angle_x = 0;
    angle_y = 1;
    angle_z = 0;
    scale_x = 1;
    scale_y = 1;
    scale_z = 1;
    rotate_x = 0;
    rotate_y = 0;
    rotate_z = 0;
    arotate_x = 0;
    arotate_y = 0;
    arotate_z = 0;
    lock = 0;
    size=75;
    ix=-rlimit;
    iy=-rlimit;
    iz=-rlimit;
    fx=rlimit;
    fy=rlimit;
    fz=rlimit;
    rainbow_mode = 0;
    screen_random = 0;

    flagGen();
    flag_waving=1;

    grid=20;
    ustride=3;
    uorder=4;
    vstride=12;
    vorder=8;
    //----------------------------------
    light_num = -1;
}
Object::Object(GLfloat X,GLfloat Y,GLfloat Z, GLfloat rotx, GLfloat roty, GLfloat rotz):Object(X,Y,Z){
    rotate_x = rotx;
    rotate_y = roty;
    rotate_z = rotz;
}
//-------------------------------------------------------------------

void Object::flagGen(){
    int i,j;
    int x,y;
    y=0;
    for(i=0;i<=7;i++){
        x = 0;
        for(j=0;j<=3;j++){
            pontos[i][j][0]= x;
            pontos[i][j][1]= y;
            pontos[i][j][2]= 0;
            x = x + 10;
        }
        y=y+20;
    }
    y=y-20;
    for(i=0;i<=7;i++){
        x = 0;
        for(j=0;j<=3;j++){
            pontos2[i][j][0]= x;
            pontos2[i][j][1]= y;
            pontos2[i][j][2]= 0;
            x = x + 10;
        }
        y=y+20;
    }
    for(j=0;j<=3;j++){
        pontos[1][j][2]= 10;
    }
    pos=1;
}
void Object::flagMove(){
    pos++;
    int j;
    if(pos>15){
        pos=1;
    }
    int npos = pos % 8;
    for(j=0;j<=3;j++){
        if(pos==1){
            pontos[1][j][2]=pontos2[7][j][2];
            pontos2[7][j][2]=pontos2[6][j][2];
        }
        else if(pos==7){
            pontos2[1][j][2]=20;
            pontos[7][j][2]=pontos[6][j][2];
            pontos2[0][j][2]=pontos[6][j][2];
            pontos[6][j][2]=pontos[5][j][2];
        }
        else if(pos==9){
            pontos2[1][j][2]=pontos2[0][j][2];
            pontos[7][j][2]=pontos[6][j][2];
            pontos2[0][j][2]=pontos[6][j][2];
            pontos[6][j][2]=-10;
        }
        else if(pos>9){
            if(pos==10){
                pontos[6][j][2]=0;
            }
            pontos2[npos][j][2]=pontos2[npos-1][j][2];
            pontos2[npos-1][j][2]=pontos2[npos-2][j][2];
        }
        else {
            pontos[npos][j][2]=pontos[npos-1][j][2];
            pontos[npos-1][j][2]=pontos[npos-2][j][2];
            if(pos==6){
                pontos2[1][j][2]=-10;
            }
        }
    }
    if(pos==7)pos++;
}

int Object::dice(int min, int max)
{
    return rand() % (max + 1 - min) + min;
}

//-------------------------------------------------------------------
void Object::setScale(GLfloat sx,GLfloat sy,GLfloat sz){
    if(lock) return;
    if(sx>0 && sy>0 && sz > 0) {
        scale_x = sx;
        scale_y = sy;
        scale_z = sz;
    }
}

void Object::getScale(float &sx, float &sy, float &sz)
{
    sx = scale_x;
    sy = scale_y;
    sz = scale_z;
}

void Object::incScale(GLfloat sx,GLfloat sy,GLfloat sz){
    if(lock) return;
    scale_x += sx;
    scale_y += sy;
    scale_z += sz;
}

void Object::decScale(GLfloat sx,GLfloat sy,GLfloat sz){
    if(lock) return;
    if(scale_x-sx>0 && scale_x-sy>0 && scale_x-sz > 0) {
        scale_x -= sx;
        scale_y -= sy;
        scale_z -= sz;
    }
}

void Object::setPosX(GLfloat x)
{
    this->x=x;
}

void Object::setPosY(GLfloat y)
{
    this->y=y;
}

void Object::setPosZ(GLfloat z)
{
    this->z=z;
}

void Object::setRotX(GLfloat rotx)
{
    rotate_x=rotx;
}

void Object::setRotY(GLfloat roty)
{
    rotate_y=roty;
}

void Object::setRotZ(GLfloat rotz)
{
    rotate_z=rotz;
}

void Object::getRot(float &rx, float &ry, float &rz)
{
    rx=rotate_x;
    ry=rotate_y;
    rz=rotate_z;
}

void Object::getPos(float &px, float &py, float &pz)
{
    px=x;
    py=y;
    pz=z;
}

int Object::getPrimitive()
{
    return primitive;
}

void Object::setTexture(QString image_path)
{
    texture_path = image_path;
    texture = new QOpenGLTexture(QImage(image_path).mirrored());
}

void Object::clearTexture()
{
    texture_path = "";
    delete texture;
    texture = NULL;
}

QString Object::getTexturePath()
{
    return texture_path;
}

void Object::setPrimitive(int op)
{
    if(lock) return;
    switch(op){
        case 0:
            primitive = CUBE;
            break;
        case 1:
            primitive = LIGHT;
            break;
        case 2:
            primitive = SPHERE;
            break;
        case 3:
            primitive = FLAG;
            break;
    }
}
void Object::setColor(int op)
{
    if(lock) return;
    switch(op){
        case 0:
            r = 1.0;
            g = 0.0;
            b = 0.0;
            break;
        case 1:
            r = 0.0;
            g = 1.0;
            b = 0.0;
            break;
        case 2:
            r = 0.0;
            g = 0.0;
            b = 1.0;
            break;
        case 3:
            r = (rand() % 100)/100.0;
            g = (rand() % 100)/100.0;
            b = (rand() % 100)/100.0;
            break;
    }
}

void Object::setColorRGB(float r, float g, float b)
{
    if(r>255) this->r = 1.0;
    else if(r<0) this->r = 0;
    else this->r = r;
    if(g>255) this->g = 1.0;
    else if(g<0) this->g = 0;
    else this->g = g;
    if(b>255) this->b = 1.0;
    else if(b<0) this->b = 0;
    else this->b = b;
}

void Object::getColorRGB(float &r, float &g, float &b)
{
    r=this->r;
    g=this->g;
    b=this->b;
}

void Object::setAnimation(int op){
    if(lock) return;
    switch(op){
        case 0:
            angle_x = 0;
            angle_y = 1;
            angle_z = 0;
            arotate_x = rotate_x;
            arotate_y = rotate_y;
            arotate_z = rotate_z;
            animation = STOP;
            break;
        case 1:
            angle_x = 0;
            angle_y = 1;
            angle_z = 0;
            animation = ROTATE_LEFT;
            break;
        case 2:
            angle_x = 0;
            angle_y = 1;
            angle_z = 0;
            animation = ROTATE_RIGHT;
            break;
        case 3:
            angle_x = 1;
            angle_y = 0;
            angle_z = 0;
            animation = ROTATE_UP;
            break;
        case 4:
            angle_x = 1;
            angle_y = 0;
            angle_z = 0;
            animation = ROTATE_DOWN;
            break;
        case 5:
            angle_x = 1;
            angle_y = 1;
            angle_z = 1;
            animation = ROTATE_ALL;
            break;
        case 6:
            if(rainbow_mode == 0) rainbow_mode = 1;
            else rainbow_mode = 0;
            break;
        case 7:
            if(screen_random == 0) {
                zdir=dice(-random,random);
                ydir=dice(-random,random);
                xdir=dice(-random,random);
                screen_random = 1;
            }
            else screen_random = 0;
            break;
        case 8:
            if(primitive==FLAG) {
                if(flag_waving == 0) flag_waving = 1;
                else flag_waving = 0;
            }
            break;
    }
}

void Object::setRainbow()
{
    rainbow_mode=1;
}

void Object::resetRainbow()
{
    rainbow_mode=0;
}

int Object::getRainbow()
{
    return rainbow_mode;
}

void Object::setScreenRandom()
{
    zdir=dice(-random,random);
    ydir=dice(-random,random);
    xdir=dice(-random,random);
    screen_random=1;
}

void Object::resetScreenRandom()
{
    screen_random=0;
    randomx=x;
    randomy=y;
    randomz=z;
}

int Object::getScreenRandom()
{
    return screen_random;
}

void Object::setFlagWaving()
{
    if(primitive==FLAG) flag_waving = 1;
}

void Object::resetFlagWaving()
{
    if(primitive==FLAG) flag_waving = 0;
}

int Object::getFlagWaving()
{
    return flag_waving;
}

int Object::getAnimation()
{
    return animation;
}

void Object::animate(){
    if(lock) return;
    switch(animation){
        case STOP:
            break;
        case ROTATE_LEFT:
            arotate_y += 2;
            if (arotate_y < 0) arotate_y = arotate_y + 360;
            break;
        case ROTATE_RIGHT:
            arotate_y -= 2;
            if (arotate_y >= 360) arotate_y = arotate_y - 360;
            break;
        case ROTATE_UP:
            arotate_x -= 2;
            if (arotate_x < 0) arotate_x = arotate_x + 360;
            break;
        case ROTATE_DOWN:
            arotate_x += 2;
            if (arotate_x >= 360) arotate_x = arotate_x - 360;
            break;
        case ROTATE_ALL:
            arotate_x -= 2;
            if (arotate_x < 0) arotate_x = arotate_x + 360;
            arotate_y -= 2;
            if (arotate_y < 0) arotate_y = arotate_y + 360;
            arotate_x -= 2;
            if (arotate_z < 0) arotate_z = arotate_z + 360;
            break;
    }
//    if(rainbow_mode){
//        r = (rand() % 100)/100.0;
//        g = (rand() % 100)/100.0;
//        b = (rand() % 100)/100.0;
//    }
    if(screen_random){

        if(randomx<=ix) {
            xdir=dice(0,random)+rspeed;
        }
        if(randomy<=iy){
            ydir=dice(0,random)+rspeed;
        }
        if(randomz<=iz) {
            zdir=dice(0,random)+rspeed;
        }
        if(randomx>=fx) {
            xdir=dice(0,-random)-rspeed;
        }
        if(randomy>=fy) {
            ydir=dice(0,-random)-rspeed;
        }
        if(randomz>=fz) {
            zdir=dice(0,-random)-rspeed;
        }
        randomx += xdir;
        randomy += ydir;
        randomz += zdir;
    }
    if(flag_waving){
        flagMove();
    }
}


void Object::draw() {
    glPushMatrix();
    if(texture==NULL){
        if(rainbow_mode){
            glColor3f((rand() % 100)/100.0, (rand() % 100)/100.0, (rand() % 100)/100.0);
        }else glColor3f(r, g, b);
    }
    else{
        glColor3f(1, 1, 1);
    }
    if(primitive==LIGHT){
        glPushMatrix();
        GLfloat light_pos[] = {x,y,z,1.0};
        if(screen_random==1){
            light_pos[0] = randomx;
            light_pos[1] = randomy;
            light_pos[2] = randomz;
        }
        switch(light_num){
        case 0:
            glLightfv(GL_LIGHT1, GL_POSITION, light_pos);
            break;
        case 1:
            glLightfv(GL_LIGHT2, GL_POSITION, light_pos);
            break;
        case 2:
            glLightfv(GL_LIGHT3, GL_POSITION, light_pos);
            break;
        case 3:
            glLightfv(GL_LIGHT4, GL_POSITION, light_pos);
            break;
        case 4:
            glLightfv(GL_LIGHT5, GL_POSITION, light_pos);
            break;
        case 5:
            glLightfv(GL_LIGHT6, GL_POSITION, light_pos);
            break;
        case 6:
            glLightfv(GL_LIGHT7, GL_POSITION, light_pos);
            break;
        }
        glPopMatrix();
    }else{
//        GLfloat especularidade[4]={0.5,0.5,0.5,1};
//        glMaterialfv(GL_FRONT,GL_SPECULAR,especularidade);
//        glMaterialfv(GL_FRONT,GL_SHININESS, especularidade);
    }
    if(screen_random==0) glTranslatef(x, y, z);
    else glTranslatef(randomx, randomy, randomz);
//    glRotatef(angle, angle_x, angle_y, angle_z);
    if(animation==STOP){
        glRotatef(rotate_x, 1, 0, 0);
        glRotatef(rotate_y, 0, 1, 0);
        glRotatef(rotate_z, 0, 0, 1);
    }
    else{
        glRotatef(arotate_x, 1, 0, 0);
        glRotatef(arotate_y, 0, 1, 0);
        glRotatef(arotate_z, 0, 0, 1);
    }
    glTranslatef(0, 0, 0);
    glScalef(scale_x, scale_y, scale_z);
    if(primitive!=LIGHT){
        if(texture!=NULL){
            glEnable(GL_TEXTURE_2D);
            glDepthMask(GL_FALSE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BGRA);
            glBindTexture(GL_TEXTURE_2D, texture->textureId());
        }
    }
    switch(primitive){
        case CUBE:
        {
            glBegin(GL_POLYGON);
            glNormal3f(0.0f, 1.0f, 0.0f); //top face Normal

            glTexCoord2f(1.0f, 0.0f);glVertex3f(size/2, size/2, size/2); // top face
            glTexCoord2f(1.0f, 1.0f);glVertex3f(size/2, size/2, -size/2);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(-size/2, size/2, -size/2);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(-size/2, size/2, size/2);
            glEnd();

            glBegin(GL_POLYGON);
            glNormal3f(0.0f, 0.0f, 1.0f); // N front face

            glTexCoord2f(1.0f, 0.0f);glVertex3f(size/2, size/2, size/2); // front face
            glTexCoord2f(1.0f, 1.0f);glVertex3f(-size/2, size/2, size/2);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(-size/2, -size/2, size/2);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(size/2, -size/2, size/2);
            glEnd();

            glBegin(GL_POLYGON);
            glNormal3f(1.0f, 0.0f, 0.0f); // N right face

            glTexCoord2f(1.0f, 0.0f);glVertex3f(size/2, size/2, size/2); // right face
            glTexCoord2f(1.0f, 1.0f);glVertex3f(size/2, -size/2, size/2);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(size/2, -size/2, -size/2);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(size/2, size/2, -size/2);
            glEnd();

            glBegin(GL_POLYGON);
            glNormal3f(-1.0f, 0.0f, 0.0f); // N left face

            glTexCoord2f(1.0f, 0.0f);glVertex3f(-size/2, size/2, size/2); // left face
            glTexCoord2f(1.0f, 1.0f);glVertex3f(-size/2, size/2, -size/2);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(-size/2, -size/2, -size/2);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(-size/2, -size/2, size/2);
            glEnd();

            glBegin(GL_POLYGON);
            glNormal3f(0.0f, -1.0f, 0.0f); // N bottom face

            glTexCoord2f(1.0f, 0.0f);glVertex3f(size/2, -size/2, size/2); // bottom face
            glTexCoord2f(1.0f, 1.0f);glVertex3f(size/2, -size/2, -size/2);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(-size/2, -size/2, -size/2);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(-size/2, -size/2, size/2);
            glEnd();

            glBegin(GL_POLYGON);
            glNormal3f(0.0f, 0.0f, -1.0f); // N back face

            glTexCoord2f(1.0f, 0.0f);glVertex3f(size/2, size/2, -size/2); // back face
            glTexCoord2f(1.0f, 1.0f);glVertex3f(-size/2, size/2, -size/2);
            glTexCoord2f(0.0f, 1.0f);glVertex3f(-size/2, -size/2, -size/2);
            glTexCoord2f(0.0f, 0.0f);glVertex3f(size/2, -size/2, -size/2);
            glEnd();

            glFlush();
            break;
        }
        case LIGHT:
        {
            GLfloat light_on[] = {0.9, 0.9, 0.9, 1.0};
            GLfloat light_off[] = {0.0, 0.0, 0.0, 1.0};
            GLUquadricObj *quadric;
            quadric = gluNewQuadric();
            gluQuadricDrawStyle(quadric, GLU_FILL);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_on);
            gluSphere(quadric,5,30,30);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, light_off);
//            glPushMatrix();
//            glTranslatef(x,y,z);
//            glRotatef(angle,0,1,0);
//            GLfloat lightpos[4] = {x,y,z,1};
//            glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
//            glPopMatrix();
            break;
        }
        case SPHERE:
            GLUquadricObj *quadric;
            quadric = gluNewQuadric();
            gluQuadricDrawStyle(quadric, GLU_FILL);
            gluQuadricTexture(quadric, GL_TRUE);
            gluQuadricNormals(quadric, GLU_SMOOTH);
            gluSphere(quadric,size/2,30,30);
            break;
        case FLAG:
            GLfloat texpts1[2][2][2] = {{{0.0, 0.0}, {0.0, -1}},{{0.5, 0.0}, {0.5, -1}}};
            glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts1[0][0][0]);
            glMap2f(GL_MAP2_VERTEX_3,0,0.25,ustride,uorder,0,1,vstride,vorder,&pontos[0][0][0]);
            glPushMatrix();
            glRotatef(angle, angle_x, angle_y, angle_z);
            glEvalMesh2(GL_FILL, 0, grid, 0, grid);
            glPopMatrix();
            GLfloat texpts2[2][2][2] = {{{0.5, 0.0}, {0.5, -1}},{{1, 0.0}, {1, -1}}};
            glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts2[0][0][0]);
            glEnable(GL_MAP2_TEXTURE_COORD_2);
            glMap2f(GL_MAP2_VERTEX_3,0,0.25,ustride,uorder,0,1,vstride,vorder,&pontos2[0][0][0]);
            glPushMatrix();
            glRotatef(angle, angle_x, angle_y, angle_z);
            glEvalMesh2(GL_FILL, 0, grid, 0, grid);
            glPopMatrix();
            break;
    }
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glPopMatrix();
}


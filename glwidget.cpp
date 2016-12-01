#include "glwidget.h"

#include <QtWidgets>
#include <QtOpenGL>
#include <QMessageBox>
#include <QString>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    setFormat(format);
}

GLWidget::~GLWidget()
{
    mouse3d->stop();
    mouse3d->wait();
}

bool GLWidget::hasLight()
{
    for(int i=0;i<=LIGHT_LIMIT-1;i++){
        if(light_used[i]==true) return true;
    }
    return false;
}

void GLWidget::setObject(int id)
{
    obj_select = id;
}

int GLWidget::setObjectGeometry(int id)
{
    if(id==LIGHT) {
        int i;
        for(i=0;i<=LIGHT_LIMIT-1;i++){
            if(!light_used[i]){
                light_used[i]=true;
                break;
            }
        }
        if(i>LIGHT_LIMIT-1) return -1;
        obj[obj_select].light_num = i;
    }
    if(obj[obj_select].getPrimitive()==LIGHT) light_used[obj[obj_select].light_num]=false;
    obj[obj_select].setPrimitive(id);
    emit setList(obj);
    return 0;
}

void GLWidget::addObject()
{
    if(pivot_mode==0) {
        obj.push_back(Object(0,0,0));
    }else{
        obj.push_back(Object(pivotx, pivoty, pivotz,0,pivot_angle,0));
    }
    emit setList(obj);
}

bool GLWidget::delObject()
{
    if(obj_select >= 0){
        QMessageBox msgBox;
        msgBox.setText("Delete?");
        msgBox.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Yes){
            if(obj[obj_select].getPrimitive()==LIGHT) light_used[obj[obj_select].light_num]=false;
            obj.erase(obj.begin()+obj_select);
            obj_select = -1;
            return true;
        }
    }
    return false;
}

std::vector<Object> GLWidget::getObjects()
{
    return obj;
}

void GLWidget::setObjectAnimation(int id)
{
    if(obj.size()>0){
        obj[obj_select].setAnimation(id);
        emit setList(obj);
    }
}

void GLWidget::setObjectRainbow()
{
    obj[obj_select].setRainbow();
    emit setList(obj);
}

void GLWidget::resetObjectRainbow()
{
    obj[obj_select].resetRainbow();
    emit setList(obj);
}

void GLWidget::setObjectScreenRandom()
{
    obj[obj_select].setScreenRandom();
    emit setList(obj);
}

void GLWidget::resetObjectScreenRandom()
{
    obj[obj_select].resetScreenRandom();
    emit setList(obj);
}

void GLWidget::setObjectFlagWaving()
{
    obj[obj_select].setFlagWaving();
    emit setList(obj);
}

void GLWidget::resetObjectFlagWaving()
{
    obj[obj_select].resetFlagWaving();
    emit setList(obj);
}

void GLWidget::setObjectColorRGB(float r, float g, float b)
{
    obj[obj_select].setColorRGB(r,g,b);
    emit setList(obj);
}

void GLWidget::setObjectScale(float sx, float sy, float sz)
{
    obj[obj_select].setScale(sx,sy,sz);
    emit setList(obj);
}

void GLWidget::setObjectX(float x)
{
    obj[obj_select].setPosX(x);
    emit setList(obj);
}

void GLWidget::setObjectY(float y)
{
    obj[obj_select].setPosY(y);
    emit setList(obj);
}

void GLWidget::setObjectZ(float z)
{
    obj[obj_select].setPosZ(z);
    emit setList(obj);
}

void GLWidget::setObjectRotX(float rotx)
{
    obj[obj_select].setRotX(rotx);
    emit setList(obj);
}

void GLWidget::setObjectRotY(float roty)
{
    obj[obj_select].setRotY(roty);
    emit setList(obj);
}

void GLWidget::setObjectRotZ(float rotz)
{
    obj[obj_select].setRotZ(rotz);
    emit setList(obj);
}

void GLWidget::setObjectCam()
{
    if(obj.size()>0){
        float px,py,pz;
        obj[obj_select].getPos(px,py,pz);
        camera.setCenter(px,py,pz);
    }
}

void GLWidget::setObjectTexture(QString image_path)
{
    obj[obj_select].setTexture(image_path);
    emit setList(obj);
}

void GLWidget::clearObjectTexture()
{
    obj[obj_select].clearTexture();
    emit setList(obj);
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    init();
    initMap();
    light();

    mouse3d = new Mouse3D();
    connect(mouse3d, SIGNAL(finished()), mouse3d, SLOT(deleteLater()));
    connect(mouse3d, SIGNAL(motion(int,int,int)), this, SLOT(motion(int,int,int)));
    connect(mouse3d, SIGNAL(motionR(int,int,int)), this, SLOT(motionR(int,int,int)));
    connect(mouse3d, SIGNAL(button(int,int)), this, SLOT(button(int,int)));
    mouse3d->start();
}

void GLWidget::paintGL()
{
//    draw();
    lightController();
    drawPrimitive();
    animate();
    updateCamera();
}

void GLWidget::resizeGL(int width, int height)
{
    view_w = width;
    view_h = height;
    camera.setViewport(width,height);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
//    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->x() > mouse_xini) angle += 10*sens;
    if(event->x() < mouse_xini) angle -= 10*sens;
    if(event->y()>mouse_yini) {
        if (anglev < 90) anglev += 10*sens;
    }
    if(event->y()<mouse_yini) {
        if (anglev > -90) anglev -= 10*sens;
    }
    mouse_xini = event->x();
    mouse_yini = event->y();

    if (angle <= 0) angle = angle + 360;
    else if (angle >= 360) angle = angle - 360;
    if (anglev <= -90) anglev = -89;
    else if (anglev >= 90) anglev = 89;
    update();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;
    if (numSteps==1) {
        radius -= 30;
        if (radius < 10) radius = 10;
        camera.setRadius(radius);
        emit radiusChanged(radius);
    } else {
        radius += 30;
        if (radius > 4000) radius = 4000;
        camera.setRadius(radius);
        emit radiusChanged(radius);
    }
    event->accept();
}

void GLWidget::draw()
{
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        glVertex3f(-1,-1,0);
        glVertex3f(-1,1,0);
        glVertex3f(1,1,0);
        glVertex3f(1,-1,0);

    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,-1,0.707);
        glVertex3f(-1,-1,0);
        glVertex3f(1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(1,0, 0.707);
        glVertex3f(1,-1,0);
        glVertex3f(1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(0,1,0.707);
        glVertex3f(1,1,0);
        glVertex3f(-1,1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glBegin(GL_TRIANGLES);
        glNormal3f(-1,0,0.707);
        glVertex3f(-1,1,0);
        glVertex3f(-1,-1,0);
        glVertex3f(0,0,1.2);
    glEnd();
    glPopMatrix();
}

void GLWidget::drawGrid(int grid_dimension,int grid_size)
{
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 0.0f);
    for(int i=-grid_dimension/2;i<=grid_dimension/2;i++)
    {
        glVertex3f((float)i*grid_size,0,(float)-(grid_dimension/2)*grid_size);
        glVertex3f((float)i*grid_size,0,(float)(grid_dimension/2)*grid_size);
        glVertex3f((float)-(grid_dimension/2)*grid_size,0,(float)i*grid_size);
        glVertex3f((float)(grid_dimension/2)*grid_size,0,(float)i*grid_size);
    }
    glEnd();
    glPopMatrix();
}

void GLWidget::drawPivot(int size){
    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0);
    glTranslatef(pivotx, pivoty, pivotz);
    glRotatef(pivot_angle, 0, 1, 0);
    glTranslatef(0, 0, 0);

    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 1.0f, 0.0f); //top face Normal

    glVertex3f(size/2, size/2, size/2); // top face
    glVertex3f(size/2, size/2, -size/2);
    glVertex3f(-size/2, size/2, -size/2);
    glVertex3f(-size/2, size/2, size/2);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, 1.0f); // N front face

    glVertex3f(size/2, size/2, size/2); // front face
    glVertex3f(-size/2, size/2, size/2);
    glVertex3f(-size/2, -size/2, size/2);
    glVertex3f(size/2, -size/2, size/2);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(1.0f, 0.0f, 0.0f); // N right face

    glVertex3f(size/2, size/2, size/2); // right face
    glVertex3f(size/2, -size/2, size/2);
    glVertex3f(size/2, -size/2, -size/2);
    glVertex3f(size/2, size/2, -size/2);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(-1.0f, 0.0f, 0.0f); // N left face

    glVertex3f(-size/2, size/2, size/2); // left face
    glVertex3f(-size/2, size/2, -size/2);
    glVertex3f(-size/2, -size/2, -size/2);
    glVertex3f(-size/2, -size/2, size/2);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0f, -1.0f, 0.0f); // N bottom face

    glVertex3f(size/2, -size/2, size/2); // bottom face
    glVertex3f(size/2, -size/2, -size/2);
    glVertex3f(-size/2, -size/2, -size/2);
    glVertex3f(-size/2, -size/2, size/2);
    glEnd();

    glBegin(GL_POLYGON);
    glNormal3f(0.0f, 0.0f, -1.0f); // N back face

    glVertex3f(size/2, size/2, -size/2); // back face
    glVertex3f(-size/2, size/2, -size/2);
    glVertex3f(-size/2, -size/2, -size/2);
    glVertex3f(size/2, -size/2, -size/2);
    glEnd();

//    glFlush();
    glPopMatrix();
}

void GLWidget::motion(int tx, int ty, int tz)
{
        if(pivot_mode==0) {
            if (ty < -sens3d) {
                radius -= 10;
                if (radius < 10) radius = 10;
                camera.setRadius(radius);
                emit radiusChanged(radius);
            } else if (ty > sens3d) {
                radius += 10;
                if (radius > max_radius) radius = max_radius;
                camera.setRadius(radius);
                emit radiusChanged(radius);
            }
        }else{
            float pivotx_plus=0;
            float pivotz_plus=0;

            if(tz<0){
                motion_angle=(360+atan2(tz,tx) * 180 / M_PI)-pivot_angle;
            }else motion_angle=(atan2(tz,tx) * 180 / M_PI)-pivot_angle;

            if(tz>0){
                if(pivotx>-plimit){
                    pivotx -= tz/50;
                }
                if(pivotx<=-plimit) pivotx= -plimit;
            }else if(tz<0) {
                if(pivotx<plimit){
                    pivotx -= tz/50;
                }
                if(pivotx>=plimit) pivotx= plimit;
            }
            if(tx>0){
                if(pivotz>-plimit){
                    pivotz -= tx/50;
                }
                if(pivotz<=-plimit) pivotz= -plimit;
            }else if(tx<0) {
                if(pivotz<plimit){
                    pivotz -= tx/50;
                }
                if(pivotz>=plimit) pivotz= plimit;
            }



            if (ty < -sens3d) {
                pivoty -= 4;
                if(pivoty<=-plimit) pivoty = -plimit;
            } else if (ty > sens3d) {
                pivoty += 4;
                if(pivoty>=plimit) pivoty = plimit;
            }
        }
        update();
}

void GLWidget::motionR(int rx, int ry, int rz)
{
        if(pivot_mode==0) {
            if (ry < -sens3d) {
                angle -= 1;
            } else if (ry > sens3d) {
                angle += 1;
            }
            if (rx < -sens3d) {
                anglev -= 1;
            } else if (rx > sens3d) {
                anglev += 1;
            }
            if (angle <= 0) angle = angle + 360;
            else if (angle >= 360) angle = angle - 360;
            if (anglev <= -90) anglev = -89;
            else if (anglev >= 90) anglev = 89;
        }else{
            if (ry < -sens3d) {
                pivot_angle -= 1;
            } else if (ry > sens3d) {
                pivot_angle += 1;
            }
            if (pivot_angle < 0) pivot_angle = pivot_angle + 360;
            else if (pivot_angle > 360) pivot_angle = pivot_angle - 360;

            if (anglev < -89) anglev = -89;
            else if (anglev > 89) anglev = 89;
        }
        update();
}

void GLWidget::button(int bnum, int bpress)
{
//    QString str="Number: " + QString::number(bnum) + "\nPressed: " + QString::number(bpress);
//    QMessageBox msgBox;
//    msgBox.setText("Button");
//    msgBox.setInformativeText(str);
//    msgBox.setStandardButtons(QMessageBox::Ok);
//    msgBox.setDefaultButton(QMessageBox::Ok);
//    msgBox.exec();
    if(bpress==0&&bnum==BTN_FIT){
        if(pivot_mode==0) pivot_mode = 1;
        else pivot_mode = 0;
    }
    if(pivot_mode==0) return;
    if(bpress==0){
        switch (bnum) {
        case BTN_MENU:
            addObject();
            break;
        case BTN_1:
            if(obj.size()>0&&obj[obj_select].getAnimation()==STOP) setObjectAnimation(ROTATE_LEFT);
            else setObjectAnimation(STOP);
            break;
        case BTN_2:
            if(obj.size()>0&&obj[obj_select].getAnimation()==STOP) setObjectAnimation(ROTATE_RIGHT);
            else setObjectAnimation(STOP);
            break;
        case BTN_3:
            if(obj.size()>0&&obj[obj_select].getAnimation()==STOP) setObjectAnimation(ROTATE_UP);
            else setObjectAnimation(STOP);
            break;
        case BTN_4:
            if(obj.size()>0&&obj[obj_select].getAnimation()==STOP) setObjectAnimation(ROTATE_DOWN);
            else setObjectAnimation(STOP);
            break;
        case BTN_5:
            if(obj.size()>0&&obj[obj_select].getAnimation()==STOP) setObjectAnimation(ROTATE_ALL);
            else setObjectAnimation(STOP);
            break;
        case BTN_ESC:
            delObject();
            emit setList(obj);
            break;
        case BTN_R:
            pivotx=0;
            pivoty=0;
            pivotz=0;
            break;
        }
    }
    update();
}

void GLWidget::setAngle(int a)
{
    angle = a;
}

void GLWidget::setAngleV(int av)
{
    anglev = av;
}

void GLWidget::setRadius(int z)
{
    camera.setRadius(z);
    radius=z;
}

void GLWidget::drawPrimitive()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawGrid(50,50);
    if(pivot_mode!=0) drawPivot(10);

    std::vector<Object>::iterator it;
    for(it=obj.begin(); it!=obj.end(); ++it){
        it->draw();
    }
    update();
}

void GLWidget::init()
{
    primitive = CUBE;
    camera = Camera();
    fovy=1.0;
    radius=1000;
    max_radius=4000;
    angle=45.0;
    anglev=30;
    obj_select = -1;
    sens = 0.2;
    sens3d = 100;
    pivotx=0;
    pivoty=0;
    pivotz=0;
    pivot_angle=0;
    pivot_mode=0;
    plimit=1300;
    mouse_xini = 1;
    mouse_yini = 1;
    motion_angle=0;
    for(int i=0;i<=7;i++) light_used[i]=false;
    default_light=false;
    camera.setAngle(angle);
    camera.setAngleV(anglev);
    camera.setRadius(radius);
    camera.setFovy(fovy);
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
}

void GLWidget::initMap(){
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MAP2_VERTEX_3);
    glEnable(GL_AUTO_NORMAL);
    glMapGrid2f(20,0.0,1.0,20,0.0,1.0);
}

void GLWidget::animate()
{
    std::vector<Object>::iterator it;
    for(it=obj.begin(); it!=obj.end(); ++it){
        it->animate();
    }
}

void GLWidget::updateCamera()
{
    camera.setAngle(angle);
    camera.setAngleV(anglev);
    emit angleChanged(angle);
    emit angleVChanged(anglev);
}

void GLWidget::light()
{
    GLfloat ambient[4]={0.4,0.4,0.4,1.0};
    GLfloat diffuse[4]={0.7,0.7,0.7,0.0};
    GLfloat specular[4]={1.0, 1.0, 1.0, 0.0};
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
//    glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER,ambient);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
//    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular );
//    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse );
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular );
//    glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse );
    glLightfv(GL_LIGHT2, GL_SPECULAR, specular );
//    glLightfv(GL_LIGHT3, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse );
    glLightfv(GL_LIGHT3, GL_SPECULAR, specular );
//    glLightfv(GL_LIGHT4, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuse );
    glLightfv(GL_LIGHT4, GL_SPECULAR, specular );
//    glLightfv(GL_LIGHT5, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuse );
    glLightfv(GL_LIGHT5, GL_SPECULAR, specular );
//    glLightfv(GL_LIGHT6, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT6, GL_DIFFUSE, diffuse );
    glLightfv(GL_LIGHT6, GL_SPECULAR, specular );
//    glLightfv(GL_LIGHT7, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT7, GL_DIFFUSE, diffuse );
    glLightfv(GL_LIGHT7, GL_SPECULAR, specular );
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::lightController()
{
    glPushMatrix();
    light();
    GLfloat ambient[4]={0.2,0.2,0.2,1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient);

    int i;
    if(default_light) glEnable(GL_LIGHT0);
    else glDisable(GL_LIGHT0);
    for(i=0;i<=LIGHT_LIMIT-1;i++){
        switch(i){
        case 0:
            if(light_used[i]) {
                glEnable(GL_LIGHT1);
                glDisable(GL_LIGHT0);
            }
            else glDisable(GL_LIGHT1);
            break;
        case 1:
            if(light_used[i]) {
                glEnable(GL_LIGHT2);
                glDisable(GL_LIGHT0);
            }
            else glDisable(GL_LIGHT2);
            break;
        case 2:
            if(light_used[i]) {
                glEnable(GL_LIGHT3);
                glDisable(GL_LIGHT0);
            }
            else glDisable(GL_LIGHT3);
            break;
        case 3:
            if(light_used[i]) {
                glEnable(GL_LIGHT4);
                glDisable(GL_LIGHT0);
            }
            else glDisable(GL_LIGHT4);
            break;
        case 4:
            if(light_used[i]) {
                glEnable(GL_LIGHT5);
                glDisable(GL_LIGHT0);
            }
            else glDisable(GL_LIGHT5);
            break;
        case 5:
            if(light_used[i]) {
                glEnable(GL_LIGHT6);
                glDisable(GL_LIGHT0);
            }
            else glDisable(GL_LIGHT6);
            break;
        case 6:
            if(light_used[i]) {
                glEnable(GL_LIGHT7);
                glDisable(GL_LIGHT0);
            }
            else glDisable(GL_LIGHT7);
            break;
        }
    }
    glPopMatrix();
    update();
}

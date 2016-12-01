#ifndef MOUSE3D_H
#define MOUSE3D_H

#include <QObject>
#include "spnav.h"
#include <QThread>

#define BTN_1 12
#define BTN_2 13
#define BTN_3 14
#define BTN_4 15
#define BTN_5 16
#define BTN_6 17
#define BTN_7 18
#define BTN_8 19
#define BTN_9 20
#define BTN_10 21
#define BTN_MENU 0
#define BTN_ALT 23
#define BTN_CTRL 25
#define BTN_ESC 22
#define BTN_SHIFT 24
#define BTN_MINUS 30
#define BTN_PLUS 29
#define BTN_DOMINANT 28
#define BTN_PAN_ZOOM 27
#define BTN_ROTATION 26
#define BTN_T 2
#define BTN_B 6
#define BTN_F 5
#define BTN_BK 7
#define BTN_R 4
#define BTN_L 3
#define BTN_ISO1 10
#define BTN_ISO2 11
#define BTN_90D_CLOCKWISE 8
#define BTN_90D_COUTERCLOCKWISE 9
#define BTN_FIT 1

class Mouse3D : public QThread
{
    Q_OBJECT
    void run();
public:
    Mouse3D();
    void stop();
private:
    int motion_x, motion_y, motion_z;
    int motion_rx, motion_ry, motion_rz;
    int bnum, bpress;
    int stop_flag;
signals:
    void motion(int x, int y, int z);
    void motionR(int rx, int ry, int rz);
    void button(int bnum, int bpress);
};

#endif // MOUSE3D_H

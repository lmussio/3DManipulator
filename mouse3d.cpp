#include "mouse3d.h"
#include <stdio.h>
Mouse3D::Mouse3D()
{
    motion_x = 0;
    motion_y = 0;
    motion_z = 0;
    motion_rx = 0;
    motion_ry = 0;
    motion_rz = 0;
    bnum = 0;
    bpress = 0;
    stop_flag = 0;
}

void Mouse3D::stop()
{
    stop_flag = 1;
}

void Mouse3D::run()
{
    spnav_event sev;
    if(spnav_open()==-1) {
            fprintf(stderr, "Failed to connect to the space navigator daemon.\n");
            return;
    }

    while(stop_flag==0){
        if(spnav_poll_event(&sev)){
            if(sev.type == SPNAV_EVENT_MOTION) {
//                if(sev.motion.x != motion_x || sev.motion.y != motion_y || sev.motion.z != motion_z){
                    motion_x=sev.motion.x;
                    motion_y=sev.motion.y;
                    motion_z=sev.motion.z;
                    emit motion(motion_x,motion_y,motion_z);
//                }

//                if(sev.motion.rx != motion_rx || sev.motion.ry != motion_ry || sev.motion.rz != motion_rz){
                    motion_rx=sev.motion.rx;
                    motion_ry=sev.motion.ry;
                    motion_rz=sev.motion.rz;
                    emit motionR(motion_rx,motion_ry,motion_rz);
//                }
            }
            else{	/* SPNAV_EVENT_BUTTON */
                if(sev.button.bnum != bnum || sev.button.press != bpress) {
                    bnum = sev.button.bnum;
                    bpress = sev.button.press;
                    emit button(bnum,bpress);
                }
            }
        }
    }
    spnav_close();
}

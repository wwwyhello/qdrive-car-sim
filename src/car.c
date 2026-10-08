#include "car.h"
#include <string.h>

void car_init(Car *car)
{
    car->sys_state = ST_IDLE;

    car->auto_req.mode = MOT_NONE;
    car->auto_req.speed = 0;

    car->manual_req.mode = MOT_NONE;
    car->manual_req.speed = 0;
    car->manual_active = 0;

    car->target_req.mode = MOT_STOP;
    car->target_req.speed = 0;

    for(int i=0;i<4;i++){
        car->wheel_target[i] = 0;
        car->wheel_actual[i] = 0;
    }
}

void car_cmd_start(Car *car)
{
    if(car->sys_state == ST_EMERGENCY) return;
    car->sys_state = ST_RUNNING;
}

void calc_wheel_target(MotionMode mode, int speed, int out[4])
{
    switch(mode){
    case MOT_FORWARD:
        out[0]=speed; out[1]=speed; out[2]=speed; out[3]=speed;
        break;
    case MOT_BACKWARD:
        out[0]=-speed; out[1]=-speed; out[2]=-speed; out[3]=-speed;
        break;
    case MOT_LEFT:
        out[0]=-speed; out[1]=speed; out[2]=speed; out[3]=-speed;
        break;
    case MOT_RIGHT:
        out[0]=speed; out[1]=-speed; out[2]=-speed; out[3]=speed;
        break;
    case MOT_STOP:
        out[0]=0; out[1]=0; out[2]=0; out[3]=0;
        break;
    default:
        out[0]=0; out[1]=0; out[2]=0; out[3]=0;
        break;
    }
}

void car_update_target(Car *car)
{
    CtrlReq *src = NULL;
    if(car->manual_active && car->manual_req.mode != MOT_NONE){
        src = &car->manual_req;
    }else if(car->auto_req.mode != MOT_NONE){
        src = &car->auto_req;
    }else{
        car->target_req.mode = MOT_STOP;
        car->target_req.speed = 0;
        calc_wheel_target(MOT_STOP,0, car->wheel_target);
        return;
    }

    car->target_req = *src;
    calc_wheel_target(src->mode, src->speed, car->wheel_target);
}

void car_cmd_auto(Car *car, MotionMode m, int sp)
{
    if(car->sys_state != ST_RUNNING) return;
    car->auto_req.mode = m;
    if(m != MOT_STOP){
        if(sp<0) sp=0;
        if(sp>100) sp=100;
        car->auto_req.speed = sp;
    }else{
        car->auto_req.speed = 0;
    }
    car_update_target(car);
}

void car_cmd_manual(Car *car, MotionMode m, int sp)
{
    if(car->sys_state != ST_RUNNING) return;
    car->manual_active = 1;
    car->manual_req.mode = m;
    if(m != MOT_STOP){
        if(sp<0) sp=0;
        if(sp>100) sp=100;
        car->manual_req.speed = sp;
    }else{
        car->manual_req.speed = 0;
    }
    car_update_target(car);
}

void car_cmd_manual_release(Car *car)
{
    car->manual_active = 0;
    car_update_target(car);
}

void car_cmd_step(Car *car)
{
    if(car->sys_state == ST_EMERGENCY) return;
    for(int i=0;i<4;i++){
        int act = car->wheel_actual[i];
        int tgt = car->wheel_target[i];
        int delta = tgt - act;
        if(delta > 20) delta = 20;
        if(delta < -20) delta = -20;
        car->wheel_actual[i] = act + delta;
    }
}

void car_cmd_emergency(Car *car)
{
    car->sys_state = ST_EMERGENCY;
    for(int i=0;i<4;i++){
        car->wheel_actual[i] = 0;
    }
}

void car_cmd_reset(Car *car)
{
    car_init(car);
}

const char* motion2str(MotionMode m)
{
    switch(m){
    case MOT_FORWARD: return "FORWARD";
    case MOT_BACKWARD: return "BACKWARD";
    case MOT_LEFT: return "LEFT";
    case MOT_RIGHT: return "RIGHT";
    case MOT_STOP: return "STOP";
    case MOT_NONE: return "NONE";
    default: return "NONE";
    }
}

MotionMode str2motion(const char *s)
{
    if(strcmp(s,"forward")==0) return MOT_FORWARD;
    if(strcmp(s,"backward")==0) return MOT_BACKWARD;
    if(strcmp(s,"left")==0) return MOT_LEFT;
    if(strcmp(s,"right")==0) return MOT_RIGHT;
    if(strcmp(s,"stop")==0) return MOT_STOP;
    return MOT_NONE;
}

void car_cmd_status(Car *car)
{
    if(car->sys_state == ST_IDLE){
        printf("STATE IDLE\n");
    }else if(car->sys_state == ST_RUNNING){
        printf("STATE RUNNING\n");
    }else if(car->sys_state == ST_EMERGENCY){
        printf("STATE EMERGENCY\n");
    }

    if(car->manual_active && car->manual_req.mode != MOT_NONE){
        printf("CONTROL MANUAL\n");
    }else if(car->auto_req.mode != MOT_NONE){
        printf("CONTROL AUTO\n");
    }else{
        printf("CONTROL NONE\n");
    }

    if(car->auto_req.mode == MOT_NONE){
        printf("AUTO NONE\n");
    }else{
        if(car->auto_req.mode == MOT_STOP){
            printf("AUTO STOP\n");
        }else{
            printf("AUTO %s %d\n", motion2str(car->auto_req.mode), car->auto_req.speed);
        }
    }

    if(car->manual_req.mode == MOT_NONE){
        printf("MANUAL NONE\n");
    }else{
        if(car->manual_req.mode == MOT_STOP){
            printf("MANUAL STOP\n");
        }else{
            printf("MANUAL %s %d\n", motion2str(car->manual_req.mode), car->manual_req.speed);
        }
    }

    if(car->target_req.mode == MOT_NONE){
        printf("TARGET STOP\n");
    }else{
        if(car->target_req.mode == MOT_STOP){
            printf("TARGET STOP\n");
        }else{
            printf("TARGET %s %d\n", motion2str(car->target_req.mode), car->target_req.speed);
        }
    }

    printf("WHEELS %d %d %d %d\n",
        car->wheel_actual[0],
        car->wheel_actual[1],
        car->wheel_actual[2],
        car->wheel_actual[3]);
}

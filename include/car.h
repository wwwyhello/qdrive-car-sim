#ifndef CAR_H
#define CAR_H

#include <stdio.h>

typedef enum {
    ST_IDLE,
    ST_RUNNING,
    ST_EMERGENCY
} SysState;

typedef enum {
    MOT_NONE,
    MOT_FORWARD,
    MOT_BACKWARD,
    MOT_LEFT,
    MOT_RIGHT,
    MOT_STOP
} MotionMode;

typedef struct {
    MotionMode mode;
    int speed;
} CtrlReq;

typedef struct {
    SysState sys_state;
    CtrlReq auto_req;
    CtrlReq manual_req;
    int manual_active;
    CtrlReq target_req;
    int wheel_target[4];
    int wheel_actual[4];
} Car;

void car_init(Car *car);
void car_cmd_start(Car *car);
void car_cmd_auto(Car *car, MotionMode m, int sp);
void car_cmd_manual(Car *car, MotionMode m, int sp);
void car_cmd_manual_release(Car *car);
void car_cmd_step(Car *car);
void car_cmd_emergency(Car *car);
void car_cmd_reset(Car *car);
void car_cmd_status(Car *car);
void calc_wheel_target(MotionMode mode, int speed, int out[4]);
void car_update_target(Car *car);
MotionMode str2motion(const char *s);
const char* motion2str(MotionMode m);

#endif
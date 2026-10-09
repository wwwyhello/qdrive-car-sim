#include "car.h"
#include <string.h>

void car_init(Car *car)
{
    car->sys_state = S_IDLE;

    car->auto_valid = 0;
    car->auto_dir = MOV_NONE;
    car->auto_speed = 0;

    car->manual_valid = 0;
    car->manual_dir = MOV_NONE;
    car->manual_speed = 0;

    for(int i = 0; i < 4; i++){
        car->target_wheel[i] = 0;
        car->real_wheel[i] = 0;
    }
}

void calc_target_wheel(MoveDir dir, int speed, int out[4])
{
    switch(dir){
        case MOV_FORWARD:
            out[0] = speed; out[1] = speed; out[2] = speed; out[3] = speed;
            break;
        case MOV_BACKWARD:
            out[0] = -speed; out[1] = -speed; out[2] = -speed; out[3] = -speed;
            break;
        case MOV_LEFT:
            out[0] = -speed; out[1] = speed; out[2] = speed; out[3] = -speed;
            break;
        case MOV_RIGHT:
            out[0] = speed; out[1] = -speed; out[2] = -speed; out[3] = speed;
            break;
        case MOV_STOP:
        default:
            out[0] = 0; out[1] = 0; out[2] = 0; out[3] = 0;
            break;
    }
}

void car_step(Car *car)
{
    for(int i = 0; i < 4; i++){
        int diff = car->target_wheel[i] - car->real_wheel[i];
        if(diff > 20){
            car->real_wheel[i] += 20;
        }else if(diff < -20){
            car->real_wheel[i] -= 20;
        }else{
            car->real_wheel[i] = car->target_wheel[i];
        }
    }
}

void get_effective_target(const Car *car, MoveDir *out_dir, int *out_speed)
{
    if(car->manual_valid){
        *out_dir = car->manual_dir;
        *out_speed = car->manual_speed;
    }else if(car->auto_valid){
        *out_dir = car->auto_dir;
        *out_speed = car->auto_speed;
    }else{
        *out_dir = MOV_STOP;
        *out_speed = 0;
    }
}

void car_emergency(Car *car)
{
    car->sys_state = S_EMERGENCY;
    for(int i = 0; i < 4; i++){
        car->real_wheel[i] = 0;
    }
}

void car_reset(Car *car)
{
    car->sys_state = S_IDLE;

    car->auto_valid = 0;
    car->manual_valid = 0;

    for(int i = 0; i < 4; i++){
        car->target_wheel[i] = 0;
        car->real_wheel[i] = 0;
    }
}

static const char* mov_to_str(MoveDir d)
{
    switch(d){
        case MOV_FORWARD: return "FORWARD";
        case MOV_BACKWARD: return "BACKWARD";
        case MOV_LEFT: return "LEFT";
        case MOV_RIGHT: return "RIGHT";
        case MOV_STOP: return "STOP";
        default: return "NONE";
    }
}

void car_print_status(const Car *car)
{
    // STATE
    if(car->sys_state == S_IDLE){
        printf("STATE IDLE\n");
    }else if(car->sys_state == S_RUNNING){
        printf("STATE RUNNING\n");
    }else if(car->sys_state == S_EMERGENCY){
        printf("STATE EMERGENCY\n");
    }

    // CONTROL
    if(car->manual_valid){
        printf("CONTROL MANUAL\n");
    }else if(car->auto_valid){
        printf("CONTROL AUTO\n");
    }else{
        printf("CONTROL NONE\n");
    }

    // AUTO
    if(car->auto_valid){
        if(car->auto_dir == MOV_STOP){
            printf("AUTO STOP\n");
        }else{
            printf("AUTO %s %d\n", mov_to_str(car->auto_dir), car->auto_speed);
        }
    }else{
        printf("AUTO NONE\n");
    }

    // MANUAL
    if(car->manual_valid){
        if(car->manual_dir == MOV_STOP){
            printf("MANUAL STOP\n");
        }else{
            printf("MANUAL %s %d\n", mov_to_str(car->manual_dir), car->manual_speed);
        }
    }else{
        printf("MANUAL NONE\n");
    }

    // TARGET
    MoveDir t_dir;
    int t_spd;
    get_effective_target(car, &t_dir, &t_spd);
    printf("TARGET %s\n", mov_to_str(t_dir));

    // WHEELS
    printf("WHEELS %d %d %d %d\n",
        car->real_wheel[0],
        car->real_wheel[1],
        car->real_wheel[2],
        car->real_wheel[3]);
}
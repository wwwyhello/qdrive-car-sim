#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "car.h"

#define BUF_LEN 256

static MoveDir str_to_mov(const char *s)
{
    if(strcmp(s,"forward")==0) return MOV_FORWARD;
    if(strcmp(s,"backward")==0) return MOV_BACKWARD;
    if(strcmp(s,"left")==0) return MOV_LEFT;
    if(strcmp(s,"right")==0) return MOV_RIGHT;
    if(strcmp(s,"stop")==0) return MOV_STOP;
    return MOV_NONE;
}

int main(void)
{
    Car car;
    car_init(&car);

    char buf[BUF_LEN];
    while(fgets(buf, BUF_LEN, stdin) != NULL)
    {
        // 去掉换行符
        buf[strcspn(buf,"\r\n")] = '\0';
        char *tok = strtok(buf, " \t");
        if(tok == NULL) continue;

        if(strcmp(tok,"quit") == 0){
            break;
        }
        else if(strcmp(tok,"start") == 0){
            if(car.sys_state == S_IDLE){
                car.sys_state = S_RUNNING;
            }
        }
        else if(strcmp(tok,"emergency") == 0){
            car_emergency(&car);
        }
        else if(strcmp(tok,"reset") == 0){
            car_reset(&car);
        }
        else if(strcmp(tok,"step") == 0){
            if(car.sys_state == S_RUNNING){
                MoveDir eff_dir;
                int eff_spd;
                get_effective_target(&car, &eff_dir, &eff_spd);
                int tmp[4];
                calc_target_wheel(eff_dir, eff_spd, tmp);
                memcpy(car.target_wheel, tmp, sizeof(tmp));
                car_step(&car);
            }
        }
        else if(strcmp(tok,"status") == 0){
            car_print_status(&car);
        }
        else if(strcmp(tok,"manual") == 0){
            char *p2 = strtok(NULL, " \t");
            if(p2 == NULL) continue;
            if(strcmp(p2,"release") == 0){
                car.manual_valid = 0;
                continue;
            }
            // manual dir [speed]
            MoveDir d = str_to_mov(p2);
            if(d == MOV_NONE) continue;
            if(d == MOV_STOP){
                car.manual_valid = 1;
                car.manual_dir = MOV_STOP;
                car.manual_speed = 0;
            }else{
                char *p3 = strtok(NULL, " \t");
                if(p3 == NULL) continue;
                int spd = atoi(p3);
                if(spd <0 || spd>100) continue;
                car.manual_valid = 1;
                car.manual_dir = d;
                car.manual_speed = spd;
            }
        }
        else if(strcmp(tok,"auto") == 0){
            if(car.sys_state == S_EMERGENCY) continue;
            char *p2 = strtok(NULL, " \t");
            if(p2 == NULL) continue;
            MoveDir d = str_to_mov(p2);
            if(d == MOV_NONE) continue;
            if(d == MOV_STOP){
                car.auto_valid = 1;
                car.auto_dir = MOV_STOP;
                car.auto_speed = 0;
            }else{
                char *p3 = strtok(NULL, " \t");
                if(p3 == NULL) continue;
                int spd = atoi(p3);
                if(spd <0 || spd>100) continue;
                car.auto_valid = 1;
                car.auto_dir = d;
                car.auto_speed = spd;
            }
        }
        // 未知命令直接忽略，不崩溃
    }
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "car.h"

#define LINE_BUF 256

int main(void)
{
    Car car;
    car_init(&car);

    char line[LINE_BUF];
    while(fgets(line, LINE_BUF, stdin) != NULL){
        size_t len = strlen(line);
        if(len>0 && line[len-1]=='\n'){
            line[len-1] = '\0';
        }

        char cmd[64], arg1[64], arg2[64];
        int n = sscanf(line, "%s %s %s", cmd, arg1, arg2);

        if(n <=0) continue;

        if(strcmp(cmd,"quit")==0){
            break;
        }else if(strcmp(cmd,"start")==0){
            car_cmd_start(&car);
        }else if(strcmp(cmd,"auto")==0){
            if(n>=2){
                MotionMode m = str2motion(arg1);
                if(m == MOT_STOP){
                    car_cmd_auto(&car, MOT_STOP, 0);
                }else if(m != MOT_NONE && n>=3){
                    int sp = atoi(arg2);
                    car_cmd_auto(&car, m, sp);
                }
            }
        }else if(strcmp(cmd,"manual")==0){
            if(n>=2 && strcmp(arg1,"release")==0){
                car_cmd_manual_release(&car);
            }else if(n>=2){
                MotionMode m = str2motion(arg1);
                if(m == MOT_STOP){
                    car_cmd_manual(&car, MOT_STOP,0);
                }else if(m != MOT_NONE && n>=3){
                    int sp = atoi(arg2);
                    car_cmd_manual(&car, m, sp);
                }
            }
        }else if(strcmp(cmd,"step")==0){
            car_cmd_step(&car);
        }else if(strcmp(cmd,"emergency")==0){
            car_cmd_emergency(&car);
        }else if(strcmp(cmd,"reset")==0){
            car_cmd_reset(&car);
        }else if(strcmp(cmd,"status")==0){
            car_cmd_status(&car);
        }
    }
    return 0;
}
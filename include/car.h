#ifndef CAR_H
#define CAR_H

#include <stdio.h>

// 系统状态
typedef enum {
    S_IDLE,
    S_RUNNING,
    S_EMERGENCY
} SysState;

// 运动方向
typedef enum {
    MOV_NONE,
    MOV_FORWARD,
    MOV_BACKWARD,
    MOV_LEFT,
    MOV_RIGHT,
    MOV_STOP
} MoveDir;

// 控制来源
typedef enum {
    CTRL_NONE,
    CTRL_AUTO,
    CTRL_MANUAL
} CtrlSrc;

typedef struct {
    SysState sys_state;

    // 自动控制
    int auto_valid;
    MoveDir auto_dir;
    int auto_speed;

    // 手动控制
    int manual_valid;
    MoveDir manual_dir;
    int manual_speed;

    // 四轮：左前轮、右前轮、左后轮、右后轮
    int target_wheel[4];
    int real_wheel[4];
} Car;

// 初始化小车
void car_init(Car *car);

// 根据方向+速度计算四轮目标速度
void calc_target_wheel(MoveDir dir, int speed, int out[4]);

// step：实际速度向目标速度靠近，每轮最多变化±20
void car_step(Car *car);

// 获取当前生效的目标(手动优先)
void get_effective_target(const Car *car, MoveDir *out_dir, int *out_speed);

// 执行emergency急停
void car_emergency(Car *car);

// 执行reset复位
void car_reset(Car *car);

// 打印status，严格题目6行大写格式
void car_print_status(const Car *car);

#endif
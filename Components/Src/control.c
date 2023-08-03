/**
 * @file control.c
 * @author dokee (dokee.39@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"
#include "control.h"
#include "motor.h"

// 原点
dot_degree_t origin = {0.0f, 0.0f};
// 表示边界的矩形
quadrangle_t boundary = {{DEGREE_X_MIN, DEGREE_Y_MAX}, {DEGREE_X_MAX, DEGREE_Y_MAX}, {DEGREE_X_MAX, DEGREE_Y_MIN}, {DEGREE_X_MIN, DEGREE_Y_MIN}};


void Control_WalkTo_Origin(void)
{
    motor_output(origin.x, origin.y);
    HAL_Delay(2000);
}


// TODO 代码方案更新后用走直线函数

void Control_Boundary_Walk(void)
{
    HAL_Delay(100);
    motor_output(boundary.dot1.x, boundary.dot1.y);
    HAL_Delay(1000);
    motor_output(boundary.dot2.x, boundary.dot2.y);
    HAL_Delay(1000);
    motor_output(boundary.dot3.x, boundary.dot3.y);
    HAL_Delay(1000);
    motor_output(boundary.dot4.x, boundary.dot4.y);
    HAL_Delay(1000);
}


/**
 * @brief 电机初始化等，把上电需要做的事情都放在里面
 * 
 */
void Control_Init(void)
{
    motor_enable();
    Control_WalkTo_Origin();
}

/**
 * @brief Control 的定时任务
 * 
 */
void Control_Task(void)
{
    Control_WalkTo_Origin();
    Control_Boundary_Walk();
}







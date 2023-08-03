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
#include "basic.h"

// 表示激光笔到屏幕的距离
float h = 100.0f;
// 原点
dot_t origin;
dot_t dot_h;
// 表示边界的矩形
quadrangle_t boundary;

// TODO 代码方案更新: 走直线函数
// TODO 舵机的抖动如何解决？
// TODO

// 记录电机的角度
float motor_x_degree = 0.0f;
float motor_y_degree = 0.0f;

void Control_WalkTo_Origin(void)
{
    motor_x_degree = origin.x.deg;
    motor_y_degree = origin.y.deg;
    motor_output(motor_x_degree, motor_y_degree);
    HAL_Delay(1500);
}

void Control_Boundary_Walk(void)
{
    HAL_Delay(100);

    motor_x_degree = boundary.dot1.x.deg;
    motor_y_degree = boundary.dot1.y.deg;
    motor_output(motor_x_degree, motor_y_degree);
    HAL_Delay(1000);
    
    motor_x_degree = boundary.dot2.x.deg;
    motor_y_degree = boundary.dot2.y.deg;
    motor_output(motor_x_degree, motor_y_degree);
    HAL_Delay(1000);

    motor_x_degree = boundary.dot3.x.deg;
    motor_y_degree = boundary.dot3.y.deg;
    motor_output(motor_x_degree, motor_y_degree);
    HAL_Delay(1000);

    motor_x_degree = boundary.dot4.x.deg;
    motor_y_degree = boundary.dot4.y.deg;
    motor_output(motor_x_degree, motor_y_degree);
    HAL_Delay(1000);
}

/**
 * @brief 
 * 
 * @param delta_degree_x 
 * @param delta_degree_y 
 */
void Control_Move(float delta_degree_x, float delta_degree_y)
{
    motor_x_degree += delta_degree_x;
    motor_y_degree += delta_degree_y;
    motor_output(motor_x_degree, motor_y_degree);
}


/**
 * @brief 电机初始化等，把上电需要做的事情都放在里面
 * 
 */
void Control_Init(void)
{
    dot_init(&(origin), 0.0f, 0.0f, 0.0f, 0.0f);
    dot_init(&(dot_h), 0.0f, 0.0f, 0.0f, 0.0f);

    dot_init(&(boundary.dot1), -25.0f, 25.0f, DEGREE_X_MIN_THEORY, DEGREE_Y_MAX_THEORY);
    dot_init(&(boundary.dot2), 25.0f, 25.0f, DEGREE_X_MAX_THEORY, DEGREE_Y_MIN_THEORY);
    dot_init(&(boundary.dot3), 25.0f, -25.0f, DEGREE_X_MAX_THEORY, DEGREE_Y_MIN_THEORY);
    dot_init(&(boundary.dot4), -25.0f, -25.0f, DEGREE_X_MIN_THEORY, DEGREE_Y_MIN_THEORY);

    motor_enable();
    Control_WalkTo_Origin();
}

/**
 * @brief Control 的定时任务 (现在在main里)
 * 
 */
void Control_Task(void)
{
    // Control_WalkTo_Origin();
    // Control_Boundary_Walk();
}







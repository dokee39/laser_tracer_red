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

#include <math.h>

#include "main.h"
#include "control.h"
#include "motor.h"
#include "basic.h"
#include "task_process.h"

// 表示激光笔到屏幕的距离
float h = 100.0f;
// 原点
dot_t origin;
// 表示边界的矩形
quadrangle_t boundary;

/* WalkLine 相关参数 BEGIN */
typedef struct
{
    dot_t dot_start;
    dot_t dot_end;

    uint16_t step_cnt;
    uint16_t step_num;

    float step_max;

    float delta_x;
    float delta_y;
} line_t;

#define STEP_MAX_HIGH_SPEED 0.5f
#define STEP_MAX_LOW_SPEED 0.3f

#define LINE_NUM_MAX 10
line_t line_list[LINE_NUM_MAX];
uint8_t line_list_head = 0;
uint8_t line_list_tail = 0;
uint8_t is_walking_line = 0;
/* WalkLine 相关参数 END */

// TODO 代码方案更新: 走直线函数
// TODO 舵机的抖动如何解决？
// TODO

// 记录电机的角度
float motor_x_degree = 0.0f;
float motor_y_degree = 0.0f;

/**
 * @brief // TODO 先手动
 *
 */
// void Control_ResetTo_Origin(void)
// {
//     if (is_walking_line)
//     {
//         is_walking_line = 0;

//         Task_Remove(&task_control_walk_line);

//         line_list_head = 0;
//         line_list_tail = 0;

//         dot_t dot_now;
//         dot_init(&dot_now, )
//         Control_Line_Add()
//     }
// }

/**
 * @brief
 *
 * @param pdot_start_in
 * @param pdot_end_in
 * @param step_max_in
 */
void Control_Line_Add(dot_t *pdot_start, dot_t *pdot_end, float step_max)
{
    line_t *pline = &line_list[line_list_tail];

    pline->dot_start = *pdot_start;
    pline->dot_end = *pdot_end;
    pline->step_max = step_max;

    pline->delta_x = pdot_end->x.pos - pdot_start->x.pos;
    pline->delta_y = pdot_end->y.pos - pdot_start->y.pos;

    if (fabs(pline->delta_x) >= fabs(pline->delta_y))
    {
        pline->step_num = (uint16_t)fabs(pline->delta_x / pline->step_max);
    }
    else
    {
        pline->step_num = (uint16_t)fabs(pline->delta_y / pline->step_max);
    }

    pline->step_cnt = 0;

    line_list_tail++;
    if (line_list_tail >= LINE_NUM_MAX)
        line_list_tail = 0;
}

void Control_WalkLine_Task(void)
{
    line_t *pline_walking = &line_list[line_list_head];

    pline_walking->step_cnt++;
    if (pline_walking->step_cnt < pline_walking->step_num)
    {
        dot_t dot_light;
        dot_init(&dot_light,
                 pline_walking->dot_start.x.pos + ((float)pline_walking->step_cnt / (float)pline_walking->step_num) * pline_walking->delta_x,
                 pline_walking->dot_start.y.pos + ((float)pline_walking->step_cnt / (float)pline_walking->step_num) * pline_walking->delta_y);

        motor_x_degree = dot_light.x.deg;
        motor_y_degree = dot_light.y.deg;
        motor_output(motor_x_degree, motor_y_degree);
    }
    else
    {
        motor_x_degree = line_list[line_list_head].dot_end.x.deg;
        motor_y_degree = line_list[line_list_head].dot_end.y.deg;
        motor_output(motor_x_degree, motor_y_degree);

        Task_Remove(&task_control_walk_line);

        line_list_head++;
        if (line_list_head >= LINE_NUM_MAX)
            line_list_head = 0;

        is_walking_line = 0;
    }
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
    // TODO 初始化的时候要不要指定可能的角度
    origin.x.pos = 0.0f;
    origin.y.pos = 0.0f;

    boundary.dot1.x.pos = -25.0f;
    boundary.dot1.y.pos = 25.0f;
    boundary.dot2.x.pos = 25.0f;
    boundary.dot2.y.pos = 25.0f;
    boundary.dot3.x.pos = 25.0f;
    boundary.dot3.y.pos = -25.0f;
    boundary.dot4.x.pos = -25.0f;
    boundary.dot4.y.pos = -25.0f;

    motor_enable();

    // TODO 从BKP读取

    // TODO Control_WalkTo_Origin();
}

/**
 * @brief
 * @note from origin
 *
 * @param pquadrangle
 */
void Control_WalkQuadrangle(quadrangle_t *pquadrangle)
{
    Control_Line_Add(&origin, &pquadrangle->dot1, STEP_MAX_HIGH_SPEED);
    Control_Line_Add(&pquadrangle->dot1, &pquadrangle->dot2, STEP_MAX_LOW_SPEED);
    Control_Line_Add(&pquadrangle->dot2, &pquadrangle->dot3, STEP_MAX_LOW_SPEED);
    Control_Line_Add(&pquadrangle->dot3, &pquadrangle->dot4, STEP_MAX_LOW_SPEED);
    Control_Line_Add(&pquadrangle->dot4, &pquadrangle->dot1, STEP_MAX_LOW_SPEED);
    Control_Line_Add(&pquadrangle->dot1, &origin, STEP_MAX_HIGH_SPEED);
}

/**
 * @brief Control 的定时任务
 *
 */
void Control_Task(void)
{
    if (is_walking_line == 0 && line_list_head != line_list_tail)
    {
        Task_Add(&task_control_walk_line);
        is_walking_line = 1;
    }
}

/**
 * @file control.h
 * @author dokee (dokee.39@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-08-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "main.h"
#include "basic.h"

/* 校准时定义横向和纵向长度对应电机的角度范围 BEGIN */
// TODO 或许定义一个结构体放在电机里面, 或者其他的方式，毕竟要在校准时更改
// TODO 或许可以保存 BKP 里面？
extern dot_t origin;
extern quadrangle_t boundary;
/* 校准时定义横向和纵向长度对应电机的角度范围 END */

/* 记录电机的角度 BEGIN */ // TODO 需要 extern 吗？
extern float motor_x_degree;
extern float motor_y_degree;
/* 记录电机的角度 END */

extern float h;

/* 函数声明 BEGIN */

void Control_Init(void);
void Control_Move(float delta_degree_x, float delta_degree_y);

void Control_Line_Add(dot_t *pdot_start, dot_t *pdot_end, float step_max);
void Control_WalkLine_Task(void);

void Control_WalkTo_Origin(void);
void Control_ResetTo_Origin(void);
void Control_WalkQuadrangle(quadrangle_t *pquadrangle);

void Control_WaitRectPos_Task(void);

void Control_Task(void);

/* 函数声明 END */

#endif // !_CONTROL_H_

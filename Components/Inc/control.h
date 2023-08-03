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

/* 校准时定义横向和纵向长度对应电机的角度范围 BEGIN */
// TODO 或许定义一个结构体放在电机里面, 或者其他的方式，毕竟要在校准时更改
// TODO 或许可以保存 BKP 里面？
#define DEGREE_X_MIN -10.0f
#define DEGREE_X_MAX 10.0f
#define DEGREE_Y_MIN -10.0f
#define DEGREE_Y_MAX 10.0f

extern quadrangle_t boundary;
/* 校准时定义横向和纵向长度对应电机的角度范围 BEGIN */

/* 函数声明 BEGIN */
void Control_Init(void);
void Control_Task(void);
/* 函数声明 END */

#endif // !_CONTROL_H_

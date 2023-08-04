/**
 * @file task_process.h
 * @author dokee (dokee.39@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-07-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef _TASK_PROCESS_H_
#define _TASK_PROCESS_H_

#include "timesilce_task.h"

// 常驻任务数目
#define FIXED_TASK_NUM 2

// 任务计数周期 (TIM_TASK_INTERVAL 的倍数)
#define TASK_CNT_ADJUST 1u
#define TASK_CNT_CONTROL 2u
#define TASK_CNT_CONTROL_WALK_LINE 1u
#define TASK_CNT_MASTER 1u
#define TASK_CNT_LED 30u
#define TASK_CNT_WAIT_RECT_POS 4u

/* 添加时间片任务结构体并 extern BEGIN */
extern TimesilceTaskObj task_control;
extern TimesilceTaskObj task_adjust;
extern TimesilceTaskObj task_control_walk_line;
extern TimesilceTaskObj task_master;
extern TimesilceTaskObj task_LED;
extern TimesilceTaskObj task_wait_rect_pos;
/* 添加时间片任务结构体并 extern END */

void TaskProcess_Init(void);
void Task_Add(TimesilceTaskObj *obj);
void Task_Remove(TimesilceTaskObj *obj);
unsigned int Task_GetNum(void);
void LED_Task(void);


#endif

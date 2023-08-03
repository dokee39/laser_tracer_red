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

// 任务计数周期 (TIM_TASK_INTERVAL 的倍数)
#define TASK_CNT_ADJUST 1u
#define TASK_CNT_CONTROL 1u
#define TASK_CNT_CONTROL_WALK_LINE 1u

/* 添加时间片任务结构体并 extern BEGIN */
extern TimesilceTaskObj task_control;
extern TimesilceTaskObj task_adjust;
extern TimesilceTaskObj task_control_walk_line;
/* 添加时间片任务结构体并 extern END */

void TaskProcess_Init(void);
void Task_Add(TimesilceTaskObj *obj);
void Task_Remove(TimesilceTaskObj *obj);


#endif

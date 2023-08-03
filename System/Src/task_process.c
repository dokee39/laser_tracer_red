/**
 * @file task_process.c
 * @author dokee (dokee.39@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-07-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.h"
#include "timesilce_task.h"
#include "task_process.h"
#include "tim.h"

#include "control.h"
#include "adjust.h"

/* 添加时间片任务结构体 BEGIN */
// TimesilceTaskObj task_control;
TimesilceTaskObj task_adjust;
/* 添加时间片任务结构体 END */

void TaskProcess_Init(void)
{
    /* 将时间片任务结构体初始化并添加到侵入式链表中 BEGIN */
    // timeslice_task_init(&task_control, Control_Task, 1, TASK_CNT_K210_RECEIVE);
    // timeslice_task_add(&task_control);
    timeslice_task_init(&task_adjust, Adjust_Task, 1, TASK_CNT_ADJUST);
    timeslice_task_add(&task_adjust);
    /* 将时间片任务结构体初始化并添加到侵入式链表中 END */
}

/* 将任务用的定时中断放在这里 BEGIN */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim_task)) // TIM6 的任务处理中断, 每 10ms 进入一次
    {
        timeslice_tick();
        timeslice_exec();
    }
}
/* 将任务用的定时中断放在这里 END */


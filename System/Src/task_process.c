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

/* 添加时间片任务结构体并 extern BEGIN */
TimesilceTaskObj task_adjust;
TimesilceTaskObj task_control;
TimesilceTaskObj task_control_walk_line;
/* 添加时间片任务结构体并 extern END */

/* 任务增删的列表 BEGIN */
// 任务不能在执行任务轮询的时候增删，统一在退出任务轮询后增删
// 一次可以添加 TASK_CHANGE_NUM 个
#define TASK_CHANGE_NUM 3
TimesilceTaskObj *(task_to_add[TASK_CHANGE_NUM]) = {NULL};
TimesilceTaskObj *(task_to_remove[TASK_CHANGE_NUM]) = {NULL};
/* 任务增删的列表 END */

void TaskProcess_Init(void)
{
    /* 将所有时间片任务结构体初始化并添加到侵入式链表中 BEGIN */
    timeslice_task_init(&task_adjust, Adjust_Task, 1, TASK_CNT_ADJUST);
    timeslice_task_init(&task_control, Control_Task, 2, TASK_CNT_CONTROL);
    timeslice_task_init(&task_control_walk_line, Control_WalkLine_Task, 3, TASK_CNT_CONTROL_WALK_LINE);
    /* 将所有时间片任务结构体初始化并添加到侵入式链表中 END */

    /* 将需要上电运行的任务添加到侵入式链表中 BEGIN */
    timeslice_task_add(&task_adjust);
    timeslice_task_add(&task_control);
    /* 将需要上电运行的任务添加到侵入式链表中 END */

    /* 将时间片任务结构体所用 timer 初始化 BEGIN */
    HAL_TIM_Base_Start_IT(&htim_task);
    /* 将时间片任务结构体所用 timer 初始化 END */
}

void Task_Add(TimesilceTaskObj *obj)
{
    uint16_t i;
    for (i = 0; i < TASK_CHANGE_NUM; i++)
    {
        if (task_to_add[i] == NULL)
        {
            task_to_add[i] = obj;
            break;
        }
    }
}

void Task_Remove(TimesilceTaskObj *obj)
{
    uint16_t i;
    for (i = 0; i < TASK_CHANGE_NUM; i++)
    {
        if (task_to_remove[i] == NULL)
        {
            task_to_remove[i] = obj;
            break;
        }
    }
}

/* 将任务用的定时中断放在这里 BEGIN */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim_task)) // TIM6 的任务处理中断, 每 10ms 进入一次
    {
        timeslice_tick();
        timeslice_exec();

        uint16_t i;
        for (i = 0; i < TASK_CHANGE_NUM; i++)
        {
            if (task_to_add[i] != NULL)
            {
                timeslice_task_add(task_to_add[i]);
                task_to_add[i] = NULL;
            }
            else
            {
                break;
            }
        }
        for (i = 0; i < TASK_CHANGE_NUM; i++)
        {
            if (task_to_remove[i] != NULL)
            {
                timeslice_task_del(task_to_remove[i]);
                task_to_remove[i] = NULL;
            }
            else
            {
                break;
            }
        }
    }
}
/* 将任务用的定时中断放在这里 END */

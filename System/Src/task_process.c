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
#include "master.h"

/* 添加时间片任务结构体并 extern BEGIN */
TimesilceTaskObj task_adjust;
TimesilceTaskObj task_control;
TimesilceTaskObj task_control_walk_line;
TimesilceTaskObj task_master;
TimesilceTaskObj task_LED;
TimesilceTaskObj task_wait_rect_pos;
/* 添加时间片任务结构体并 extern END */

/* 任务增删的列表 BEGIN */
// 任务不能在执行任务轮询的时候增删，统一在退出任务轮询后增删
// 一次可以添加 TASK_CHANGE_NUM 个
#define TASK_CHANGE_NUM 7
TimesilceTaskObj *(task_to_add[TASK_CHANGE_NUM]) = {NULL};
TimesilceTaskObj *(task_to_remove[TASK_CHANGE_NUM]) = {NULL};
/* 任务增删的列表 END */

void TaskProcess_Init(void)
{
    /* 将所有时间片任务结构体初始化并添加到侵入式链表中 BEGIN */
    timeslice_task_init(&task_adjust, Adjust_Task, 1, TASK_CNT_ADJUST);
    timeslice_task_init(&task_control, Control_Task, 2, TASK_CNT_CONTROL);
    timeslice_task_init(&task_control_walk_line, Control_WalkLine_Task, 3, TASK_CNT_CONTROL_WALK_LINE);
    timeslice_task_init(&task_master, Master_Task, 4, TASK_CNT_MASTER);
    timeslice_task_init(&task_LED, LED_Task, 5, TASK_CNT_LED);
    timeslice_task_init(&task_wait_rect_pos, Control_WaitRectPos_Task, 6, TASK_CNT_WAIT_RECT_POS);
    /* 将所有时间片任务结构体初始化并添加到侵入式链表中 END */

    /* 将需要上电运行的任务添加到侵入式链表中 BEGIN */
    timeslice_task_add(&task_master);
    timeslice_task_add(&task_control);
    timeslice_task_add(&task_LED);
    /* 将需要上电运行的任务添加到侵入式链表中 END */

    /* 将时间片任务结构体 (和其他) 所用 timer 初始化 BEGIN */
    HAL_TIM_Base_Start_IT(&htim_task);
    HAL_TIM_Base_Start_IT(&htim_reset);
    /* 将时间片任务结构体所用 timer 初始化 END */
}

void LED_Task(void)
{
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
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

unsigned int Task_GetNum(void)
{
    return timeslice_get_task_num();
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

    if (htim == (&htim_reset))
    {
        /* key_reset 按键复位 BEGIN */
        static uint16_t KEY_DELAY = 2u;
        static uint16_t cnt_reset = 0;

        if (HAL_GPIO_ReadPin(key_reset_GPIO_Port, key_reset_Pin) == GPIO_PIN_RESET)
            cnt_reset++;
        else
            cnt_reset = 0;

        if (cnt_reset == KEY_DELAY)
            Control_ResetTo_Origin();
        /* key_reset 按键复位 END */
    }
}
/* 将任务用的定时中断放在这里 END */

/**
 * @file master.c
 * @author dokee (dokee.39@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-08-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "main.h"
#include "master.h"
#include "OLED.h"
#include "task_process.h"
#include "control.h"
#include "adjust.h"
#include "receive.h"

#define KEY_DELAY 2u

#define TASK_SHOW_NUM 3

typedef enum
{
    task_adjust_ID0 = 0,
    task_walk_boundary_ID1 = 1,
    task_walk_rect_ID2 = 2,
} task_show_ID_t;

void Master_Task(void)
{
    static task_show_ID_t task_show_ID = task_adjust_ID0;

    static uint8_t cnt_switch = 0;
    static uint8_t cnt_OK = 0;

    if (HAL_GPIO_ReadPin(key_switch_GPIO_Port, key_switch_Pin) == GPIO_PIN_RESET)
        cnt_switch++;
    else
        cnt_switch = 0;

    if (cnt_switch == KEY_DELAY)
    {
        task_show_ID++;
        if (task_show_ID >= TASK_SHOW_NUM)
            task_show_ID = task_adjust_ID0;
    }

    switch (task_show_ID)
    {
    case task_adjust_ID0:
        OLED_ShowString(1, 1, "--> ADJUST ?    ");
        OLED_ShowString(2, 1, "                ");
        if (has_adjusted)
            OLED_ShowString(3, 1, "  has adjusted  ");
        else
            OLED_ShowString(3, 1, "  not adjusted  ");
        OLED_ShowString(4, 1, "                ");
        break;
    case task_walk_boundary_ID1:
        OLED_ShowString(1, 1, "--> BOUNDARY ?  ");
        OLED_ShowString(2, 1, "                ");
        OLED_ShowString(3, 1, "                ");
        OLED_ShowString(4, 1, "                ");
        break;
    case task_walk_rect_ID2:
        OLED_ShowString(1, 1, "--> WALK RECT ? ");
        OLED_ShowString(2, 1, "                ");
        OLED_ShowString(3, 1, "                ");
        OLED_ShowString(4, 1, "                ");
        break;
    default:
        break;
    }

    if (HAL_GPIO_ReadPin(key_OK_GPIO_Port, key_OK_Pin) == GPIO_PIN_RESET)
        cnt_OK++;
    else
        cnt_OK = 0;

    if (cnt_OK == KEY_DELAY)
    {
        switch (task_show_ID)
        {
        case task_adjust_ID0:
            Task_Add(&task_adjust);
            Task_Remove(&task_master);
            OLED_ShowString(1, 1, "---  ADJUST  ---");
            OLED_ShowString(2, 1, "                ");
            OLED_ShowString(3, 1, "                ");
            OLED_ShowString(4, 1, "                ");
            break;
        case task_walk_boundary_ID1:
            Control_WalkQuadrangle(&boundary);
            Task_Remove(&task_master);
            OLED_ShowString(1, 1, "--- BOUNDARY ---");
            OLED_ShowString(2, 1, "                ");
            OLED_ShowString(3, 1, "                ");
            OLED_ShowString(4, 1, "                ");
            break;
        case task_walk_rect_ID2:
            OLED_ShowString(1, 1, "-- WALK  RECT --");
            OLED_ShowString(2, 1, "                ");
            OLED_ShowString(3, 1, "                ");
            OLED_ShowString(4, 1, "                ");
            Receive_Reset(&uart_receive_with_K210);
            Task_Add(&task_wait_rect_pos);
            Task_Remove(&task_master);
            break;
        default:
            break;
        }
    }
}

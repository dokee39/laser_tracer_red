/**
 * @file adjust.c
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

#define KEY_DELAY 2u          // 表示按键消抖的时间，为 TIM_TASK_INTERVAL 的倍数 -> 50ms
#define KEY_CONTIUNE_TIME 40u // 表示按键连续触发的时间，为 TIM_TASK_INTERVAL 的倍数 -> 1000ms
#define KEY_CONTIUNE_CONTIUNE_TIME 20u // 表示按键再次连续触发的时间，为 TIM_TASK_INTERVAL 的倍数 -> 500ms

void Adjust_ZoomIn(void)
{
    boundary.dot1.x -= 0.5f;
    boundary.dot1.y += 0.5f;
    boundary.dot2.x += 0.5f;
    boundary.dot2.y += 0.5f;
    boundary.dot3.x += 0.5f;
    boundary.dot3.y -= 0.5f;
    boundary.dot4.x -= 0.5f;
    boundary.dot4.y -= 0.5f;
}

void Adjust_ZoomOut(void)
{
    boundary.dot1.x += 0.5f;
    boundary.dot1.y -= 0.5f;
    boundary.dot2.x -= 0.5f;
    boundary.dot2.y -= 0.5f;
    boundary.dot3.x -= 0.5f;
    boundary.dot3.y += 0.5f;
    boundary.dot4.x += 0.5f;
    boundary.dot4.y += 0.5f;
}

/**
 * @brief
 *
 */
void Adjust_Task(void)
{
    static uint16_t cnt_zoom_in = 0;
    static uint16_t cnt_zoom_out = 0;

    if (HAL_GPIO_ReadPin(adjust_zoom_in_GPIO_Port, adjust_zoom_in_Pin) == GPIO_PIN_RESET)
        cnt_zoom_in++;
    else
        cnt_zoom_in = 0;
    if (HAL_GPIO_ReadPin(adjust_zoom_out_GPIO_Port, adjust_zoom_out_Pin) == GPIO_PIN_RESET)
        cnt_zoom_out++;
    else
        cnt_zoom_out = 0;

    if (cnt_zoom_in == KEY_DELAY)
    Adjust_ZoomIn();
    else if (cnt_zoom_in == KEY_CONTIUNE_TIME)
    {
        cnt_zoom_in = KEY_CONTIUNE_CONTIUNE_TIME;
        Adjust_ZoomIn();
    }

    if (cnt_zoom_in == KEY_DELAY)
    Adjust_ZoomOut();
    else if (cnt_zoom_in == KEY_CONTIUNE_TIME)
    {
        cnt_zoom_in = KEY_CONTIUNE_CONTIUNE_TIME;
        Adjust_ZoomOut();
    }
}

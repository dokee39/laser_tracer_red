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

#include <math.h>

#include "adjust.h"
#include "main.h"
#include "control.h"
#include "basic.h"
#include "task_process.h"
#include "motor.h"

#include "OLED.h"

adjust_range_t adjust_range;

#define KEY_DELAY 2u // 表示按键消抖的时间，为 TIM_TASK_INTERVAL 的倍数 -> 50ms
// #define KEY_CONTIUNE_TIME 40u // 表示按键连续触发的时间，为 TIM_TASK_INTERVAL 的倍数 -> 1000ms
// #define KEY_CONTIUNE_CONTIUNE_TIME 20u // 表示按键再次连续触发的时间，为 TIM_TASK_INTERVAL 的倍数 -> 500ms

#define ADJUST_STEP 0.35f // 调整的步进

/**
 * @brief
 *
 */
void Adjust_Task(void)
{
    static uint16_t cnt_up = 0;
    static uint16_t cnt_down = 0;
    static uint16_t cnt_left = 0;
    static uint16_t cnt_right = 0;

    static uint16_t cnt_OK = 0;
    static uint8_t is_last_OK = 0;

    float delta_degree_x = 0.0f;
    float delta_degree_y = 0.0f;
    
    float origin_xdeg_temp = 0.0f;
    float origin_ydeg_temp = 0.0f;

    if (HAL_GPIO_ReadPin(adjust_up_GPIO_Port, adjust_up_Pin) == GPIO_PIN_RESET)
        cnt_up++;
    else
        cnt_up = 0;
    if (HAL_GPIO_ReadPin(adjust_down_GPIO_Port, adjust_down_Pin) == GPIO_PIN_RESET)
        cnt_down++;
    else
        cnt_down = 0;
    if (HAL_GPIO_ReadPin(adjust_left_GPIO_Port, adjust_left_Pin) == GPIO_PIN_RESET)
        cnt_left++;
    else
        cnt_left = 0;
    if (HAL_GPIO_ReadPin(adjust_right_GPIO_Port, adjust_right_Pin) == GPIO_PIN_RESET)
        cnt_right++;
    else
        cnt_right = 0;

    if (cnt_up >= KEY_DELAY)
    {
        cnt_up = 0;
        delta_degree_y += ADJUST_STEP;
    }
    if (cnt_down >= KEY_DELAY)
    {
        cnt_down = 0;
        delta_degree_y -= ADJUST_STEP;
    }
    if (cnt_left >= KEY_DELAY)
    {
        cnt_left = 0;
        delta_degree_x -= ADJUST_STEP;
    }
    if (cnt_right >= KEY_DELAY)
    {
        cnt_right = 0;
        delta_degree_x += ADJUST_STEP;
    }

    if (delta_degree_x != 0 || delta_degree_y != 0)
    {
        Control_Move(delta_degree_x, delta_degree_y);
    }

    if (HAL_GPIO_ReadPin(adjust_OK_GPIO_Port, adjust_OK_Pin) == GPIO_PIN_RESET)
    {
        if (!is_last_OK)
        {
            cnt_OK++;
            is_last_OK = 1;

            switch (cnt_OK)
            {
            case 1:
                x_deg_offset = motor_x_degree;
                y_deg_offset = motor_y_degree;
                OLED_ShowString(4, 1, "OK: 1 offset  ");
                break;
            case 2:
                boundary.dot1.x.deg = motor_x_degree;
                boundary.dot1.y.deg = motor_y_degree;
                deg2shift_cal(&boundary.dot1);
                OLED_ShowString(4, 1, "OK: 2 rect1   ");
                break;
            case 3:
                boundary.dot2.x.deg = motor_x_degree;
                boundary.dot2.y.deg = motor_y_degree;
                deg2shift_cal(&boundary.dot2);
                OLED_ShowString(4, 1, "OK: 3 rect2   ");
                break;
            case 4:
                boundary.dot3.x.deg = motor_x_degree;
                boundary.dot3.y.deg = motor_y_degree;
                deg2shift_cal(&boundary.dot3);
                OLED_ShowString(4, 1, "OK: 4 rect3   ");
                break;
            case 5:
                boundary.dot4.x.deg = motor_x_degree;
                boundary.dot4.y.deg = motor_y_degree;
                deg2shift_cal(&boundary.dot4);
                OLED_ShowString(4, 1, "OK: 5 rect4   ");
                break;
            default:
                cnt_OK = 0;
                OLED_ShowString(4, 1, "OK: EXEC     ");
                Control_WalkQuadrangle(&boundary);
                Task_Remove(&task_adjust);
                break;
            }
        }
    }
    else
    {
        is_last_OK = 0;
    }
}

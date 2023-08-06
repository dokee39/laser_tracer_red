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

uint16_t has_adjusted = 0;

adjust_range_t adjust_range;

#define KEY_DELAY 2u // 表示按键消抖的时间，为 TIM_TASK_INTERVAL 的倍数 -> 50ms
#define KEY_DELAY_FAST 40u // 表示按键进入快速模式的时间

#define ADJUST_STEP 0.05f // 调整的步进
#define ADJUST_STEP_LARGE 0.5f // 调整的快步进


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

    if (cnt_up >= KEY_DELAY_FAST)
        delta_degree_y += ADJUST_STEP_LARGE;
    else if (cnt_up >= KEY_DELAY)
        delta_degree_y += ADJUST_STEP;

    if (cnt_down >= KEY_DELAY_FAST)
        delta_degree_y -= ADJUST_STEP_LARGE;
    else if (cnt_down >= KEY_DELAY)
        delta_degree_y -= ADJUST_STEP;
    
    if (cnt_left >= KEY_DELAY_FAST)
        delta_degree_x -= ADJUST_STEP_LARGE;
    else if (cnt_left >= KEY_DELAY)
        delta_degree_x -= ADJUST_STEP;
    
    if (cnt_right >= KEY_DELAY_FAST)
        delta_degree_x += ADJUST_STEP_LARGE;
    else if (cnt_right >= KEY_DELAY)
        delta_degree_x += ADJUST_STEP;

    if (delta_degree_x != 0 || delta_degree_y != 0)
    {
        Control_Move(delta_degree_x, delta_degree_y);
    }

    if (HAL_GPIO_ReadPin(key_OK_GPIO_Port, key_OK_Pin) == GPIO_PIN_RESET)
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
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(3, 1, "  OK: 1 adjust  ");
                OLED_ShowString(4, 1, "  set: offset   ");
                break;
            case 2:
                x_deg_offset = motor_x_degree;
                y_deg_offset = motor_y_degree;
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(3, 1, "  OK: 2 offset  ");
                OLED_ShowString(4, 1, "  set: rect1    ");
                break;
            case 3:
                boundary.dot1.x.deg = motor_x_degree;
                boundary.dot1.y.deg = motor_y_degree;
                deg2shift_cal(&boundary.dot1);
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(3, 1, "  OK: 3 rect1   ");
                OLED_ShowString(4, 1, "  set: rect2    ");
                break;
            case 4:
                boundary.dot2.x.deg = motor_x_degree;
                boundary.dot2.y.deg = motor_y_degree;
                deg2shift_cal(&boundary.dot2);
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(3, 1, "  OK: 4 rect2   ");
                OLED_ShowString(4, 1, "  set: rect3    ");
                break;
            case 5:
                boundary.dot3.x.deg = motor_x_degree;
                boundary.dot3.y.deg = motor_y_degree;
                deg2shift_cal(&boundary.dot3);
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(3, 1, "  OK: 5 rect3   ");
                OLED_ShowString(4, 1, "  set: rect4    ");
                break;
            case 6:
                boundary.dot4.x.deg = motor_x_degree;
                boundary.dot4.y.deg = motor_y_degree;
                deg2shift_cal(&boundary.dot4);
                OLED_ShowString(2, 1, "     ALL OK!    ");
                OLED_ShowString(3, 1, "  OK: 6 rect4   ");
                OLED_ShowString(4, 1, "    --> exit    ");
                break;
            default:
                cnt_OK = 0;
                Task_Remove(&task_adjust);
                Control_WalkTo_Origin();
                has_adjusted = 1;
                break;
            }
        }
    }
    else
    {
        is_last_OK = 0;
    }
}

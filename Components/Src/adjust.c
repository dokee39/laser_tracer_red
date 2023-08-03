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

#include "OLED.h"

adjust_range_t adjust_range;

#define KEY_DELAY 2u // 表示按键消抖的时间，为 TIM_TASK_INTERVAL 的倍数 -> 50ms
// #define KEY_CONTIUNE_TIME 40u // 表示按键连续触发的时间，为 TIM_TASK_INTERVAL 的倍数 -> 1000ms
// #define KEY_CONTIUNE_CONTIUNE_TIME 20u // 表示按键再次连续触发的时间，为 TIM_TASK_INTERVAL 的倍数 -> 500ms

#define ADJUST_STEP 0.2f // 调整的步进

void Adjust_DotInLine_Cal(dot_in_line_t *pdot_in_line, float theta1, float theta2)
{
    float l2;
    float x = 25.0f;
    theta1 = theta1 / 180.0f * 3.1415f;
    theta2 = theta2 / 180.0f * 3.1415f;

    l2 = (pow(sin(theta1), 2) + pow(sin(theta2), 2)) / pow(sin(theta1 + theta2), 2);
    l2 = 2 * l2 - 1;
    l2 = sqrt(l2 / pow(x, 2));

    if (theta1 > theta2)
    {
        float l1 = (sin(theta2) / sin(theta1 + theta2)) * 2 * l2;
        float h = (l2 * l1) / sin(theta1);
        pdot_in_line->deg = -acos(h / l2) * 180.0f / 3.1415f;
        pdot_in_line->pos = -sqrt(pow(l2, 2) + pow(h, 2));
    }
    else
    {
        float l3 = (sin(theta1) / sin(theta1 + theta2)) * 2 * l2;
        float h = (l2 * l3) / sin(theta2);
        pdot_in_line->deg = acos(h / l2) * 180.0f / 3.1415f;
        pdot_in_line->pos = sqrt(pow(l2, 2) + pow(h, 2));
    }
}

void Adjust_DotH_Cal(void)
{
    Adjust_DotInLine_Cal(&dot_h.x, -((boundary.dot1.x.deg_normal + boundary.dot4.x.deg_normal) / 2), ((boundary.dot2.x.deg_normal + boundary.dot3.x.deg_normal) / 2));
        Adjust_DotInLine_Cal(&dot_h.x, -((boundary.dot3.y.deg_normal + boundary.dot4.y.deg_normal) / 2), ((boundary.dot1.y.deg_normal + boundary.dot2.y.deg_normal) / 2));
}

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

    if (cnt_up >= KEY_DELAY)
    {
        cnt_up = 0;
        delta_degree_x += ADJUST_STEP;
    }
    if (cnt_down >= KEY_DELAY)
    {
        cnt_down = 0;
        delta_degree_x -= ADJUST_STEP;
    }
    if (cnt_left >= KEY_DELAY)
    {
        cnt_left = 0;
        delta_degree_y += ADJUST_STEP;
    }
    if (cnt_right >= KEY_DELAY)
    {
        cnt_right = 0;
        delta_degree_y -= ADJUST_STEP;
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
                boundary.dot1.x.deg = motor_x_degree;
                boundary.dot1.y.deg = motor_y_degree;
                deg2normal_cal(&boundary.dot1);
                OLED_ShowString(4, 1, "OK: 1        ");
                break;
            case 2:
                boundary.dot2.x.deg = motor_x_degree;
                boundary.dot2.y.deg = motor_y_degree;
                deg2normal_cal(&boundary.dot2);
                OLED_ShowString(4, 1, "OK: 2        ");
                break;
            case 3:
                boundary.dot3.x.deg = motor_x_degree;
                boundary.dot3.y.deg = motor_y_degree;
                deg2normal_cal(&boundary.dot3);
                OLED_ShowString(4, 1, "OK: 3        ");
                break;
            case 4:
                boundary.dot4.x.deg = motor_x_degree;
                boundary.dot4.y.deg = motor_y_degree;
                deg2normal_cal(&boundary.dot4);
                OLED_ShowString(4, 1, "OK: 4        ");
                break;
            case 5:
                Adjust_DotH_Cal();
                OLED_ShowString(4, 1, "OK: ALL      ");
                break;
            default:
                cnt_OK = 0;
                OLED_ShowString(4, 1, "OK: 0        ");
                break;
            }
        }
    }
    else
    {
        is_last_OK = 0;
    }
}

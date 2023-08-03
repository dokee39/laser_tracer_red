/**
 * @file degree.c
 * @brief 角度控制
 */

#include "tim.h"
#include "motor.h"
#include "main.h"

#include <stdio.h>
#include "OLED.h"

static void abs_limit(float *a, float ABS_MAX)
{
    if (*a > ABS_MAX)
    {
        *a = ABS_MAX;
    }
    if (*a < -ABS_MAX)
    {
        *a = -ABS_MAX;
    }
}

void motor_enable(void)
{
    __HAL_TIM_SET_COMPARE(&htim_motor, TIM_CHANNEL_MOTOR_X, (uint16_t)(((0.0f + 135.0f) / 270.0f * 2000.0f) + 500.0f));
    __HAL_TIM_SET_COMPARE(&htim_motor, TIM_CHANNEL_MOTOR_Y, (uint16_t)(((0.0f + 90.0f) / 180.0f * 2000.0f) + 500.0f));
    HAL_TIM_PWM_Start(&htim_motor, TIM_CHANNEL_MOTOR_X);
    HAL_TIM_PWM_Start(&htim_motor, TIM_CHANNEL_MOTOR_Y);
}

void motor_output(float degree_x, float degree_y)
{
    // TODO 临时，放在这里是不合适的
    char str[10] = "00.00";
    sprintf(str, "%.2f", degree_x);
    OLED_ShowString(2, 1, "x_deg: ");
    OLED_ShowString(2, 8, str);
    sprintf(str, "%.2f", degree_y);
    OLED_ShowString(3, 1, "y_deg: ");
    OLED_ShowString(3, 8, str);
    // END

    abs_limit(&degree_x, DRGREE_MAX);
    degree_x = -degree_x;
    __HAL_TIM_SET_COMPARE(&htim_motor, TIM_CHANNEL_MOTOR_X, (uint16_t)(((degree_x + 135.0f) / 270.0f * 2000.0f) + 500.0f));
    abs_limit(&degree_y, DRGREE_MAX);
    __HAL_TIM_SET_COMPARE(&htim_motor, TIM_CHANNEL_MOTOR_Y, (uint16_t)(((degree_y + 90.0f) / 180.0f * 2000.0f) + 500.0f));
}

/**
 * @file degree.c
 * @brief 角度控制
 */

#include "tim.h"
#include "motor.h"
#include "main.h"

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
    HAL_TIM_PWM_Start(&htim_motor, TIM_CHANNEL_MOTOR_X);
    HAL_TIM_PWM_Start(&htim_motor, TIM_CHANNEL_MOTOR_Y);
}

void motor_output(float degree_x, float degree_y)
{
    abs_limit(&degree_x, DRGREE_MAX);
    degree_x = -degree_x;
    __HAL_TIM_SET_COMPARE(&htim_motor, TIM_CHANNEL_MOTOR_X, (uint16_t)(((degree_x + 135.0f) / 270.0f * 2000.0f) + 500.0f));
    abs_limit(&degree_y, DRGREE_MAX);
    __HAL_TIM_SET_COMPARE(&htim_motor, TIM_CHANNEL_MOTOR_Y, (uint16_t)(((degree_y + 90.0f) / 180.0f * 2000.0f) + 500.0f));
}

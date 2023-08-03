/**
 * @file adjust.h
 * @author dokee (dokee.39@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-08-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _ADJUST_H_
#define _ADJUST_H_

typedef struct
{
    float x_min;
    float x_mid;
    float x_max;
    float x_h_pos;
    float x_h_degree;
    float y_min;
    float y_mid;
    float y_max;
    float y_h_pos;
    float y_h_degree;
} adjust_range_t;

extern adjust_range_t adjust_range;

void Adjust_Task(void);

#endif // !_ADJUST_H_

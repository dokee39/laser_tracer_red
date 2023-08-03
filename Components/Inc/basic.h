/**
 * @file basic.h
 * @author dokee (dokee.39@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _BASIC_H_
#define _BASIC_H_

typedef struct
{
    float pos;
    float deg;
    float deg_normal;
} dot_in_line_t;

typedef struct
{
    dot_in_line_t x;
    dot_in_line_t y;
} dot_t;

/*
 *********
 * 1   2 *
 *       *
 * 4   3 *
 *********
 */
typedef struct
{
    dot_t dot1;
    dot_t dot2;
    dot_t dot3;
    dot_t dot4;
} quadrangle_t;

void deg2normal_cal(dot_t *pdot);
void normal2deg_cal(dot_t *pdot);
void dot_init(dot_t *pdot, float xpos, float ypos, float xdeg, float ydeg);

#endif // !_BASIC_H_

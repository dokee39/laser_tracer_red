/**
 * @file degree.h
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "main.h"

#define DRGREE_MAX 45

extern float x_deg_offset;
extern float y_deg_offset;

void motor_enable(void);
void motor_output(float degree_x, float degree_y);

#endif // !_MOTOR_H_

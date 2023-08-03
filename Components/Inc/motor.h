/**
 * @file degree.h
 */

#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "main.h"

#define DRGREE_MAX 45

void motor_enable(void);
void motor_output(float degree_x, float degree_y);

#endif // !_MOTOR_H_

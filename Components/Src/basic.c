/**
 * @file basic.c
 * @author dokee (dokee.39@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-08-03
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <math.h>
#include "basic.h"
#include "control.h"

// TODO 如果计算时间太长的话就改成插值
// TODO 找出所有可能的除 0
// TODO 检查三角变换

// TODO 暂时直接用线性的，如果实际效果不好再换
void pos2shift_cal(dot_t *pdot)
{
    if (pdot->x.pos == 0 && pdot->y.pos == 0)
    {
        // (0, 0)
        pdot->x.pos_shift = 0;
        pdot->y.pos_shift = 0;
    }
    else if (pdot->x.pos + pdot->y.pos >= 0)
    {
        if (pdot->y.pos - pdot->x.pos <= 0)
        {
            // right
            float y = (pdot->y.pos / pdot->x.pos) * 25.0f;
            float ratio_c = pdot->x.pos / 25.0f; // centre_line
            float ratio_b = (y + 25.0f) / 50.0f; // boundary
            // xpos_shift_boundary
            float xpos_s_b = ratio_b * boundary.dot2.x.pos_shift + (1 - ratio_b) * boundary.dot3.x.pos_shift;
            float ypos_s_b = ratio_b * boundary.dot2.y.pos_shift + (1 - ratio_b) * boundary.dot3.y.pos_shift;
            pdot->x.pos_shift = xpos_s_b * ratio_c;
            pdot->y.pos_shift = ypos_s_b * ratio_c;
        }
        else
        {
            // up
            float x = (pdot->x.pos / pdot->y.pos) * 25.0f;
            float ratio_c = pdot->y.pos / 25.0f; // centre_line
            float ratio_b = (x + 25.0f) / 50.0f;    // boundary
            // xpos_shift_boundary
            float xpos_s_b = ratio_b * boundary.dot2.x.pos_shift + (1 - ratio_b) * boundary.dot1.x.pos_shift;
            float ypos_s_b = ratio_b * boundary.dot2.y.pos_shift + (1 - ratio_b) * boundary.dot1.y.pos_shift;
            pdot->x.pos_shift = xpos_s_b * ratio_c;
            pdot->y.pos_shift = ypos_s_b * ratio_c;
        }
    }
    else
    {
        if (pdot->y.pos - pdot->x.pos <= 0)
        {
            // down
            float x = (pdot->x.pos / pdot->y.pos) * (-25.0f);
            float ratio_c = pdot->y.pos / (-25.0f); // centre_line
            float ratio_b = (x + 25.0f) / 50.0f; // boundary
            // xpos_shift_boundary
            float xpos_s_b = ratio_b * boundary.dot3.x.pos_shift + (1 - ratio_b) * boundary.dot4.x.pos_shift;
            float ypos_s_b = ratio_b * boundary.dot3.y.pos_shift + (1 - ratio_b) * boundary.dot4.y.pos_shift;
            pdot->x.pos_shift = xpos_s_b * ratio_c;
            pdot->y.pos_shift = ypos_s_b * ratio_c;
        }
        else
        {
            // left
            float y = (pdot->y.pos / pdot->x.pos) * (-25.0f);
            float ratio_c = pdot->x.pos / (-25.0f); // centre_line
            float ratio_b = (y + 25.0f) / 50.0f;    // boundary
            // xpos_shift_boundary
            float xpos_s_b = ratio_b * boundary.dot1.x.pos_shift + (1 - ratio_b) * boundary.dot4.x.pos_shift;
            float ypos_s_b = ratio_b * boundary.dot1.y.pos_shift + (1 - ratio_b) * boundary.dot4.y.pos_shift;
            pdot->x.pos_shift = xpos_s_b * ratio_c;
            pdot->y.pos_shift = ypos_s_b * ratio_c;
        }
    }
}

void shift2deg_cal(dot_t *pdot)
{
    pdot->x.deg = atan2(pdot->x.pos_shift, sqrt(pow(h, 2) + pow(pdot->y.pos_shift, 2))) * 180 / 3.14;
    pdot->y.deg = atan2(pdot->y.pos_shift, sqrt(pow(h, 2) + pow(pdot->x.pos_shift, 2))) * 180 / 3.14;
}

void deg2shift_cal(dot_t *pdot)
{
    float k_x = tan(pdot->x.deg / 180 * 3.14); // tan(theta_x)
    float k_y = tan(pdot->y.deg / 180 * 3.14);
    pdot->x.pos_shift = sqrt((pow(k_x, 2) * (pow(k_y, 2) + 1) * pow(h, 2)) / (1 - pow(k_x, 2) * pow(k_y, 2)));
    pdot->y.pos_shift = sqrt((pow(k_y, 2) * (pow(k_x, 2) + 1) * pow(h, 2)) / (1 - pow(k_x, 2) * pow(k_y, 2)));
    if (pdot->x.deg < 0)
        pdot->x.pos_shift = -pdot->x.pos_shift;
    if (pdot->y.deg < 0)
        pdot->y.pos_shift = -pdot->y.pos_shift;
}

void dot_init(dot_t *pdot, float xpos, float ypos)
{
    pdot->x.pos = xpos;
    pdot->y.pos = ypos;
    pos2shift_cal(pdot);
    shift2deg_cal(pdot);
}

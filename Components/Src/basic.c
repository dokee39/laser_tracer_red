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
// TODO 另外不能为 0 的问题？ atan2()
void deg2normal_cal(dot_t *pdot)
{
    pdot->x.deg_normal = pdot->x.deg * (atan2(pdot->x.pos - dot_h.x.pos, h) / atan2(pdot->x.pos - dot_h.x.pos, sqrt(pow(h, 2) + pow(pdot->y.pos - dot_h.y.pos, 2))));
    pdot->y.deg_normal = pdot->y.deg * (atan2(pdot->y.pos - dot_h.y.pos, h) / atan2(pdot->y.pos - dot_h.y.pos, sqrt(pow(h, 2) + pow(pdot->x.pos - dot_h.x.pos, 2))));
}

void normal2deg_cal(dot_t *pdot)
{
    pdot->x.deg_normal = pdot->x.deg * (atan2(pdot->x.pos - dot_h.x.pos, sqrt(pow(h, 2) + pow(pdot->y.pos - dot_h.y.pos, 2))) / atan2(pdot->x.pos - dot_h.x.pos, h));
    pdot->y.deg_normal = pdot->y.deg * (atan2(pdot->y.pos - dot_h.y.pos, sqrt(pow(h, 2) + pow(pdot->x.pos - dot_h.x.pos, 2))) / atan2(pdot->y.pos - dot_h.y.pos, h));
}

void pos2deg_cal(dot_t*pdot)
{
    pdot->x.deg_normal = atan2(pdot->x.pos - dot_h.x.pos, h);
    pdot->y.deg_normal = atan2(pdot->y.pos - dot_h.y.pos, h);
    normal2deg_cal(pdot);
}

void dot_init(dot_t *pdot, float xpos, float ypos, float xdeg, float ydeg)
{
    pdot->x.pos = xpos;
    pdot->y.pos = ypos;
    pdot->x.deg = xdeg;
    pdot->y.deg = ydeg;
    deg2normal_cal(pdot);
}





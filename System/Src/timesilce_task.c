/**
 * @file task.c
 * @author dokee (dokee.39@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-07-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "timesilce_task.h"

static LIST_HEAD(timeslice_task_list);

/**
 * @brief 
 * 
 */
void timeslice_exec(void)
{
    ListObj *node;
    TimesilceTaskObj *task;

    list_for_each(node, &timeslice_task_list)
    {
        task = list_entry(node, TimesilceTaskObj, timeslice_task_list);
        if (task->is_run == TASK_RUN)
        {
            task->task_hdl();
            task->is_run = TASK_STOP;
        }
    }
}

/**
 * @brief 
 * 
 */
void timeslice_tick(void)
{
    ListObj *node;
    TimesilceTaskObj *task;

    list_for_each(node, &timeslice_task_list)
    {
        task = list_entry(node, TimesilceTaskObj, timeslice_task_list);
        if (task->timer != 0)
        {
            task->timer--;
            if (task->timer == 0)
            {
                task->is_run = TASK_RUN;
                task->timer = task->timeslice_len;
            }
        }
    }
}

unsigned int timeslice_get_task_num(void)
{
    return list_len(&timeslice_task_list);
}

/**
 * @brief 
 * 
 * @param obj 
 * @param task_hdl 
 * @param id 
 * @param timeslice_len 
 */
void timeslice_task_init(TimesilceTaskObj *obj, void (*task_hdl)(void), unsigned int id, unsigned int timeslice_len)
{
    obj->id = id;
    obj->is_run = TASK_STOP;
    obj->task_hdl = task_hdl;
    obj->timer = timeslice_len;
    obj->timeslice_len = timeslice_len;
}

/**
 * @brief 
 * 
 * @param obj 
 */
void timeslice_task_add(TimesilceTaskObj *obj)
{
    list_insert_before(&timeslice_task_list, &obj->timeslice_task_list);
}

/**
 * @brief 
 * 
 * @param obj 
 */
void timeslice_task_del(TimesilceTaskObj *obj)
{
    if (timeslice_task_isexist(obj))
        list_remove(&obj->timeslice_task_list);
    else
        return;
}

/**
 * @brief 
 * 
 * @param obj 
 * @return unsigned char 
 */
unsigned char timeslice_task_isexist(TimesilceTaskObj *obj)
{
    unsigned char isexist = 0;
    ListObj *node;
    TimesilceTaskObj *task;

    list_for_each(node, &timeslice_task_list)
    {
        task = list_entry(node, TimesilceTaskObj, timeslice_task_list);
        if (obj->id == task->id)
            isexist = 1;
    }

    return isexist;
}

/**
 * @brief 
 * 
 * @param obj 
 * @return unsigned int 
 */
unsigned int timeslice_get_task_timeslice_len(TimesilceTaskObj *obj)
{
    return obj->timeslice_len;
}

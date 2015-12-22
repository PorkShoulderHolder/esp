#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#ifndef _Env_
#define _Env_

struct Env{
    char cmdName[81];
    char* args;
    struct t_entry *var_table;
    xQueueHandle queue_handle;
    xTaskHandle task_handle;
};

#endif
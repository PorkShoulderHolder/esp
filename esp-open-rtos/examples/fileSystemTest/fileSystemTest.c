#include "task.h"
#include "queue.h"
#include <spiffs.h>

//simple file from running test of file

void user_init(void)
{
    uart_set_baud(0, 9600);
    test_spiffs();
}
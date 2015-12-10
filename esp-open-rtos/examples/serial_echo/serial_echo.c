/* Extremely simple example that just reads from stdin and echoes back on stdout
 *
 * Has an easter egg, which is if you type "QUACK" it'll quack 3 times back at you.
 *
 * This example code is in the public domain
 */
/* Very basic example that just demonstrates we can run at all!
 */
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void task1(void *pvParameters)
{
    //xQueueHandle *queue = (xQueueHandle *)pvParameters;
    printf("Hello from task1!\r\n");
    uint32_t count = 0;
    while(1) {
        printf("task 1: %d\n", count);
        count++;
    }
}

void task2(void *pvParameters)
{
    printf("Hello from task 2!\r\n");
    //xQueueHandle *queue = (xQueueHandle *)pvParameters;
    uint32_t count = 0;
    while(1) {
        printf("task 2: %d\n", count);
        count++;
    }
}

static xQueueHandle mainqueue;

void user_init(void)
{
    uart_set_baud(0, 9600);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());
    mainqueue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(task1, (signed char *)"tsk1", 256, &mainqueue, 2, NULL);
    xTaskCreate(task2, (signed char *)"tsk2", 256, &mainqueue, 2, NULL);
}

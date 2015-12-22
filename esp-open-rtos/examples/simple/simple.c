/* Very basic example that just demonstrates we can run at all!
 */
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

static xQueueHandle mainqueue;


void task1(void *pvParameters)
{
    xQueueHandle *queue = (xQueueHandle *)pvParameters;
    printf("Hello from task1!\r\n");
    uint32_t count = 0;
    while(1) {
        vTaskDelay(100);
        xQueueSend(*queue, &count, 0);
        count++;
    }
}


void task3(void *pvParameters)
{
    printf("Hello from task 3!\r\n");
    xQueueHandle *queue = (xQueueHandle *)pvParameters;
    while(1) {
        uint32_t count;
        if(xQueueReceive(*queue, &count, 1000)) {
            printf("three Got %u\n", count);
        } else {
            printf("No msg :(\n");
        }
    }
}

void task2(void *pvParameters)
{
    printf("Hello from task 2!\r\n");
    xQueueHandle *queue = (xQueueHandle *)pvParameters;
    while(1) {
        uint32_t count;
        if(xQueueReceive(*queue, &count, 1000)) {
            printf("two Got %u\n", count);
            if (count % 6 == 0)
            {
                printf("new task\n");
                xTaskCreate(task3, (signed char *)"tsk3", 256, &mainqueue, 0, NULL);
            }
        } else {
            printf("No msg :(\n");
        }

    }
}

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());
    mainqueue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(task1, (signed char *)"tsk1", 256, &mainqueue, 0, NULL);
    xTaskCreate(task2, (signed char *)"tsk2", 256, &mainqueue, 0, NULL);
}

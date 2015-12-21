/* Very basic example that just demonstrates we can run at all!
 */
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <esp8266.h>
#include <esp/uart.h>
#include <stdio.h>

#define MAX_ARGC (10)


struct Cmd{
    char name[81];
    void (*cmdfn)(void* args);
};

struct Env{
    char cmdName[81];
    char* args;
};

struct Cmd cmds[20];
int cmdIndex = 0;

void hello_world(void* args){
    printf("\nhello world!");
}

void powers_of_two(void* args){
    int power = 2;
    while(true){
        power*=2;
        printf("power: %d\n", power);
    }
}

void register_cmd(void (*cmdfn)(void*), const char* name){
    strcpy(cmds[cmdIndex].name, name);
    cmds[cmdIndex].cmdfn = cmdfn;
    cmdIndex++;
}

static void create_process(void *pvParameters){
    struct Env* env = (struct Env*)pvParameters;
    for(int i = 0; i < cmdIndex; i++){
        printf("%d", i);
        printf("%s\n", cmds[i].name);
        if(strncmp(cmds[i].name, env->cmdName, sizeof(env->cmdName)) == 0){
            cmds[i].cmdfn(NULL);
            vTaskDelete(NULL);
       }
    }
    printf("Error: %s is not a valid command\n", env->cmdName);
    vTaskDelete(NULL);
}

struct Env env;


static void run_command(char *cmd){
    char *argv[MAX_ARGC];
    int argc = 1;
    char *temp, *rover;
    memset((void*) argv, 0, sizeof(argv));
    argv[0] = cmd;
    rover = cmd;

    while(argc < MAX_ARGC && (temp = strstr(rover, " "))) {
        rover = &(temp[1]);
        argv[argc++] = rover;
        *temp = 0;
    }

    if(strncmp(argv[0], "end", 3) == 0){
        //we'll disable interupts here for a moment
        printf("ending user processes:\n");
        //then we'll continue
    }

    strcpy(env.cmdName, argv[0]);
    xTaskCreate(create_process, (signed char *)"create_process", 1024, &env, 0, NULL);
}


static void repl(void *pvParameters)
{
    char ch;
    char cmd[81];
    int i = 0;
    printf("\n\n\nWelcome Friend :\n");
    printf(">> ");
    while(1) {
        if (read(0, (void*)&ch, 1)) { // 0 is stdin
            printf("%c", ch);
            if (ch == '\n' || ch == '\r') {
                run_command(cmd);
                cmd[i] = 0;
                i = 0;
                printf("\n");
                printf(">> ");
            } else {
                if (i < sizeof(cmd)) cmd[i++] = ch;
            }
        }
    }
}




void user_init(void)
{
    uart_set_baud(0, 9600);
    register_cmd(&powers_of_two, (const char*)"two");
    register_cmd(&hello_world, (const char*)"hello");
    register_cmd(&powers_of_two, (const char*)"two");
    xTaskCreate(repl, (signed char *)"repl", 4096, NULL, 0, NULL);

}

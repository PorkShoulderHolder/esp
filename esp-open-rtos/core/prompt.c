/* Very basic example that just demonstrates we can run at all!
 */
#include "prompt.h"

#define MAX_ARGC (10)

static unsigned int TASKDONE;

struct Cmd{
    char name[81];
    char * (*cmdfn)(void* args);
};


struct Cmd cmds[20];
int cmdIndex = 0;

char* hello_world(void* args){
    char *out = "";
    sprintf(out,"\nhello world!");
    return out;
}

char* powers_of_two(void* args){
    int power = 2;
    while(true){
        power*=2;
        char * out = "";
        sprintf(out,"power: %d\n", power);
        return out;
    }
}

void register_cmd(void (*cmdfn)(void*), const char* name){
    strcpy(cmds[cmdIndex].name, name);
    cmds[cmdIndex].cmdfn = cmdfn;
    cmdIndex++;
}

char * create_process(void *pvParameters){
    struct Env* env = (struct Env*)pvParameters;
    printf("received command %s\n", env->cmdName);
    printf("supposed tobe %s\n", cmds[1].name);


    for(int i = 0; i < cmdIndex; i++){
        printf("%d", i);
        printf("%s\n", cmds[i].name);
        size_t size = strlen(env->cmdName) > strlen(cmds[i].name) ? strlen(cmds[i].name) : strlen(env->cmdName); 
        printf("%d\n", size);
        if(strncmp(cmds[i].name, env->cmdName, size) == 0){
            return cmds[i].cmdfn(NULL);
            xQueueSend((xQueueHandle)env->queue_handle, &TASKDONE, 0);
            vTaskDelete(NULL);
       }
    }
    char *out = "";
    sprintf(out, "Error: %s is not a valid command\n", env->cmdName);
    return out;
    vTaskDelete(NULL);
}

char * run_command(char *cmd, struct Env *env){
    /* 
        run as 
            run_command(cmd, NULL); 
        to create a new environment
    */

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
    unsigned l = strlen(argv[argc - 1]) - 1;
    
    for (int i = 0; i < argc; ++i)
    {
        printf("arg %d: %s\n", i, argv[i]);
    }

    memmove(argv[argc - 1] + l, argv[argc - 1] + l + 1, strlen(argv[argc - 1]) - l);

    for (int i = 0; i < argc; ++i)
    {
        printf("arg %d: %s\n", i, argv[i]);
    }

    if(strncmp(argv[0], "end", 3) == 0){
        //we'll disable interupts here for a moment
        printf("ending user processes:\n");
        //then we'll continue
    }

    strcpy(env->cmdName, argv[0]);
    printf("received command %s\n", cmd);
    printf("received command %s\n", env->cmdName);
    return create_process((void *)env);
    //xTaskCreate(create_process, (signed char *)"create_process", 1024, env, 0, env->task_handle);
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
                struct Env e;
                run_command(cmd, &e);
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

void prompt_init(void){
    TASKDONE = 12;
    register_cmd(&powers_of_two, (const char*)"two");
    register_cmd(&hello_world, (const char*)"hello");
}



void user_initd(void)
{
    uart_set_baud(0, 115200);
    register_cmd(&powers_of_two, (const char*)"two");
    register_cmd(&hello_world, (const char*)"hello");
    xTaskCreate(repl, (signed char *)"repl", 4096, NULL, 0, NULL);
}

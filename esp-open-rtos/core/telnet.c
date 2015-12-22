#include "telnet.h"

void telnetconn_callback(struct netconn * conn, enum netconn_evt evt, u16_t len) {

    if (evt == NETCONN_EVT_RCVPLUS) {
        printf("recieved message \n");
    }
}


void telnet_init_callback(){
    struct netconn *conn = netconn_new_with_callback(NETCONN_TCP, telnetconn_callback);
    netconn_bind(conn, IP_ADDR_ANY, 80);
    netconn_listen(conn);
}

void process_buffer(struct netbuf *buf, struct Env *env){
    
    u16_t data_len = 0;
    data_len = netbuf_len(buf);
    char data[data_len];
    netbuf_copy(buf, data, data_len);
    run_command(data, env);
}

void manage_conn(void *conn_ptr){
    struct netconn *conn = (struct netconn*)(conn_ptr);
    struct netbuf *buf = netbuf_new();
    printf("starting connection\n");
    uint8_t err;
    err_t write_err;
    
    struct Env env;
    env.queue_handle = xQueueCreate(10, sizeof(unsigned int));
    xTaskHandle task_handle;
    env.task_handle = &task_handle;
    eTaskState ts; 
    while((err = netconn_recv(conn, &buf)) == ERR_OK) {
        char* out;
        char* resp = "";
        process_buffer(buf, &env);
        ts = eTaskGetState(env.task_handle);
        unsigned int q_buffer;
        if(xQueueReceive(env.queue_handle, &q_buffer, 0)){
            out = (char *) q_buffer;
        }
        else{
            continue;
        }
        if(ts == eDeleted || ts == eSuspended || (*q_buffer) == TASKDONE){
            printf("finished\n");
            sprintf(resp, "%s>", out);
        }
        else{
            sprintf(resp, "%s", out);
        } 
        netbuf_delete(buf);
        write_err = netconn_write(conn, resp, strlen(resp), 1);
        if(write_err < 0)
            printf("write ERRNO: %d\n", write_err);
    }
    printf("ERRNO: %d\n", err);
    printf("deleting connection\n");
    netconn_close(conn);
    vTaskDelete(NULL);
}

void run_server(){
    printf("starting server...\n");
    struct ip_info pTempIp;
    sdk_wifi_get_ip_info(0x00, &pTempIp);
    printf("--IP:\"%d.%d.%d.%d\"\r\n", IP2STR(&pTempIp.ip));
    struct netconn *conn; 
    conn = netconn_new(NETCONN_TCP);
    struct netconn *new_conn;

    netconn_bind(conn, IP_ADDR_ANY, 80);
    netconn_listen(conn);

    while ( true )
    {
       uint8_t err = netconn_accept(conn, &new_conn);
       if ( err != ERR_OK )
          continue;
       char *resp = ">";
       err = netconn_write(new_conn, resp, strlen(resp), 1);
       printf("message recvd \n");
       xTaskCreate(&manage_conn, (signed char *)"connection_task", 2048, new_conn, 2, NULL);
    }
    printf("deleting server\n");
    netconn_close(conn);
    vTaskDelete(NULL);
}

void print_ip(uint32_t ip)
{
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8) & 0xFF;
    bytes[2] = (ip >> 16) & 0xFF;
    bytes[3] = (ip >> 24) & 0xFF;   
    printf("IP: %d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);        
}

void telnet_init(void){

    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());
 
    struct sdk_station_config config = {
        .ssid = WIFI_SSID,
        .password = WIFI_PASS,
    };

    /* required to call wifi_set_opmode before station_set_config */
    sdk_wifi_station_disconnect();
    sdk_wifi_set_opmode(STATION_MODE);
    sdk_wifi_station_set_config(&config);
    sdk_wifi_station_connect();

    printf("connecting...\n");
    int status = 0; 
    uint32_t inc = 1;
    while(status == 0){
        if(inc % 10000000 == 0){
            printf("status: %d\n", status);
            status = sdk_wifi_station_get_connect_status(); 
        }
        inc++;
    }
    printf("connected!\n");


    struct ip_info info;

    if(sdk_wifi_get_ip_info(STATION_IF, &info)){
        printf("has valid addr...\n");
        print_ip(info.ip.addr);

        xTaskCreate(&run_server, (signed char *)"runserver_task", 256, NULL, 2, NULL);
    }
    else{
        printf("not so sure...\n");
        print_ip(info.ip.addr);
        printf("error getting ip addr, task not started\n");
    }
}
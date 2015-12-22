
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

#include "queue.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"

#include "lwip/dns.h"
#include "lwip/api.h"
#include "ssid_config.h"
#include "env.h"
#include "prompt.h"


void telnet_init(void);

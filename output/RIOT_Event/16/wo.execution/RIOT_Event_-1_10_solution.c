#include "unistd.h"
#include "timex.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdbool.h"
#include "stddef.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "ztimer.h"
#include "time_units.h"
#include "msg.h"
#include "mutex.h"
#include "thread.h"
#include "mbox.h"
#include "event/thread.h"
#include "event/callback.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "esp_wifi.h"
#include "esp_system.h"

#define INTERVAL (ZTIMER_SEC_DIVIDER)

ztimer_t timer;

void callback(void *arg) {
    (void)arg;
    printf("Periodic callback executed\n");
}

int main(void) {
    printf("Starting event periodic example.\n");
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
    ztimer_start(ZTIMER_MSEC, &timer, callback, NULL);
    
    while (1) {
        sleep(1);
        printf("Running...\n");
    }
    return 0;
}

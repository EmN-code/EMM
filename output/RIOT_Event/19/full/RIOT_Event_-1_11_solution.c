
#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/gcoap.h"
#include "mbox.h"
#include "msg.h"
#include "mutex.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "net/emcute.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "time_units.h"

#define TIMER_INTERVAL 1000000 // 1 second in microseconds

static void callback_function(void *arg)
{
    (void)arg;
    printf("Callback called every second\n");
}

int main(void)
{
    ztimer_t timer;
    ztimer_init(ZTIMER_MSEC, NULL);
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL, callback_function, NULL);

    while (1) {
        // Event loop, put thread to sleep to save power.
        usleep(TIMER_INTERVAL);
    }

    return 0;
}
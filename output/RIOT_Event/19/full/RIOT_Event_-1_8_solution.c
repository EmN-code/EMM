#include "ztimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "msg.h"
#include "mutex.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "time_units.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "timex.h"
#include "mbox.h"
#include "unistd.h"
#include "thread.h"
#include "event/thread.h"

#define INTERVAL_SEC (1U)  // 1 second interval

static void callback(void *arg) {
    printf("Event triggered\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_init(&evtimer, callback);

    evtimer_event_t event;
    evtimer_set(&event, INTERVAL_SEC * US_PER_SEC, evtimer_event_periodic, callback);

    evtimer_add(&evtimer, &event);
    
    while (1) {
        evtimer_periodic_interval(&evtimer);
        usleep(INTERVAL_SEC * US_PER_SEC);
    }
    return 0;
}
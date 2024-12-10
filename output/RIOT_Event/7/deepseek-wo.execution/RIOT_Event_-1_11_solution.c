
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "time_units.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stddef.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "timex.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "stdatomic.h"
#include "mutex.h"
#include "esp_system.h"
#include "esp_wifi.h"

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static evtimer_event_t event;

void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&evtimer, event_periodic_callback, NULL);

    // Add the event to the event timer with a 1-second interval
    xtimer_ticks32_t interval = xtimer_ticks_from_usec(TIMER_INTERVAL);
    evtimer_add_event(&evtimer, &event, interval);

    // Main loop to keep the application running
    while (1) {
        thread_yield();
    }

    return 0;
}

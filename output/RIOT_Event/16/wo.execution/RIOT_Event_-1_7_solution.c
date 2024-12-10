#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "unistd.h"
#include "xtimer.h"
#include "ztimer.h"
#include "arpa/inet.h"
#include "event/callback.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "msg.h"
#include "paho_mqtt.h"

typedef struct {
    ztimer_t timer;
    uint32_t base;
    evtimer_callback_t callback;
    evtimer_event_t *events;
} evtimer_t;

evtimer_t evtimer;

void event_callback(void *arg) {
    static int count = 0;
    printf("Callback executed %d times\n", ++count);
}

int main(void) {
    // Initialize event timer
    evtimer.callback = event_callback;
    evtimer.arg = NULL;
    
    // Initialize the timer with 1 second periodicity
    ztimer_set(&evtimer.timer, 1 * US_PER_SEC);
    ztimer_periodic_start(&evtimer.timer);
    
    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1 * US_PER_SEC);  // Sleep to let the callback execute
        evtimer.callback(NULL);
    }
    
    return 0;
}
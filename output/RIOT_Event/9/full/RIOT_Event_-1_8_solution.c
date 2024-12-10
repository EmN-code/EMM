#include "architecture.h"
#include "arpa/inet.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "MQTTClient.h"
#include "msg.h"
#include "mutex.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/udp.h"
#include "net/sock/util.h"
#include "netinet/in.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "thread.h"
#include "time_units.h"
#include "timex.h"
#include "unistd.h"
#include "xtimer.h"
#include "ztimer.h"

#define INTERVAL (1U * 1000000U)  // 1 second interval

static void callback(void *arg){
    (void)arg;
    puts("Callback triggered.");
}

int main(void) {
    evtimer_t evtimer;

    // Initialize the event timer for mbox events
    evtimer_init_mbox(&evtimer);

    // Initialize mbox and msg queue
    msg_t msg_queue[8]; // Must be a power of two
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, 8);

    evtimer_mbox_event_t evtimer_event;

    evtimer_event.event.periodic.interval = INTERVAL;

    evtimer_event.event.callback = callback;
    evtimer_event.event.arg = NULL;

    evtimer_add_mbox(&evtimer, &evtimer_event, &mbox);

    while (1) {
        msg_t msg;
        if (_mbox_get(&mbox, &msg, 1)) {
            // Process message
            callback(NULL);
        }
    }
    return 0;
}
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stddef.h"
#include "unistd.h"
#include "timex.h"
#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "thread.h"
#include "mutex.h"
#include "architecture.h"
#include "kernel_defines.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "net/gcoap.h"
#include "net/emcute.h"

void event_periodic_callback(void *arg) {
    (void)arg;
    printf("Event triggered\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_mbox_event_t event;
    mbox_t mbox;
    msg_t msg_queue[8];

    printf("Starting event periodic example\n");

    // Initialize the mbox and msg queue
    msg_init_queue(msg_queue, 8);
    mbox_init(&mbox, msg_queue, 8);

    // Initialize the event timer to use mbox
    evtimer_init_mbox(&evtimer);

    // Prepare event and set it to trigger every second
    event.event.period = 1;
    event.event.callback = event_periodic_callback;

    // Add the event to the timer
    evtimer_add_mbox(&evtimer, &event, &mbox);

    while (1) {
        // Process the mbox events
        msg_t msg;
        if (_mbox_get(&mbox, &msg, 1) == 1) {
            // Handle the mbox event
            evtimer_event_t *ev = (evtimer_event_t *)msg.content.ptr;
            if (ev->callback) {
                ev->callback(ev);
            }
        }
    }

    return 0;
}
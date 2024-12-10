
#include "net/gcoap.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "mutex.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "net/sock/util.h"
#include "stdlib.h"
#include "net/sock/udp.h"
#include "sys/socket.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "net/emcute.h"
#include "unistd.h"
#include "stdatomic.h"
#include "MQTTClient.h"

#define PERIODIC_CALLBACK_INTERVAL (1 * US_PER_SEC)

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

void periodic_callback(void) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer for mbox events
    evtimer_init_mbox(&evtimer);

    // Add a periodic event to the event timer
    evtimer_mbox_event_t periodic_event;
    periodic_event.event.offset = 0;
    periodic_event.event.interval = PERIODIC_CALLBACK_INTERVAL;
    periodic_event.event.handler = (evtimer_cb_t)periodic_callback;
    evtimer_add_mbox(&evtimer, &periodic_event, &mbox);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        if (_mbox_get(&mbox, &msg, 1)) {
            // Handle the received message
            printf("Message received!\n");
        }
    }

    return 0;
}

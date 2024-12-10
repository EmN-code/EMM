
#include "esp_system.h"
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

#define QUEUE_SIZE 8

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Create an event for the periodic callback
    evtimer_mbox_event_t event;
    event.event.callback = (evtimer_cb_t)periodic_callback;
    event.event.arg = NULL;

    // Add the event to the event timer with a 1-second interval
    ztimer_set(ZTIMER_MSEC, &event.event.timer, 1000);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        if (mbox_get(&mbox, &msg, 1) == 1) {
            // Handle the received message
            printf("Message received!\n");
        }
    }

    return 0;
}

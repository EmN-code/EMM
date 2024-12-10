
#include "paho_mqtt.h"
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "unistd.h"
#include "stdatomic.h"
#include "MQTTClient.h"

#define QUEUE_SIZE (16)

static msg_t msg_queue[QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

static void periodic_callback(void *arg)
{
    printf("Periodic callback triggered\n");
    // Add your periodic task here
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize the event timer for mailbox events
    evtimer_init_mbox(&evtimer);

    // Add a periodic event to the event timer
    ztimer_t periodic_timer;
    periodic_timer.callback = periodic_callback;
    periodic_timer.arg = NULL;
    periodic_timer.interval = ZTIMER_SEC(1);

    // Start the periodic timer
    ztimer_set(&periodic_timer);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            // Handle the received message
            printf("Message received\n");
        }
    }

    return 0;
}

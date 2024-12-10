
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
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "MQTTClient.h"
#include "mma8x5x_params.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[16];
static mbox_t mbox;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Set up the periodic event timer
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    // Create the periodic event
    evtimer_msg_event_t event;
    event.event.handler = (ev_callback_t)periodic_callback;
    event.event.arg = NULL;

    // Add the event to the timer with a 1-second interval
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Handle the received message if needed
    }

    return 0;
}


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
#include "dht_params.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "thread.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "MQTTClient.h"

#define MSG_QUEUE_SIZE (8)

static msg_t msg_queue[MSG_QUEUE_SIZE];

void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
    // Add your periodic task logic here
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the periodic event timer
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    // Create an event for the periodic callback
    evtimer_msg_event_t event;
    event.msg.type = 0; // Set the message type as needed
    event.msg.content.ptr = NULL; // Set the message content as needed

    // Set the periodic event to trigger every second
    ztimer_set(ZTIMER_MSEC, &event.event.timer, 1000);

    // Add the event to the event timer
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Main loop to handle messages
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == 0) {
            periodic_callback(msg.content.ptr);
        }
    }

    return 0;
}

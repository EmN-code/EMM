#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "string.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "kernel_defines.h"
#include "mutex.h"
#include "stdlib.h"
#include "time_units.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "dht.h"
#include "dht_params.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "MQTTClient.h"
#include "paho_mqtt.h"
#include "event.h"
#include "mbox.h"

static msg_t msg_queue[8];
static mbox_t mbox;

event_queue_t queue;

static void _event_handler(event_t *event)
{
    (void)event; // to suppress unused variable warning
    printf("Event triggered!\n");
}

int main(void)
{
    /* Initialize message queue */
    msg_init_queue(msg_queue, 8);
    printf("Message queue initialized.\n");

    /* Initialize mbox */
    mbox_init(&mbox, msg_queue, 8);
    printf("Mbox initialized.\n");

    /* Initialize the event queue */
    event_queue_init(&queue);

    evtimer_t evtimer;
    evtimer_mbox_event_t event;
    event.event.event.handler = _event_handler; // Assign handler

    /* Initialize event timer and add event to it */
    evtimer_init_mbox(&evtimer);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    /* Simulate the event */
    while (true) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        printf("Event posted to mbox!\n");
        mbox_put(&mbox, NULL, 0);  // Post a message to the mbox
    }

    return 0;
}

#include "esp_system.h"
#include "esp_wifi.h"
#include "net/gcoap.h"
#include "paho_mqtt.h"
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

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    msg_t msg_queue[8];
    mbox_t mbox;

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer for mailbox events
    evtimer_init_mbox(&timer.event_timer);

    // Add a periodic event to the event timer
    evtimer_mbox_event_t event;
    evtimer_add_mbox(&timer.event_timer, &event, &mbox);

    // Set the timer to trigger the periodic callback every second
    xtimer_set(&timer, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg, 1);  // Blocking call to get messages from the mailbox
        // Handle the message if needed
    }

    return 0;
}

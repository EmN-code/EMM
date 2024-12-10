
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <timex.h>
#include <xtimer.h>
#include <unistd.h>
#include <mutex.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <mbox.h>
#include <msg.h>
#include <event/thread.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mma8x5x_regs.h>
#include <dht_params.h>
#include <dht.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <ztimer.h>
#include <time_units.h>
#include <stddef.h>

#define STACKSIZE 512
#define PRIORITY 5

static msg_t msg_queue[8];
static mbox_t mbox;
static evtimer_t evtimer;

static void event_handler(void *arg)
{
    (void)arg;
    printf("Event handled!\n");
}

int main(void)
{
    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer
    evtimer_init_mbox(&evtimer);

    // Create an event
    evtimer_mbox_event_t event;
    event.event.handler = event_handler;
    event.event.arg = NULL;

    // Add the event to the event timer
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Simulate an interrupt by triggering the event
    xtimer_sleep(2);

    // Check if the event was handled
    msg_t msg;
    if (mbox_get(&mbox, &msg, 0) == 1) {
        printf("Message received!\n");
    } else {
        printf("No message received!\n");
    }

    return 0;
}

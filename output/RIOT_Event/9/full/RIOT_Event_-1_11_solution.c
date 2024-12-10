#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include <ztimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdatomic.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time_units.h>
#include <thread.h>
#include <timex.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <netinet/in.h>

#include <msg.h>
#include "event/thread.h"
#include <esp_wifi.h>
#include <esp_system.h>
#include <architecture.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include "mqttclient.h"
#include <kernel_defines.h>
#include <arpa/inet.h>
#include <periph/rtc.h>
#include <xtimer.h>
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <mbox.h>
#include <paho_mqtt.h>
#include <mutex.h>

#define MSG_QUEUE_SIZE 8

static evtimer_t evtimer;
static msg_t msg_queue[MSG_QUEUE_SIZE];

void periodic_event_callback(void *arg) {
    printf("Periodic callback triggered\n");
}

int main(void) {
    // Initialize the mailbox
    mbox_t mailbox;
    msg_t mbox_queue[MSG_QUEUE_SIZE];
    mbox_init(&mailbox, mbox_queue, MSG_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_mbox_t event_mbox;
    evtimer_init_mbox(&event_mbox);

    // Prepare the periodic event
    evtimer_msg_event_t periodic_event;
    periodic_event.mbox = &mailbox;
    periodic_event.callback = periodic_event_callback;

    // Add the periodic event to the timer
    evtimer_add_mbox(&evtimer, &periodic_event, &mailbox);

    // Main loop to process events
    while (1) {
        evtimer_msg_queue_process(&event_mbox);
        xtimer_sleep(1);
    }

    return 0;
}
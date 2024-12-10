#include <ztimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <msg.h>
#include <mutex.h>
#include <evtimer_mbox.h>
#include <xtimer.h>
#include <thread.h>
#include <stdlib.h>
#include <stdio.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <architecture.h>
#include <arpa/inet.h>
#include <timex.h>
#include <time_units.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <net/gcoap.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <evtimer_msg.h>
#include <kernel_defines.h>
#include <unistd.h>

static evtimer_t evtimer;
static evtimer_mbox_event_t evtimer_event;
static mbox_t mbox;
static msg_t msg_queue[8];

void periodic_callback(void *arg) {
    printf("Periodic callback function triggered.\n");
}

int main(void) {
    puts("Starting event timer and callback setup\n");

    // Initialize mailbox
    mbox_init(&mbox, msg_queue, sizeof(msg_queue));

    // Initialize an event timer for mailbox
    evtimer_init_mbox(&evtimer);

    // Configure the event timer
    evtimer_event.event.offset = 1 * US_PER_SEC;
    evtimer_event.event.period = 1 * US_PER_SEC;
    evtimer_event.event.callback = periodic_callback;

    // Add the event to the event timer
    evtimer_add_mbox(&evtimer, &evtimer_event, &mbox);

    // Process messages indefinitely
    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);
        evtimer_mbox_process(msg.content.ptr);
    }

    return 0;
}
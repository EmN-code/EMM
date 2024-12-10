#include <ztimer.h>
#include <xtimer.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <msg.h>
#include <mbox.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/sock/util.h>
#include <net/gcoap.h>
#include <timex.h>
#include <time_units.h>
#include <unistd.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <architecture.h>
#include <kernel_defines.h>
#include <thread.h>
#include <event/thread.h>
#include <mutex.h>
#include <stdatomic.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

#define MSG_QUEUE_SIZE      (8U)

static msg_t msg_queue[MSG_QUEUE_SIZE];
static evtimer_t evtimer;
static mbox_t mbox;
static msg_t mbox_queue[MSG_QUEUE_SIZE];

void evtimer_callback(void *arg) {
    msg_t msg;
    msg.type = (uint16_t)((uintptr_t)arg);
    // Send a message to our own message inbox
    msg_send(&msg, thread_getpid());
}

int main(void) {
    xtimer_sleep(1);

    // Initialize the mailbox
    mbox_init(&mbox, mbox_queue, MSG_QUEUE_SIZE);

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Add an event to evtimer with interval of 1 second
    evtimer_mbox_event_t event;
    evtimer_mbox_init(&event, &mbox, 1000);
    evtimer_add_mbox(&evtimer, &event, &mbox);

    // Run the event timer in a loop
    while (1) {
        evtimer_run(&evtimer);
        xtimer_sleep(1);
    }

    return 0;
}

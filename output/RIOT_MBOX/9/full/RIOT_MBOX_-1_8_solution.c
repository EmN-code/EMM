#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "sys/socket.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "mutex.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "arpa/inet.h"
#include "architecture.h"
#include "MQTTClient.h"

#define MSG_QUEUE_SIZE (4U)

static msg_t _msg_queue[MSG_QUEUE_SIZE];

void *thread_handler(void *arg) {
    (void) arg;
    while (1) {
        printf("Thread running...\n");
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    // Initialize the message queue
    msg_init_queue(_msg_queue, MSG_QUEUE_SIZE);

    // Setting up the network
    Network network;
    int rc;
    while (1) {
        rc = NetworkConnect(&network, "192.168.1.1", 1883);
        if (rc != 0) {
            printf("Network connection failed. Retrying...\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Network connected successfully.\n");
            break;
        }
    }

    // Create a thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  thread_handler, NULL, "thread");

    // Setting up an event timer for periodic events
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);
    evtimer_mbox_event_t event;
    mbox_t mbox;
    msg_t mbox_queue[MSG_QUEUE_SIZE];
    mbox_init(&mbox, mbox_queue, MSG_QUEUE_SIZE);

    evtimer_add_mbox(&evtimer, &event, &mbox);

    while (1) {
        evtimer_run(&evtimer);
    }

    return 0;
}
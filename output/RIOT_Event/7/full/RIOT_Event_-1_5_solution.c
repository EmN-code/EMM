#include "net/sock/util.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "net/gcoap.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "paho_mqtt.h"
#include "stdbool.h"
#include "ztimer.h"
#include "msg.h"
#include "timex.h"
#include "stddef.h"
#include "stdio.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "time_units.h"
#include "net/sock/udp.h"
#include "event/callback.h"
#include "stdlib.h"
#include "xtimer.h"
#include "thread.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "unistd.h"
#include "event/thread.h"

#define SERVER_ADDR  "192.168.1.1"
#define SERVER_PORT  1883
#define BUF_SIZE      (128U)
#define WAIT_TIME_S   (1U) // 1 second

static msg_t msg_queue[8];  // Message queue
static evtimer_t evtimer;
static mbox_t mbox;
static char mbox_stack[THREAD_STACKSIZE_DEFAULT];

void periodic_event_handler(void *arg) {
    (void)arg;
    printf("Callback executed every second\n");
}

static void evtimer_setup(void) {
    msg_init_queue(msg_queue, 8);

    evtimer_msg_event_t event;
    evtimer_init(&evtimer, &mbox);

    event.msg.type = 0;
    event.msg.content.ptr = periodic_event_handler;
    evtimer_add_msg(&evtimer, &event, thread_getpid());
}

int main(void) {
    // Initialize networking or other setups
    while (1) {
        int rc = NetworkConnect(NULL, SERVER_ADDR, SERVER_PORT);

        if (rc != 0) {
            printf("Failed to connect. Retrying in %d second...\n", WAIT_TIME_S);
            xtimer_sleep(WAIT_TIME_S);
            continue;
        } else {
            printf("Connected successfully\n");
            break;
        }
    }

    evtimer_setup();

    while (1) {
        evtimer_msg_event_t *e;
        e = mbox_msg_wait(&mbox);

        if (e) {
            ((void (*)(void *))e->msg.content.ptr)(NULL);
        }
    }
    return 0;
}

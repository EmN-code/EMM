#include <ztimer.h>
#include <xtimer.h>
#include <unistd.h>
#include <timex.h>
#include <thread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sched.h>
#include <paho_mqtt.h>
#include <netinet/in.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <dht_params.h>
#include <dht.h>
#include <arpa/inet.h>
#include <MQTTClient.h>
#include <time_units.h>

#define EVENT_PERIOD (1U * US_PER_SEC)
#define MAIN_QUEUE_SIZE 8

static msg_t msg_queue[MAIN_QUEUE_SIZE];

void *thread_handler(void *arg) {
    (void)arg; // to avoid unused parameter warning
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        // Processing received message
        printf("Message received: Type %d\n", msg.type);
    }
    return NULL;
}

int main(void) {
    evtimer_msg_t evtimer;
    evtimer_msg_event_t event; // Initialize the event variable to avoid unused warning.

    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);

    char stack[THREAD_STACKSIZE_MAIN];
    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, thread_handler, NULL, "thread_handler");
    if (thread_pid < 0) {
        puts("Error: thread_create() failed\n");
        return 1;
    }

    evtimer_init_msg(&evtimer);
    evtimer_add_msg(&evtimer, &event, thread_pid);

    while (1) {
        ztimer_sleep(ZTIMER_USEC, EVENT_PERIOD);
        msg_t msg;
        msg.type = 0x420;
        msg.content.value = 42;
        msg_send(&msg, thread_pid);
    }
    return 0;
}
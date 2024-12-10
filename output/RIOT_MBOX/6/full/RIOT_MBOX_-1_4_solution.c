#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "stdio.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "kernel_defines.h"

#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];
static char stack[THREAD_STACKSIZE_DEFAULT];

void *message_thread(void *arg) {
    (void) arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message of type: %u\n", msg.type);
    }
    return NULL;
}

int main(void) {
    /* Initialize the message queue */
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    /* Start a thread to process messages */
    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack), 15,
                                            THREAD_CREATE_STACKTEST,
                                            message_thread, NULL, "msg_thread");
    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error: Unable to create thread\n");
        return 1;
    }

    /* Initialize an event timer message box */
    evtimer_mbox_t evtimer;
    evtimer_mbox_init(&evtimer, MAIN_QUEUE_SIZE);

    /* Set up periodic message sending */
    evtimer_msg_t evtimer_msg;
    msg.content.value = 12345;
    evtimer_mbox_add(&evtimer, &evtimer_msg, 1000000 /* 1 second */);

    printf("Message box initialized and running\n");

    /* Busy loop to simulate doing other work */
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
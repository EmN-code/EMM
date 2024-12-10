#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdatomic.h"
#include "time_units.h"
#include "netinet/in.h"
#include "timex.h"
#include "unistd.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "dht_params.h"

#ifndef THREAD_STACKSIZE_DEFAULT
#define THREAD_STACKSIZE_DEFAULT (1024)
#endif

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mbox_t mbox;
static msg_t mbox_queue[8];

static void mbox_event_callback(void *arg)
{
    msg_t msg;
    mbox_get(&mbox, &msg);
    printf("Message received from PID: %d with type %d\n", msg.sender_pid, msg.type);
}

void *thread_handler(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Processing message from PID: %d\n", msg.sender_pid);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    mbox_init(&mbox, mbox_queue, 8);

    evtimer_t evtimer;
    evtimer_mbox_event_t event;

    event.event.period = 2000000U; // Set period to 2 seconds
    event.event.callback = mbox_event_callback;
    evtimer_add_mbox(&evtimer, &event, &mbox);

    printf("Setup complete. Starting thread and mbox event handling.\n");

    kernel_pid_t thread_pid = thread_create(stack, sizeof(stack),
                                            THREAD_PRIORITY_MAIN - 1,
                                            THREAD_CREATE_STACKTEST,
                                            thread_handler, NULL, "thread");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        printf("Creating thread failed\n");
        return 1;
    }

    while (1) {
        xtimer_sleep(5);
        printf("Main loop doing work.\n");
    }
    return 0;
}

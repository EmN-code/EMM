#include "ztimer.h"
#include "xtimer.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "unistd.h"
#include "timex.h"
#include "thread.h"
#include "stdio.h"
#include "time_units.h"
#include "netinet/in.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "msg.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "arpa/inet.h"
#include "event/thread.h"

#define THREAD_STACKSIZE_MAIN    (1024)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define EVENT_INTERVAL           (5U * US_PER_SEC)  // 5 seconds in microseconds

static evtimer_t evtimer;
static evtimer_mbox_event_t evtimer_event;
static mbox_t mbox;

msg_t queue[8];

/**
 * @brief Task function to handle the incoming messages.
 */
void *msg_handler_thread(void *arg) {
    (void)arg;
    msg_t msg;
    
    while (1) {
        mbox_get(&mbox, &msg);
        printf("Received a message with type: %d\n", msg.type);
    }

    return NULL;
}

int main(void) {
    // Initialize message queue
    msg_init_queue(queue, 8);

    // Initialize mailbox
    mbox_init(&mbox, queue, 8);

    // Initialize event timer
    evtimer_init(&evtimer, EVENT_INTERVAL);

    // Create a message handler thread
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                  msg_handler_thread, NULL, "msg_handler_thread");

    // Add event to the event timer with mailbox
    evtimer_add_mbox(&evtimer, &evtimer_event, &mbox);

    puts("Event timer with message box is running...");

    // This main loop could be doing something else or just sleeping
    while (1) {
        sleep(10);  // just to keep the main thread alive
    }

    return 0;
}
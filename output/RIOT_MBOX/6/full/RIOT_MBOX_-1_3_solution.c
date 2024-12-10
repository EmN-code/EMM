#include "xtimer.h"
#include "event/thread.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "msg.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "MQTTClient.h"
#include "time_units.h"
#include "thread.h"
#include "timex.h"

// Define message queue size
#define QUEUE_SIZE (8)

// Declare message queue
static msg_t msg_queue[QUEUE_SIZE];

// Declare event
static evtimer_msg_event_t event;

// Define stack for thread
static char stack[THREAD_STACKSIZE_MAIN];

void *event_handler(void *arg)
{
    (void)arg;
    msg_t msg;
    while (1) {
        // Receive a message (blocking)
        msg_receive(&msg);
        printf("Received event at time: %u\n", (unsigned)xtimer_now());
    }
    return NULL;
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, QUEUE_SIZE);

    // Initialize evtimer_mbox
    evtimer_msg_init(&event, msg_queue, QUEUE_SIZE);

    // Create a new thread for handling events
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  event_handler, NULL, "event_handler");

    // Add event to timer with mbox functionality
    unsigned interval = 1000000; // 1 second
    evtimer_msg_event_t my_event;
    evtimer_msg_add(&my_event, interval, msg_queue);

    while (1) {
        // Main loop doing other tasks
        xtimer_sleep(10);
    }

    return 0;
}
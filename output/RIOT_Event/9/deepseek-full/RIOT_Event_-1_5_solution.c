
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stdatomic.h"

#define MSG_QUEUE_SIZE 8
#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");

    // Send a message to the main thread
    msg_t msg;
    msg.content.value = 1;  // Dummy value
    msg_send(&msg, thread_getpid());
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the mailbox
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);

    // Set up the periodic timer
    ztimer_t timer;
    timer.callback = periodic_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    ztimer_set(ZTIMER_USEC, &timer, TIMER_INTERVAL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        if (mbox_get(&mbox, &msg, 1) == 1) {
            printf("Received message with value: %u\n", (unsigned int)msg.content.value);
        }
    }

    return 0;
}

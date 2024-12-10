
#include <stdio.h>
#include "xtimer.h"
#include "msg.h"

#define MSG_QUEUE_SIZE     (8)
#define INTERVAL_MS        (1000000U)  // 1 second interval

static msg_t msg_queue[MSG_QUEUE_SIZE];

void periodic_event(void)
{
    printf("Event triggered!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Initialize the xtimer (no arguments)
    xtimer_init();

    // Set up a periodic event
    while (1) {
        xtimer_sleep(INTERVAL_MS / 1000000U);  // Convert microseconds to seconds
        periodic_event();
    }

    return 0;
}

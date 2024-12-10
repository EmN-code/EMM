
#include <stdio.h>
#include <stdlib.h>
#include <xtimer.h>
#include <msg.h>
#include <unistd.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    msg_t msg;

    // Initialize the message queue
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the callback every second
    while (1) {
        xtimer_set(&timer, PERIODIC_INTERVAL);
        msg_receive(&msg);
        event_periodic_callback(NULL);
    }

    return 0;
}

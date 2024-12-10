
#include <stdio.h>
#include <xtimer.h>
#include <msg.h>

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void event_periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Periodic event triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    msg_t msg;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the callback every second
    xtimer_set_msg(&timer, PERIODIC_INTERVAL, &msg, event_periodic_callback, NULL);

    while (1) {
        // Wait for the message to be received
        msg_receive(&msg);
    }

    return 0;
}

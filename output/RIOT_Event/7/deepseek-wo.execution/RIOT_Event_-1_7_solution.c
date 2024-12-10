
#include <stdio.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <time_units.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>
#include <net/gcoap.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;
    msg_t msg;

    // Initialize the timer
    xtimer_init();

    // Set the timer to trigger the callback every second
    xtimer_set(&timer, TIMER_INTERVAL, event_periodic_callback, NULL);

    // Main loop to keep the thread alive
    while (true) {
        thread_yield();
    }

    return 0;
}

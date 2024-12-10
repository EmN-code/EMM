
#include <stdlib.h>
#include <msg.h>
#include <time_units.h>
#include <xtimer.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void timer_callback(void *arg)
{
    (void)arg;
    // Toggle LED logic here
    printf("LED toggled\n");
}

int main(void)
{
    xtimer_t timer;

    // Initialize the timer
    xtimer_init();

    // Set the timer to trigger every second
    xtimer_set(&timer, TIMER_INTERVAL, timer_callback, NULL);

    // Main loop to keep the application running
    while (1) {
        // Sleep to prevent busy-waiting
        xtimer_usleep(100000);  // Sleep for 100ms
    }

    return 0;
}

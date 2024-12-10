#include <xtimer.h>
#include <ztimer.h>
#include <timex.h>
#include <stdio.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stddef.h>
#include <kernel_defines.h>
#include <time_units.h>
#include <stdlib.h>
#include <mbox.h>
#include <unistd.h>
#include <msg.h>

#define INTERVAL (1U * US_PER_SEC)

static ztimer_t timer;

void timer_callback(void *arg)
{
    (void)arg;
    // Simulate LED Toggle with a print statement
    puts("LED Toggled");
}

int main(void)
{
    ztimer_set(&timer, ztimer_now() + INTERVAL);

    while (1) {
        // Check if the timer has expired
        if (ztimer_elapsed(&timer)) {
            // Execute the callback function
            timer_callback(NULL);
            // Reset the timer
            ztimer_set(&timer, ztimer_now() + INTERVAL);
        }
    }

    return 0;
}
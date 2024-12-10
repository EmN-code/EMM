
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <net/sock/util.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <net/gcoap.h>

#define PERIODIC_INTERVAL (1000000U)  // 1 second in microseconds

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    puts("RIOT periodic event example");

    // Initialize the periodic timer
    xtimer_t periodic_timer;
    xtimer_init(&periodic_timer, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Start the periodic timer
    xtimer_set(&periodic_timer, PERIODIC_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        // Yield to the kernel to allow other threads to run
        thread_yield();
    }

    return 0;
}

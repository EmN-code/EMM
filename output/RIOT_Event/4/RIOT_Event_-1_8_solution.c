
#include <stdio.h>
#include <unistd.h>
#include <thread.h>
#include <mutex.h>
#include <periph/gpio.h>
#include <time_units.h>
#include <kernel_defines.h>

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    puts("RIOT periodic callback example");

    // Initialize the periodic callback
    periodic_callback_t callback;
    periodic_callback_init(&callback, ZTIMER_MSEC, &periodic_callback, NULL);

    // Start the periodic callback with a 1-second interval
    periodic_callback_start(&callback, INTERVAL_MS);

    // Main loop to keep the application running
    while (1) {
        thread_sleep();
    }

    return 0;
}

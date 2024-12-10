
#include <stdio.h>
#include <xtimer.h>
#include <periph/gpio.h>
#include <thread.h>
#include <msg.h>

#define INTERVAL_MS 1000

static void periodic_callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;
    msg_t msg;

    // Initialize the timer
    xtimer_init(&timer);

    // Set up the periodic callback
    xtimer_set_periodic(&timer, INTERVAL_MS, (xtimer_callback_t)periodic_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        msg_receive(&msg);
        xtimer_periodic_wakeup(&timer, INTERVAL_MS);
    }

    return 0;
}

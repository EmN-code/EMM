
#include <stdio.h>
#include "xtimer.h"
#include "thread.h"

#define STACKSIZE (THREAD_STACKSIZE_DEFAULT)

static char stack[STACKSIZE];

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the callback every second
    xtimer_set_periodic(&timer, XTIMER_TICKS(1000000), periodic_callback, NULL);

    // Main loop to keep the thread running
    while (1) {
        thread_yield();
    }

    return 0;
}

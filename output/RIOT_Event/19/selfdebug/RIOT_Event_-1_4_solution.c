#include <stdio.h>  // For printf
#include <thread.h> // For threading and stack initialization
#include <xtimer.h> // For the xtimer functionality

#define TIMER_INTERVAL_SEC (1U) // Timer interval in seconds
#define STACK_SIZE (THREAD_STACKSIZE_MAIN) // Stack size for the thread

static char stack[STACK_SIZE];

void event_periodic(void *arg) {
    (void)arg;
    printf("Event triggered\n");
}

int main(void) {
    // Initialize the periodic event
    xtimer_ticks32_t interval = xtimer_usec_from_ticks(xtimer_n_ticks_from_ticks64(xtimer_sec_from_interval(TIMER_INTERVAL_SEC)));
    xtimer_t periodic_timer;
    periodic_timer.callback = event_periodic;
    periodic_timer.arg = NULL;

    // Start the periodic event in a separate thread
    event_periodic_init(stack, STACK_SIZE, thread_getpid()
    xtimer_set(&periodic_timer, interval);

    // Let the main thread sleep
    thread_sleep();

    return 0;
}
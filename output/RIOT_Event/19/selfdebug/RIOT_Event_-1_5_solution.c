#include "xtimer.h" // Required for using xtimer_set_callback and xtimer_periodic_t
#include "thread.h" // Required for threading functions
#include "event.h" // Required for event related functions
#include <stdio.h> // Standard I/O functions

#define INTERVAL (1000000) // Interval for 1 second in microseconds

static char stack[THREAD_STACKSIZE_MAIN];
static void *thread_entry(void *arg);

xtimer_periodic_t periodic_timer;

static void callback(void *arg) {
    (void)arg;
    puts("Callback triggered every second.");
}

extern int main(void) {
    xtimer_periodic_init(&periodic_timer, INTERVAL);
    puts("Starting periodic timer on ESP32");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  thread_entry, NULL, "timer_thread");

    return 0;
}

static void *thread_entry(void *arg) {
    (void)arg;
    while (1) {
        xtimer_periodic_wakeup(&periodic_timer);
        callback(NULL);
    }
    return NULL;
}
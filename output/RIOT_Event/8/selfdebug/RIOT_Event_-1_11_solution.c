#include <ztimer.h>
#include <thread.h>
#include <stdio.h>

#define TIMER_INTERVAL (1000000U)  // 1 second in microsecond timer ticks

static ztimer_t timer;
static int counter = 0;

static void timer_callback(void *arg) {
    counter++;
    printf("Timer callback executed %d times.\n", counter);
    // Reset the timer for another 1 second interval
    ztimer_set_msg(ZTIMER_MSEC, &timer, TIMER_INTERVAL, thread_get_active());
}

int main(void) {
    printf("Starting the timer example...\n");

    // Initialize the ztimer and set its callback
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Start the timer for the first time
    ztimer_set_msg(ZTIMER_MSEC, &timer, TIMER_INTERVAL, thread_get_active());

    // The main thread can continue performing other tasks
    while (1) {
        // In a real program, this might contain code to do other work,
        // such as handling user input or communicating with other devices.
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
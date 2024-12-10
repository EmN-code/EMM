#include ztimer.h
#include timex.h
#include stddef.h

static void callback(void *arg) {
    // Sample callback function to print a message every time it is called
    printf("Callback function executed.\n");
}

int main(void) {
    // Initialize the ztimer library
    ztimer_clock_t *clock = ZTIMER_MSEC;
    ztimer_t timer;

    // Set the timer to call the callback function
    ztimer_set_msg(clock, &timer, 1 * ZTIMER_MSEC_PER_SEC, NULL, callback);

    // Sleep forever, let the timer manage the execution
    while (1) {
        ztimer_sleep(clock, 1 * ZTIMER_MSEC_PER_SEC);
    }

    return 0;
}
#include <ztimer.h>  // Required for timer functionalities
#include <stdio.h>   // Required for standard I/O functionalities

// Callback function to be called every second
env ztimer_t timer;

void callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Callback function executed.\n");
}

int main(void) {
    printf("Initialization and starting of periodic event callback every second.\n");
    // Initialize the timer
    ztimer_set(ZTIMER_MSEC, &timer, ZTIMER_MSEC_BASE * 1000, callback, NULL);
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep 1 second
    }
    return 0;
}
#include <ztimer.h>  // Include ztimer library for timer functionality
#include <stdio.h>    // For basic input and output

static void event_periodic_cb(void *arg) {
    (void)arg; // Unused parameter
    printf("Periodic event triggered\n");
}

int main(void) {
    ztimer_periodic_t periodic_timer;

    // Initialize a periodic ztimer
    ztimer_periodic_init(ZTIMER_SEC, &periodic_timer, event_periodic_cb, NULL, 1);
    
    // Run indefinitely
    while (1) {
        // Sleep here simulates doing other tasks in between callbacks
        ztimer_periodic_wait(&periodic_timer);
    }

    return 0;
}
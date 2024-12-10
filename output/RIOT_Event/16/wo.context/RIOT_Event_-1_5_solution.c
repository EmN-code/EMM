#include <ztimer.h>

void event_periodic_callback(void *arg) {
    // This function will be called every second
    (void)arg; // To avoid compiler warnings about unused variable
    // Logic to be executed periodically
    puts("Periodic callback executed");
}

int main(void) {
    // ZTIMER_MSEC is the timer to be used for millisecond precision
    static ztimer_periodic_t periodic_timer; 
    
    // Initialize the timer
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, event_periodic_callback, NULL, 1 * MS_PER_SEC);

    // Start the periodic callback
    ztimer_periodic_start(&periodic_timer);

    // Main loop, it could include other logic
    while (1) {
        // Simulating a delay in main loop to not consume full CPU
        ztimer_sleep(ZTIMER_MSEC, 1 * MS_PER_SEC);
    }

    return 0;
}
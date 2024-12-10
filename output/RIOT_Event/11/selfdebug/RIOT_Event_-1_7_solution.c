#include <ztimer.h>  // Correct header for ztimer
#include <stdio.h>    // Standard input/output

// Callback function that will be called periodically
void periodic_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    printf("Periodic event triggered!\n");
}

// Main function to initialize and start the periodic event
int main(void) {
    printf("Setting up periodic event every 1 second.\n");
    
    // Define and initialize a periodic timer
    ztimer_t periodic_timer;

    // Initialize periodic timer with a 1 second interval
    ztimer_set_msg(ZTIMER_SEC, &periodic_timer, ztimer_now(ZTIMER_SEC) + 1, periodic_callback, NULL);

    // Main loop
    while(1) {
      // Sleep or wait for events to keep the main loop running
      ztimer_sleep(ZTIMER_SEC, 1);
    }
    return 0;
}

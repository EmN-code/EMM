#include <ztimer.h>  // Include ztimer library for timer functions
#include <thread.h>  // Include for threading operations

#define INTERVAL (1LU * US_PER_SEC)  // Define timer interval of one second

void periodic_callback(void *arg) {
    (void)arg;
    // Your periodic task code here, executed every second.
    printf("Callback function called!\n");
}

int main(void) {
    // Initialize periodic timer
    ztimer_periodic_t periodic_timer;
    ztimer_periodic_wakeup(ZTIMER_USEC, &periodic_timer, INTERVAL);

    while (1) {
        // Wait for the next timer wakeup
        ztimer_periodic_wakeup(ZTIMER_USEC, &periodic_timer, INTERVAL);
        periodic_callback(NULL);
    }
    return 0;
}
#include <ztimer.h>
#include <ztimer/periodic.h>
#include <stdio.h>

#define INTERVAL_US   (1U * ZTIMER_USEC_PER_SEC)

static void event_periodic_callback(void *arg) {
    (void)arg;
    puts("Callback function called!");
}

int main(void) {
    puts("RIOT - ESP32 Event Timer Example");
    
    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_USEC, &periodic_timer, event_periodic_callback, NULL, INTERVAL_US);
    ztimer_periodic_start(&periodic_timer);

    while (1) {
        // Keeping the main loop running
    }
    return 0;
}
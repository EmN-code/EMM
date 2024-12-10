#include ztimer.h
#include evtimer.h
#include esp_err.h

static void event_periodic(void *arg) {
    // Placeholder callback - this can be replaced with actual functionality
    printf("Event Periodic Triggered\n");
}

int main(void) {
    // Initialize the event timer
    evtimer_t evtimer;
    evtimer_init(&evtimer, NULL);

    // Create a periodic event every second
    evtimer_msg_event_t event_msg;
    ztimer_t timer;
    timer.callback = event_periodic;
    timer.arg = NULL;
    xtimer_set(&timer, 1 * US_PER_SEC);
    
    while(1) {
        // Enter low-power mode and let the timer run. 
        // Real code would do something useful here instead of an endless loop
    }

    return 0;
}
// Description: RIOT code for ESP32 that sets up an event timer to call a callback function every second. 

#include <evtimer.h>
#include <xtimer.h>
#include <mutex.h>

#define INTERVAL (1U * US_PER_SEC) // 1 second interval

static evtimer_t evtimer;
static evtimer_event_t event;

void callback(void *arg) {
    (void)arg;
    printf("Callback triggered!\n");
}

void init_periodic_event(void) {
    evtimer_init(&evtimer, callback);
    evtimer_add(&evtimer, &event);

    // Set periodic time by re-adding the event in the queue
    while (1) {
        xtimer_usleep(INTERVAL);
        evtimer_add(&evtimer, &event);
    }
}

int main(void) {
    puts("Starting timer example...");
    init_periodic_event();
    return 0;
}
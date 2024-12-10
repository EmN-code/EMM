#include <xtimer.h>
#include <stdio.h>
#include <event.h>
#include <esp_wifi_types.h>

#define PERIODIC_INTERVAL (1U * US_PER_SEC) // 1 second

static xtimer_ticks32_t last_wakeup;
static event_periodic_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    printf("Initializing periodic event example.\n");

    // Initialize last_wakeup with the current time
    last_wakeup = xtimer_now();

    // Initialize the periodic event
    event_periodic_init(&periodic_event, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Catch the control with an infinite loop
    while (1) {
        xtimer_usleep(PERIODIC_INTERVAL);
        printf("Main loop active.\n");
    }

    return 0;
}
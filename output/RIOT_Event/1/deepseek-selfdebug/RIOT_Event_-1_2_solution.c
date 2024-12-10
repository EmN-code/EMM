
#include "event/periodic.h"
#include "ztimer.h"
#include "timex.h"

#define INTERVAL_US (1 * US_PER_SEC)  // 1 second interval

static void callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_USEC, INTERVAL_US, NULL, callback, NULL);
    event_periodic_callback_start(&event);

    while (1) {
        // Main loop
    }

    return 0;
}

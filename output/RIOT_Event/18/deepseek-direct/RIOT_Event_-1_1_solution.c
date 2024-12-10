
#include "xtimer.h"
#include "periph/gpio.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static event_t periodic_event;

void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    // Initialize the periodic event
    event_init(&periodic_event, periodic_callback, NULL);

    // Start the periodic event
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second
        event_post(&periodic_event, NULL);
    }

    return 0;
}

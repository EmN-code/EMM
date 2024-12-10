
#include "xtimer.h"
#include "timex.h"
#include "periph/gpio.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;
    xtimer_init(&timer);

    xtimer_periodic_callback_t periodic_event;
    xtimer_periodic_callback_init(&periodic_event, INTERVAL_MS * 1000, periodic_callback, NULL);

    xtimer_add_periodic(&timer, &periodic_event);

    while (1) {
        // Main loop
    }

    return 0;
}

#include <ztimer.h>
#include <stdio.h>
#include <inttypes.h>

#define MS_PER_SEC (1000U)

static void event_periodic(void *arg) {
    (void)arg;
    puts("Event triggered every second!");
}

int main(void) {
    puts("Starting periodic event example on ESP32");

    ztimer_periodic_t periodic_timer;

    /* Initialize periodic timer */
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, event_periodic, NULL, 1 * MS_PER_SEC);

    /* Start periodic timer */
    ztimer_periodic_start(&periodic_timer);

    /* Keep the program running to allow periodic events to occur */
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, MS_PER_SEC);
    }

    return 0;
}
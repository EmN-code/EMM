#include <xtimer.h>
#include <event.h>
#include <stdio.h>

void timeout_callback(void *arg) {
    printf("Periodic event triggered!\n");
}

int main(void) {
    puts("RIOT ESP32 Event Callback Example");

    event_callback_t callback = { .callback = timeout_callback, .arg = NULL };
    event_periodic_t periodic;

    if (event_periodic_init(&periodic, &callback) == 0) {
        if (event_periodic_start(&periodic, 1 * US_PER_SEC, EVENT_PRIO_MEDIUM) == 0) {
            while (1) {
                event_dispatch(EVENT_PRIO_MEDIUM);
                xtimer_sleep(1);
            }
        }
        else {
            puts("Failed to start periodic event");
        }
    }
    else {
        puts("Failed to initialize periodic event");
    }

    return 0;
}
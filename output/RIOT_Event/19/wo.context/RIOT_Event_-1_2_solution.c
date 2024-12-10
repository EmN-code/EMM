#include <xtimer.h>
#include <event/periodic.h>
#include <event.h>
#include <stdio.h>

#define INTERVAL (1000000U)  // 1 second in microseconds

static void my_event_periodic_callback(void *arg) {
    (void)arg;
    puts("Periodic event triggered!");
}

int main(void) {
    event_periodic_t my_event;

    /* Initialize the periodic event */
    event_periodic_init(&my_event, my_event_periodic_callback, NULL);
    /* Set the periodic event to trigger every second */
    event_periodic_start(&my_event, INTERVAL);

    /* Keep the main thread alive */
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}
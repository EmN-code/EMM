#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"
#include "timex.h"
#include "periph/gpio.h"

static void my_callback(void *arg) {
    (void)arg;
    printf("Callback executed!\n");
}

int main(void) {
    puts("RIOT callback example");

    /* Define a periodic event */
    event_periodic_t my_periodic_event;
    my_periodic_event.time = 1 * US_PER_SEC;  /* Set the period to 1 second */
    my_periodic_event.callback = my_callback;
    my_periodic_event.arg = NULL;

    /* Start the periodic event */
    event_periodic_start(&my_periodic_event);

    while (1) {
        /* Do nothing, the callback will handle the event */
        xtimer_sleep(1);
    }

    return 0;
}
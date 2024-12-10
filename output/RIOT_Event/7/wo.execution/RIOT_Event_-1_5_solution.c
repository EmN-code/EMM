#include <xtimer.h>
#include <stddef.h>
#include <stdatomic.h>
#include <time_units.h>
#include <stddef.h>
#include <stdatomic.h>
#include <xtimer.h>
#include <msg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>

#define PERIOD 1000000U /* 1 second (in microseconds) */

static void evtimer_callback(void *arg) {
    (void)arg;
    /* Your logic here, e.g., toggle an LED, send a message, etc. */
    printf("Callback executed.\n");
}

int main(void) {
    evtimer_msg_event_t event;
    evtimer_t evtimer;

    /* Event timer initialization */
    evtimer.event.callback = evtimer_callback;
    evtimer.event.arg = NULL;
    evtimer.event.period = PERIOD;
    evtimer_msg_handler_init(&evtimer, &event);

    /* Starting the periodic timer */
    evtimer_msg_add_timer(&evtimer, &event);

    /* A message or infinite loop to keep the main function occupied */
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}
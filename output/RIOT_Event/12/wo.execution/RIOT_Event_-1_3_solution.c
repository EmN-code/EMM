#include <xtimer.h>
#include <timex.h>
#include <stdio.h>
#include <stddef.h>
#include <stdatomic.h>
#include <time_units.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <evtimer_mbox.h>
#include <kernel_defines.h>
#include <evtimer_msg.h>

#define INTERVAL (1U * US_PER_SEC)

static void _callback(void *arg) {
    (void)arg;
    printf("Event triggered\n");
}

int main(void) {
    evtimer_t timer;
    evtimer_msg_event_t event;
    msg_t m;
    m.type = 0xABCD;  // random type, should be meaningful to the application

    evtimer_init_mbox(&timer);
    event.event.offset = INTERVAL;
    event.msg = m;

    evtimer_add_mbox(&timer, &event, NULL);
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}
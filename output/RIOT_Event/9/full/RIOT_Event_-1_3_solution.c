#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)

static void _event_periodic_callback(void *arg) {
    (void)arg;
    printf("Callback function triggered.\n");
}

int main(void) {
    printf("Event periodic example initialized.\n");
    
    evtimer_msg_event_t event;
    evtimer_add_msg(NULL, &event, 0);

    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    evtimer_add_msg(NULL, &event, 0);

    while (1) {
        sleep(1);
        _event_periodic_callback(NULL);
    }

    return 0;
}
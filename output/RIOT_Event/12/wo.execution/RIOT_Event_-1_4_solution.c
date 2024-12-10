#include "ztimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "event/thread.h"
#include "msg.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "MQTTClient.h"
#include "timex.h"
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "netinet/in.h"
#include "stdbool.h"
#include "time_units.h"
#include "stddef.h"
#include "unistd.h"
#include "stdatomic.h"

/* Declare the event handler */
static void my_event_handler(void *arg)
{
    (void)arg;
    printf("Event triggered\n");
}

int main(void)
{
    evtimer_t my_timer;
    msg_t evt_queue[8];
    msg_init_queue(evt_queue, 8);

    /* Initialize the event timer */
    evtimer_init_mbox(&my_timer);

    /* To replace evtimer_msg_handler_init and evtimer_msg_add_timer,
       directly initialize and start an event periodic timer */

    event_periodic_type_t my_event_periodic;
    event_periodic_init(&my_event_periodic, my_event_handler, NULL);
    event_periodic_start(&my_event_periodic, 1000000);
 
    while (1) {
        /* Sleep for short intervals to simulate periodic task in main */
        usleep(1000000);
    }
    return 0;
}
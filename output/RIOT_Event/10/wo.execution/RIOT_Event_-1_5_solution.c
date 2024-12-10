#include "ztimer.h"
#include "xtimer.h"
#include "unistd.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "msg.h"
#include "mbox.h"
#include "timex.h"
#include "time_units.h"
#include "thread.h"
#include "paho_mqtt.h"
#include "MQTTClient.h"
#include "kernel_defines.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"

// Callback function that will be called periodically
event_periodic_t periodic_event;

void _callback(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    evtimer_msg_t evtimer;
    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.msg.sender_pid = 0; // Assign a valid process ID
    event.event.handler = _callback; // Properly use the handler function

    kernel_pid_t target_pid = 1; // Example target PID
    evtimer_add_msg(&evtimer, &event, target_pid);

    // Start a loop that triggers the event periodically
event_periodic_init(&periodic_event, &xtimer, 1U * US_PER_SEC, _callback, NULL);
    event_periodic_start(&periodic_event);

    while (1) {
        sleep(1); // Use sleep instead of using non-standard usleep
    }

    return 0;
}
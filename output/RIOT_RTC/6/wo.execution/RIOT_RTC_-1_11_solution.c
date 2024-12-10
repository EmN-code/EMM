#include "ztimer.h"
#include "xtimer.h"
#include "stdio.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

static void alarm_callback(void *arg)
{
    (void)arg; // Unused variable
    printf("Alarm triggered!\n");
}

int main(void)
{
    evtimer_msg_event_t event;

    // Get current time and set the alarm for 5 seconds later
    ztimer_now(ZTIMER_SEC) + 5;
    
    // Initialize event timer
    evtimer_msg_init(&evtimer);

    // Prepare the event structure
    evtimer_set_msg(&event, 1, alarm_callback, NULL);

    // Add an event to trigger after 5 seconds
    evtimer_add_msg(&evtimer, &event, 5);

    // Busy-loop to simulate background activity
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
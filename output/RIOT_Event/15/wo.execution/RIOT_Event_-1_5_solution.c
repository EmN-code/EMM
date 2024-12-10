#include "xtimer.h"
#include "stdbool.h"
#include "stddef.h"
#include "thread.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "event/thread.h"
#include "event/periodic.h"
#include "timex.h"
#include "time_units.h"
#include "mbox.h"
#include "unistd.h"
#include "paho_mqtt.h"
#include "msg.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

static void event_callback(void *arg)
{
    printf("Callback function called.\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    kernel_pid_t pid = thread_get_active();

    // Initialize event timer for message passing
    evtimer_init_msg(&evtimer);
    
    // Set up periodic event
    evtimer_periodic_init(&event.event, &evtimer);

    // Set the periodic interval to 1 second
    evtimer_periodic_start(&event.event, 1000000);

    // Add the event message to the timer
    evtimer_add_msg(&evtimer, &event, pid);

    printf("Event periodic callback initialized.\n");

    // Wait here to keep the program running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        event_callback(NULL); // Call the callback
    }

    return 0;
}
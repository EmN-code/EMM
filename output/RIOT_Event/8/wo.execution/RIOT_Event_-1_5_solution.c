#include "xtimer.h"
#include "paho_mqtt.h"
#include "kernel_defines.h"
#include "mbox.h"
#include "timex.h"
#include "MQTTClient.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "ztimer.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "thread.h"
#include "time_units.h"
#include "event/periodic.h"
#include "unistd.h"
#include "msg.h"
#include "stdlib.h"

// Define an example callback function
void periodic_callback(void *arg)
{
    puts("Periodic event triggered!");
}

int main(void)
{
    // Create and initialize the event
evtimer_t evtimer;
    evtimer_mbox_event_t event_mbox[1];
    mbox_t mbox_memory;

    // Initialize the mbox
    mbox_init(&mbox_memory, NULL);

    // Initialize the event timer for mbox
    evtimer_init_mbox(&evtimer);

    // Configure the event
    event_mbox[0].event.callback = periodic_callback;
    event_mbox[0].event.arg = NULL; // Can carry any message or argument.

    // Use a periodic event or general timer to call the callback every second
    const uint32_t interval_sec = 1;

    msg_t msg;

    while (1) {
        msg_receive(&msg);
        if (msg.content.value == KERNEL_PID_UNDEF) {
            xtimer_sleep(interval_sec);
            periodic_callback(NULL);
        }
    }

    return 0;
}
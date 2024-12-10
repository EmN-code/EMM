#include <unistd.h>
#include <ztimer.h>
#include <xtimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include <paho_mqtt.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <msg.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <event/periodic.h>
#include <event/callback.h>
#include <timex.h>
#include <MQTTClient.h>
#include <time_units.h>
#include <thread.h>

// Function for the periodic event handler
void periodic_event_handler(void)
{
    printf("Periodic callback executed.\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_msg_event_t event;
    kernel_pid_t main_pid = thread_getpid();

    // Initialize event timer for message events
    evtimer_init_msg(&evtimer);

    // Set up the periodic event
    event.event.period = ztimer_usec_from_sec(1);
    event.event.callback = periodic_event_handler;
    event.msg.sender_pid = main_pid;

    // Add periodic message event
    evtimer_add_msg(&evtimer, &event, main_pid);

    printf("Event timer initialized to call back every second.\n");

    // Main loop
    while (1)
    {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
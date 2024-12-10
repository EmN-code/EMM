#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <MQTTClient.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <periph/rtc.h>
#include <paho_mqtt.h>
#include <thread.h>
#include <time_units.h>
#include <mbox.h>
#include <msg.h>
#include <ztimer.h>
#include <timex.h>
#include <event/callback.h>
#include <event/periodic.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <stdio.h>

#define PERIODIC_INTERVAL    1000000 // 1 second in microseconds

void cb_function(void *arg)
{
    (void)arg;
    printf("Callback triggered!\n");
}

int main(void)
{
    printf("Starting periodic event example...\n");

    evtimer_t evtimer;
    msg_t msg_queue[8];

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up periodic callback using RIOT's evtimer (simulating periodic behavior)
    while (1) {
        usleep(PERIODIC_INTERVAL);
        cb_function(NULL); // Manual periodic trigger
    }

    return 0;
}
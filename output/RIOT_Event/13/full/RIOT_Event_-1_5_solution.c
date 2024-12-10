#include <xtimer.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <event/thread.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <stddef.h>
#include <ztimer.h>
#include <event/callback.h>
#include <unistd.h>
#include <evtimer_msg.h>
#include <MQTTClient.h>
#include <paho_mqtt.h>
#include <time_units.h>
#include <event/periodic.h>
#include <thread.h>
#include <timex.h>
#include <periph/rtc.h>

static evtimer_t evtimer;
static void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic Event Triggered!\n");
}

int main(void)
{
    xtimer_ticks32_t interval = xtimer_ms2ticks(1000);
    evtimer_init_msg(&evtimer);
    evtimer_add_periodic(&evtimer, &interval, event_periodic_callback, NULL);
    
    while (1) {
        pause(); // Simulate waiting for interrupts
    }
    return 0;
}

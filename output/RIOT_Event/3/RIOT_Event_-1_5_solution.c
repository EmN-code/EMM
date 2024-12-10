
#include "paho_mqtt.h"
#include "unistd.h"
#include "time_units.h"
#include "thread.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "stddef.h"
#include "ztimer.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "timex.h"
#include "stdbool.h"
#include "mbox.h"
#include "stdlib.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "msg.h"
#include "dht.h"
#include "dht_params.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "periph/rtc.h"

#define INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t event;
    ztimer_clock_t *clock = ZTIMER_USEC;
    event_queue_t queue;

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, clock, &queue, periodic_callback, NULL);

    // Start the periodic callback event with a 1-second interval
    event_periodic_callback_start(&event, INTERVAL);

    // Main loop to keep the application running
    while (1) {
        thread_yield();
    }

    return 0;
}

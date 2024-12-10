
#include "event/periodic_callback.h"
#include "ztimer.h"
#include "event/thread.h"
#include "stdio.h"
#include "thread.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event_queue_init.h"
#include "event_queue_dispatch.h"
#include "mbox.h"
#include "periph/gpio.h"
#include "xtimer.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "msg.h"
#include "MQTTClient.h"
#include "dht_params.h"

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval

static event_periodic_callback_t periodic_event;
static event_queue_t event_queue;

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, &event_queue, periodic_callback, NULL);

    // Start the periodic event with a 1-second interval
    event_periodic_callback_start(&periodic_event, INTERVAL);

    // Dispatch events from the event queue
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}

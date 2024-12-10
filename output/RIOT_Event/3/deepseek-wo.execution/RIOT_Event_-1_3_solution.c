
#include "mbox.h"
#include "event/periodic_callback.h"
#include "ztimer.h"
#include "thread.h"
#include "event/thread.h"
#include "periph/gpio.h"
#include "evtimer_msg.h"
#include "xtimer.h"
#include "time_units.h"
#include "periph/rtc.h"
#include "timex.h"
#include "periph/i2c.h"
#include "evtimer_mbox.h"
#include "msg.h"
#include "MQTTClient.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static event_queue_t event_queue;
static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    // Initialize the event queue
    event_queue_init(&event_queue);

    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue, periodic_callback, NULL);

    // Start the periodic event with a 1-second interval
    event_periodic_callback_start(&periodic_event, INTERVAL_MS);

    // Main loop to dispatch events
    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}

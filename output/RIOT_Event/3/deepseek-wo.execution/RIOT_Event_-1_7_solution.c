
#include "event/callback.h"
#include "mbox.h"
#include "event/periodic_callback.h"
#include "paho_mqtt.h"
#include "ztimer.h"
#include "thread.h"
#include "dht.h"
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
#include "dht_params.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_periodic_callback_t event;
    ztimer_clock_t *clock = ZTIMER_USEC;
    event_queue_t queue;

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, clock, &queue, periodic_callback, NULL);

    // Start the periodic callback event with a 1 second interval
    event_periodic_callback_start(&event, INTERVAL_MS);

    // Dispatch events from the queue
    while (1) {
        event_queue_dispatch(&queue);
    }

    return 0;
}

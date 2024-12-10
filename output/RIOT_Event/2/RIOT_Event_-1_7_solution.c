
#include "paho_mqtt.h"
#include "unistd.h"
#include "net/sock/util.h"
#include "time_units.h"
#include "thread.h"
#include "stdio.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "net/sock/udp.h"
#include "stddef.h"
#include "ztimer.h"
#include "arpa/inet.h"
#include "stdatomic.h"
#include "MQTTClient.h"
#include "timex.h"
#include "stdbool.h"
#include "mbox.h"
#include "stdlib.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "msg.h"
#include "dht.h"
#include "dht_params.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "netinet/in.h"
#include "periph/rtc.h"

// Define the periodic callback structure
typedef struct {
    ztimer_t timer;
    event_queue_t queue;
    void (*callback)(void *);
    void *arg;
} event_periodic_callback_t;

// Initialize the periodic callback
static inline void event_periodic_callback_init(event_periodic_callback_t *event,
                                                ztimer_clock_t *clock, event_queue_t *queue,
                                                void (*callback)(void *), void *arg)
{
    event->timer.callback = callback;
    event->timer.arg = arg;
    event->queue = *queue;
}

// Start the periodic callback
static inline void event_periodic_callback_start(event_periodic_callback_t *event,
                                                 uint32_t interval)
{
    ztimer_set(event->timer.clock, &event->timer, interval);
}

// Callback function to be executed periodically
void periodic_callback(void *arg)
{
    printf("Periodic callback executed!\n");
}

int main(void)
{
    // Initialize the event queue
    event_queue_t queue;
    event_queue_init(&queue);

    // Initialize the periodic callback event
    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_MSEC, &queue, periodic_callback, NULL);

    // Start the periodic callback with an interval of 1 second
    event_periodic_callback_start(&event, 1000);

    // Main loop to handle events
    while (1) {
        event_t *event = event_queue_get(&queue);
        if (event) {
            event->handler(event);
        }
    }

    return 0;
}

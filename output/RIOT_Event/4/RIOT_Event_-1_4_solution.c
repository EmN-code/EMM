
#include "time_units.h"
#include "thread.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "ztimer.h"
#include "mma8x5x_regs.h"
#include "MQTTClient.h"
#include "timex.h"
#include "mbox.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "msg.h"
#include "dht.h"
#include "dht_params.h"
#include "evtimer_msg.h"
#include "periph/rtc.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t event;
    event_queue_t queue;

    // Initialize the event queue
    event_queue_init(&queue);

    // Initialize the periodic callback event
    event_periodic_callback_init(&event, ZTIMER_MSEC, &queue, periodic_callback, NULL);

    // Start the periodic callback with a 1-second interval
    event_periodic_callback_start(&event, INTERVAL_MS);

    // Main loop to process events
    while (1) {
        event_t *e;
        if ((e = event_get(&queue))) {
            event_handle(e);
        }
    }

    return 0;
}

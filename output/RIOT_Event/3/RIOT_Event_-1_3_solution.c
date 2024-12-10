
#include "time_units.h"
#include "thread.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "ztimer.h"
#include "timex.h"
#include "mbox.h"
#include "xtimer.h"
#include "msg.h"
#include "periph/i2c.h"
#include "dht_params.h"
#include "evtimer_msg.h"
#include "periph/rtc.h"

#define INTERVAL_SEC (1U)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t event;
    event_queue_t queue;

    event_queue_init(&queue);
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, periodic_callback, NULL);
    event_periodic_callback_start(&event, INTERVAL_SEC * US_PER_SEC);

    while (1) {
        event_t *ev = event_queue_get(&queue);
        if (ev) {
            event_handle(ev);
        }
    }

    return 0;
}

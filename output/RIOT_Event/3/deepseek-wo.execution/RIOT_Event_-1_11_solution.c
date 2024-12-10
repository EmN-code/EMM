
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

#define INTERVAL (1000000U)  // 1 second in microseconds

static event_queue_t event_queue;

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    event_queue_init(&event_queue);

    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_USEC, &event_queue, periodic_callback, NULL);
    event_periodic_callback_start(&event, INTERVAL);

    while (1) {
        event_queue_dispatch(&event_queue);
    }

    return 0;
}

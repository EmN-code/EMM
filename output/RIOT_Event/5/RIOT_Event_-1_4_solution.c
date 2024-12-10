
#include <stdio.h>
#include <periph/gpio.h>
#include <thread.h>
#include <time_units.h>
#include <paho_mqtt.h>
#include <MQTTClient.h>
#include <timex.h>
#include <mbox.h>
#include <periph/i2c.h>
#include <msg.h>
#include <dht_params.h>
#include <periph/rtc.h>

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

    /* Initialize the event queue */
    event_queue_init(&queue);

    /* Initialize the periodic callback event */
    event_periodic_callback_init(&event, ZTIMER_USEC, &queue, periodic_callback, NULL);

    /* Start the periodic callback event with a 1-second interval */
    event_periodic_callback_start(&event, INTERVAL_SEC * US_PER_SEC);

    /* Main loop to process events */
    while (1) {
        event_process(&queue);
    }

    return 0;
}

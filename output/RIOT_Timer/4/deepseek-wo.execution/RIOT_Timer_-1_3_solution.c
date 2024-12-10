
#include "periph/gpio.h"
#include "event/periodic_callback.h"
#include "event/thread.h"
#include "mbox.h"
#include "timex.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "sys/socket.h"
#include "evtimer_mbox.h"
#include "periph/rtc.h"
#include "time_units.h"
#include "ztimer.h"

#define LED_PIN GPIO_PIN(0, 16)  // Example LED pin for ESP32

static void led_toggle_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create and initialize the periodic callback event
    event_periodic_callback_t periodic_event;
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue_null, led_toggle_callback, NULL);

    // Start the periodic callback with a 1-second interval
    event_periodic_callback_start(&periodic_event, 1000);

    // Main loop to keep the application running
    while (1) {
        // Process events in the event queue
        event_loop(&event_queue_null);
    }

    return 0;
}

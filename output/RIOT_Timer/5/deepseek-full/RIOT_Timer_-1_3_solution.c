
#include "msg.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "periph/rtc.h"
#include "event/periodic_callback.h"
#include "periph/gpio.h"
#include "sys/socket.h"
#include "evtimer_msg.h"
#include "ztimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph/i2c.h"
#include "event/thread.h"
#include "xtimer.h"

#define LED_PIN GPIO_PIN(0, 13)  // Example LED pin for ESP32

static event_periodic_callback_t periodic_event;
static event_queue_t event_queue;

static void led_toggle_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize the event queue
    event_queue_init(&event_queue);

    // Create and start the periodic event
    event_periodic_callback_create(&periodic_event, ZTIMER_MSEC, 1000, &event_queue, led_toggle_callback, NULL);
    event_periodic_callback_start(&periodic_event, 1000);

    // Main loop to process events
    while (1) {
        event_t event;
        if (event_queue_get(&event_queue, &event) >= 0) {
            event_process(&event);
        }
    }

    return 0;
}

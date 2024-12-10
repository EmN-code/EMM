
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

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO pin 13 on port 0 for the LED

static volatile int led_state = 0;

static void led_toggle_callback(void *arg)
{
    (void)arg;
    led_state = !led_state;
    gpio_write(LED_PIN, led_state);
}

int main(void)
{
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Create an event queue
    event_queue_t event_queue;
    event_queue_init(&event_queue);

    // Create a periodic callback event
    event_periodic_callback_t periodic_event;
    event_periodic_callback_create(&periodic_event, ZTIMER_MSEC, 1000, &event_queue, led_toggle_callback, NULL);

    // Start the periodic event
    event_periodic_callback_start(&periodic_event);

    // Event loop
    while (1) {
        event_t *event = event_get(&event_queue);
        if (event) {
            event->handler(event);
        }
    }

    return 0;
}

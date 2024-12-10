
#include <stdio.h>
#include "periph/gpio.h"
#include "event/periodic_callback.h"
#include "dht_params.h"
#include "event/thread.h"
#include "mbox.h"
#include "timex.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x_params.h"
#include "net/sock/udp.h"
#include "MQTTClient.h"
#include "evtimer_msg.h"
#include "net/sock/util.h"
#include "kernel_defines.h"
#include "evtimer_mbox.h"
#include "periph/rtc.h"
#include "time_units.h"
#include "thread.h"
#include "ztimer.h"

#define LED_PIN GPIO_PIN(0, 16)  // Example GPIO pin for LED

static void led_toggle(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create and start the periodic callback event
    event_periodic_callback_t event;
    event_periodic_callback_init(&event, ZTIMER_MSEC, event_queue_null, led_toggle, NULL);
    event_periodic_callback_start(&event, 1000);  // 1000 ms = 1 second

    // Main loop to keep the program running
    while (1) {
        // The event will handle the LED toggling
    }

    return 0;
}

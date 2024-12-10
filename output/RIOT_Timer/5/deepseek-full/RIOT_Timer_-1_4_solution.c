
#include "net/sock/udp.h"
#include "dht_params.h"
#include "evtimer_mbox.h"
#include "event/periodic_callback.h"
#include "periph/gpio.h"
#include "sys/socket.h"
#include "time_units.h"
#include "kernel_defines.h"
#include "xtimer.h"
#include "msg.h"
#include "thread.h"
#include "mma8x5x_params.h"
#include "evtimer_msg.h"
#include "periph/i2c.h"
#include "net/sock/util.h"
#include "event/thread.h"
#include "mbox.h"
#include "periph/rtc.h"
#include "ztimer.h"
#include "timex.h"

#define LED_PIN GPIO_PIN(0, 13)

static void led_toggle_callback(void *arg)
{
    (void)arg;
    gpio_toggle(LED_PIN);
}

int main(void)
{
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Create and start the periodic callback event
    event_periodic_callback_t event;
    event_periodic_callback_create(&event, ZTIMER_MSEC, 1000, NULL, led_toggle_callback, NULL);
    event_periodic_callback_start(&event, 1000);

    // Main loop to keep the application running
    while (1) {
        // Process events or other tasks
        // For simplicity, we don't have any other tasks here
        xtimer_sleep(1);
    }

    return 0;
}

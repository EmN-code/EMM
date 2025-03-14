
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
#include "netinet/in.h"
#include "evtimer_msg.h"
#include "net/sock/util.h"
#include "sys/socket.h"
#include "evtimer_mbox.h"
#include "architecture.h"
#include "periph/rtc.h"
#include "time_units.h"
#include "thread.h"
#include "ztimer.h"

#define LED_PIN GPIO_PIN(0, 13)  // Example LED pin for ESP32

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
    event_periodic_callback_create(&event, ZTIMER_MSEC, 1000, NULL, led_toggle, NULL);

    // Start the periodic callback with the interval
    event_periodic_callback_start(&event, 1000);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}

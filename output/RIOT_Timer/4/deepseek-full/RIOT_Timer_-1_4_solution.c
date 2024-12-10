
#include "net/sock/udp.h"
#include "assert.h"
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

#define LED_PIN GPIO_PIN(0, 13)  // Example LED pin for ESP32

static void led_toggle(void) {
    gpio_toggle(LED_PIN);
}

static void periodic_callback(void *arg) {
    (void)arg;
    led_toggle();
}

int main(void) {
    // Initialize GPIO for LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Create and start the periodic callback
    event_periodic_callback_t event;
    event_periodic_callback_create(&event, ZTIMER_MSEC, 1000, NULL, periodic_callback, NULL);
    event_periodic_callback_start(&event);

    // Main loop to keep the application running
    while (1) {
        // You can add additional logic here if needed
    }

    return 0;
}


#include "xtimer.h"
#include "periph/gpio.h"
#include "event/periodic_callback.h"
#include "evtimer_mbox.h"
#include "periph/i2c.h"
#include "periph/rtc.h"
#include "evtimer_msg.h"
#include "ztimer.h"

#define LED_PIN GPIO_PIN(0, 13)  // Assuming GPIO 13 is connected to an LED
#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void _timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up the periodic timer
    xtimer_t timer;
    xtimer_set(&timer, TIMER_INTERVAL, _timer_callback, NULL);

    // Main loop to keep the program running
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second to avoid busy-waiting
    }

    return 0;
}

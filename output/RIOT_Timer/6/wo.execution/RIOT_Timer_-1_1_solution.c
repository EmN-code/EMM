#include <stdio.h>
#include <timex.h>
#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)  // Assuming the LED is connected to PORT_A, PIN 0

void timer_cb(void *arg) {
    (void)arg; // Unused
    static bool led_on = false;
    if (led_on) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
    led_on = !led_on;
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Setup a periodic timer that runs every second
    ztimer_t timer;
    ztimer_set_msg_periodic(ZTIMER_MSEC, &timer, 1000, NULL, NULL);
    ztimer_cb_init(&timer, timer_cb, NULL);

    while (1) {
        // Let the timer callback manage LED
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
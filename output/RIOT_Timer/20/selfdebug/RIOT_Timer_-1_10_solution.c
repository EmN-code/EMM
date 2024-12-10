#include <ztimer.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <xtimer.h>

#define TIMER_INTERVAL (1LU * US_PER_SEC)

static ztimer_t timer;
static int led_state = 0;

static void timer_callback(void *arg) {
    (void)arg;
    led_state = !led_state;
    gpio_write(GPIO_PIN(PORT_B, 7), led_state);
    printf("LED is now %s\n", led_state ? "ON" : "OFF");
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);
}

int main(void) {
    printf("ESP32 Timer Example\n");

    // Initialize the timer with the callback
    ztimer_set_handler(&timer, timer_callback, NULL);

    // Set the interval
    ztimer_set(ZTIMER_MSEC, &timer, TIMER_INTERVAL);

    // Initialize GPIO as output
    gpio_init(GPIO_PIN(PORT_B, 7), GPIO_OUT);

    // Enter main event loop
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }

    return 0;
}
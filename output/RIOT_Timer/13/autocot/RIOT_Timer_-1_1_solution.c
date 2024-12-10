#include <stdint.h>
#include <stdio.h>

#include "timer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 2)  // Assume GPIO 2 for LED, change as needed

void timer_callback(void *arg)
{
    (void)arg;
    static bool led_on = false;
    
    if (led_on) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }
    led_on = !led_on;
    
    printf("LED toggled.\n");
}

int main(void)
{
    printf("Timer example starting.\n");

    if (gpio_init(LED_PIN, GPIO_OUT) != 0) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    tim_t dev = 0;
    uint32_t freq = 1;  // 1 Hz for 1 second interval
    timer_cb_t cb = timer_callback;
    void *arg = NULL;

    if (timer_init(dev, freq, cb, arg) != 0) {
        printf("Error initializing timer\n");
        return 1;
    }

    timer_start(dev);

    while (1) {
        // Main loop does nothing, everything is in the callback
    }

    return 0;
}
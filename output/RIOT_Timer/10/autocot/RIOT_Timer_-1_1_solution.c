#include "xtimer.h"
#include "ztimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_B, 5) // Choose an appropriate GPIO pin

void timer_callback(void)
{
    static bool led_state = false;
    
    // Toggle LED state
    if (led_state) {
        gpio_clear(LED_PIN);
    } else {
        gpio_set(LED_PIN);
    }

    led_state = !led_state;
}

int main(void)
{
    // Initialize the GPIO pin
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer that triggers every second
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.target = ztimer_now(ZTIMER_MSEC) + 1000; // 1 second interval

    // Start the timer
    ztimer_set(ZTIMER_MSEC, &timer, 1000);

    // Keep the program running
    while (1) {
        __WFI();  // Wait for interrupt
    }

    return 0;
}
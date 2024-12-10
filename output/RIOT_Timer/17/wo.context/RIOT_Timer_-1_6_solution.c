#include <stdio.h>
#include <timex.h>
#include <ztimer.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 0)  // Assuming LED is connected to GPIO port A, pin 0

void timer_callback(void *arg)
{
    (void)arg;
    // Toggle LED pin
    gpio_toggle(LED_PIN);
    printf("LED toggled\n");
}

int main(void)
{
    printf("Timer example on ESP32\n");
    gpio_init(LED_PIN, GPIO_OUT);

    // Initialize a timer
    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set a timer to trigger every second
    ztimer_set(ZTIMER_SEC, &timer, 1);

    while (1) {
        // Sleep to save power
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}

#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(PORT_A, 5)  // Define the pin where the LED is connected

void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED state
    printf("LED toggled\n");
}

int main(void) {
    // Initialize the LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing GPIO\n");
        return 1;
    }

    printf("LED Timer example...
");

    // Initialize timer
    xtimer_ticks32_t interval = { .ticks32 = 1000000U };  // 1 second interval
    xtimer_set(&interval, timer_callback, NULL);

    // Enter main loop
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define TIMER_INTERVAL (1U * 1000000U) // Timer interval in microseconds (1 second)
#define LED_PIN GPIO_PIN(0, 2) // Example GPIO pin (ports are device-specific)

timer_t timer;

void timer_callback(void *arg) {
    (void)arg; // Unused parameter
    gpio_set(LED_PIN);
    printf("LED turned on\n");
}

int main(void) {
    puts("RIOT ESP32 Timer Example");

    // Initialize the LED pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        puts("Error: unable to initialize LED pin\n");
        return 1;
    }

    // Initialize timer
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, TIMER_INTERVAL);

    // Keeping the main thread alive
    while (1) {
        // Let the timer callback handle the LED
        xtimer_sleep(1);
    }

    return 0;
}
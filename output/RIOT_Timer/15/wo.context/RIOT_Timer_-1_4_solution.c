#include <xtimer.h>
#include <stdio.h>
#include <periph/gpio.h>

#define LED_PIN GPIO_PIN(0, 2)  // GPIO 0, pin 2 of ESP32
#define TIMER_INTERVAL (1U * 1000000U)  // Timer interval of 1 second

void blink_led(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED state
    printf("LED toggled\n");
}

int main(void) {
    // Initialize LED_PIN as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: Failed to initialize LED pin\n");
        return 1;
    }

    timex_t interval = {.seconds = 1, .microseconds = 0};
    xtimer_ticks64_t last_wakeup = xtimer_now64();

    printf("Starting LED blink timer\n");

    while (1) {
        xtimer_periodic_wakeup64(&last_wakeup, interval);  // Wait until the next tick
        blink_led(NULL);  // Call LED toggle function
    }

    return 0;
}
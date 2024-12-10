#include <xtimer.h>  // Timer header file
#include <stdio.h>  // Standard I/O header file
#include <periph/gpio.h>  // GPIO header file

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval
#define LED_PIN GPIO_PIN(PORT_A, 0)  // Define the LED pin

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED state
    printf("LED toggled\n");  // Print message to console
}

int main(void) {
    // Initialize the GPIO pin as output
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error: Unable to initialize GPIO\n");
        return 1;
    }

    // Create a timer and set it to trigger every second
    xtimer_ticks32_t last_wakeup = xtimer_now();
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
        timer_callback(NULL);  // Call the callback function
    }

    return 0;
}
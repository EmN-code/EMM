#include <timex.h>
#include <stdio.h>
#include <periph/gpio.h>
#include <event/callback.h>

#define LED_PIN (GPIO_PIN(PORT_A, 2)) // Assuming the LED is connected to pin A2

// Timer callback
void toggle_led(void *arg) {
    (void)arg;

    // Read the current LED pin state
    int led_state = gpio_read(LED_PIN);
    if (led_state < 0) {
        printf("Error reading LED state!\n");
        return;
    }

    // Toggle the LED state
    if (led_state) {
        gpio_clear(LED_PIN);
        printf("LED OFF\n");
    } else {
        gpio_set(LED_PIN);
        printf("LED ON\n");
    }
}

int main(void) {
    // Initialize the GPIO pin
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Failed to initialize LED pin\n");
        return 1;
    }

    // Set initial state
    gpio_clear(LED_PIN);

    // Set up a timer to toggle the LED every 1 second
    timex_t interval = timex_set(1, 0); // (1 second)
    timer_t timer;
    timer_init(&timer, interval, toggle_led, NULL);

    printf("Timer initialized. Toggling LED every second.\n");

    // Endless loop
    while (1) {
        // Main thread could perform other tasks in real applications
    }

    return 0;
}
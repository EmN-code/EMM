
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(0, 16)  // Assuming GPIO 16 on ESP32 is connected to an LED

void toggle_led(void) {
    static int state = 0;
    gpio_write(LED_PIN, state);
    state = !state;
}

int main(void) {
    // Initialize the LED pin as output
    gpio_init(LED_PIN, GPIO_OUT);

    // Create a timer that triggers every second
    xtimer_t timer;
    uint32_t period = 1000000;  // 1 second in microseconds

    // Initialize the timer
    xtimer_init();

    // Set the timer to call toggle_led every second
    xtimer_set(&timer, period, (void (*)(void *))toggle_led, NULL);

    // Main loop to keep the program running
    while (1) {
        // The timer will handle the LED toggling, so we just need to keep the main loop running
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}

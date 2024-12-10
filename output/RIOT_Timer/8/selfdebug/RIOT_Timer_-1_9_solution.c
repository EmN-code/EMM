// Description:
// This code configures the ESP32 to set up a timer that triggers every second and turns on an LED. It resolves the previous error by including the correct header files and ensuring the GPIO configuration is performed correctly.

#include "xtimer.h"  // Include for timer functionality
#include "timex.h"   // Include for time definitions like SEC_IN_USEC
#include "periph/gpio.h"  // Include for GPIO operations

#define LED_PIN GPIO_PIN(0, 2)  // Define the GPIO pin where the LED is connected. This might be different based on your ESP32 board configuration.

static void timer_callback(void *arg) {
    (void)arg;
    gpio_toggle(LED_PIN);  // Toggle the LED state
}

int main(void) {
    // Initialize the GPIO pin for the LED
    gpio_init(LED_PIN, GPIO_OUT);

    // Set up a timer
    xtimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set the timer to trigger every second
    xtimer_set(&timer, SEC_IN_USEC);

    // The main thread can continue to do other things or just idle
    while (1) {
        xtimer_sleep(1);  // Sleep for a second
    }

    return 0;
}